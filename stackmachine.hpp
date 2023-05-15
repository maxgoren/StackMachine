/*

MIT License

Copyright (c) 2023 Max Goren

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef STACKMACHINE_HPP
#define STACKMACHINE_HPP 
#include <iostream>
#include <vector>
using namespace std;

typedef int OPSTAT;
const OPSTAT OP_SUCCESS = 1;
const OPSTAT OP_ERROR = -1;
const OPSTAT OP_BRANCH = 2;

class StackMachine {
    private:
        int top; //top of data stack pointer
        int data[255]; //data stack
        OPSTAT push(string arg);
        OPSTAT push(int arg);
        int pop();
        OPSTAT show();
        //return stack
        int rtop;   // top of return stack pointer
        int rets[255]; //return stack
        OPSTAT pushr();
        OPSTAT popr();
        //alu
        OPSTAT ALU(string op, string arg);
        int tosr; //top of stack register
        OPSTAT cmpl();
        OPSTAT cmpg();
        OPSTAT cmpeq();
        OPSTAT add();
        OPSTAT mult();
        OPSTAT sub();
        OPSTAT div();
        OPSTAT mod();
        OPSTAT bwAnd();
        OPSTAT bwOr();
        OPSTAT bwXor();
        OPSTAT dup();
        OPSTAT swap();
        OPSTAT over();
        OPSTAT store();
        OPSTAT load();
        OPSTAT jmpnz(int addr); //jump to addr if top of stack is NOT zero
        OPSTAT jmpz(int addr);  //jump to addr if top of stack IS zero
        OPSTAT call(); //call to
        OPSTAT retf(); //return from
        void halt(); //end program.
        vector<string> smtokenizer(string expr, char DELIM);
        int programCounter;
        int memoryAddrReg;
        int randomAccessMemory[255];
    public:
        StackMachine() {
            top = 0;
            tosr = 0;
            rtop = 0;
            memoryAddrReg = 0;
            programCounter = 0;
        }
        void run(vector<string>& program);
};

OPSTAT StackMachine::cmpl() {
    return push(data[top-2] < data[top - 1]);
}

OPSTAT StackMachine::cmpg() {
    return push(data[top-2] > data[top - 1]);
}

OPSTAT StackMachine::cmpeq() {
    return push(data[top-2] == data[top - 1]);
}

/// @brief bitwise AND top two items on data stack
/// @return 
OPSTAT StackMachine::bwAnd() {
    int a = pop();
    int b = pop();
    tosr = a & b;
    return push(tosr);
}

/// @brief bitwise OR top two items on data stack
/// @return 
OPSTAT StackMachine::bwOr() {
    int a = pop();
    int b = pop();
    tosr = a | b;
    return push(tosr);
}

/// @brief bitwise OR top two items on data stack
/// @return 
OPSTAT StackMachine::bwXor() {
    int a = pop();
    int b = pop();
    tosr = a ^ b;
    return push(tosr);
}

/// @brief Places a copy of the top item of the stack on the top of the stack.
/// @return 
OPSTAT StackMachine::dup() {
    tosr = data[top-1];
    return push(tosr);
}

/// @brief Places a copy of the second item on the data stack at the top of the stack
/// @return 
OPSTAT StackMachine::over() {
    tosr = data[top-2];
    return push(tosr);
}

/// @brief switches order of first two items on data stack
/// @return 
OPSTAT StackMachine::swap() {
    int a = pop();
    int b = pop();
    push(a);
    push(b);
    return OP_SUCCESS;
}

/// @brief pushes data from random access memory address provied onto data stack
/// @param addr 
/// @return 
OPSTAT StackMachine::load() {
    int addr = pop();
    return push(randomAccessMemory[addr]);
}

/// @brief stores data on top of data stack into random access memory at address provied.
/// @param addr 
/// @return 
OPSTAT StackMachine::store() {
    int addr = pop();
    int info = pop();
    randomAccessMemory[addr] = info;
    return OP_SUCCESS;
}

/// @brief Jump if not zero - checks top value of stack, if NOT zero, jumps to memory address provided otherwise continues
/// @param addr 
/// @return 
OPSTAT StackMachine::jmpnz(int addr) {
    if (data[top-1] != 0) {
        push(programCounter);   //put current address on data stack
        pushr();               //move address from data stack to return stack
        programCounter = addr; //set  to jump point
        return OP_BRANCH;      //signal next instruction is result of branching.
    }
    tosr = data[top-1]; //Otherwise proceed as normal
    return pop();
}

/// Opposite behavior to above.
OPSTAT StackMachine::jmpz(int addr) {
    if (data[top-1] == 0) {
        push(programCounter);   //put current address on data stack
        pushr();               //move address from data stack to return stack
        programCounter = addr; //set  to jump point
        return OP_BRANCH;      //signal next instruction is result of branching.
    }
    tosr = data[top-1]; //Otherwise proceed as normal
    return pop();
}

/// @brief calls a subroutine at the address stored at top of data stack.
/// @return 
OPSTAT StackMachine::call() {
    push(programCounter);    //put programCounter on data stack.
    pushr();                //move programCounter from data stack to return stack;
    programCounter = pop(); //put new address from data stack into programCounter.
    return OP_BRANCH;       //signal the next instruction is the result of branching.
}

/// @brief return from sub routine. pushes address from return stack to data stack and pops from data stack to program counter
/// @return 
OPSTAT StackMachine::retf() {
    popr();
    programCounter = pop();
    return OP_BRANCH;
}

/// @brief converts string to int and pushes onto data stack
/// @param arg 
/// @return 
OPSTAT StackMachine::push(string arg) {
    data[top++] = atoi(arg.c_str());
    tosr = data[top-1];
    return OP_SUCCESS;
}
/// pushes data onto data stack
OPSTAT StackMachine::push(int arg) {
    data[top++] = arg;
    tosr = arg;
    return OP_SUCCESS;
}

/// @brief pops top most data off data stack and returns it
/// @return 
int StackMachine::pop() {
    tosr = data[--top];
    return tosr;
}

/// @brief pops data stack and pushes result to return stack
/// @return 
OPSTAT StackMachine::pushr() { 
    rets[rtop++] = pop(); 
    return OP_SUCCESS; 
}

/// @brief pops data of return stack and pushes result to data stack
/// @return 
OPSTAT StackMachine::popr() { 
    return push(rets[--rtop]); 
}

/// @brief Add top two values on data stack
/// @return 
OPSTAT StackMachine::add() {
    int a = pop();
    int b = pop();
    tosr = a + b; 
    push(tosr);
    return OP_SUCCESS;
}

/// @brief multiply top two values on data stack
/// @return 
OPSTAT StackMachine::mult() {
    int a = pop();
    int b = pop();
    tosr = a*b;
    push(tosr);
    return OP_SUCCESS;
}

/// @brief subtract value of second item on stack by top item on stack, saving result to top of stack
/// @return 
OPSTAT StackMachine::sub() {
    int a = pop();
    int b = pop();
    tosr = b - a;
    push(tosr);
    return OP_SUCCESS;
}

/// @brief divide value of second value on stack by top item on stack, saving result to top of stack
/// @return 
OPSTAT StackMachine::div() {
    int a = pop();
    int b = pop();
    tosr = b/a;
    push(tosr);
    return OP_SUCCESS;
}

/// @brief performs modulo on value of second value on stack by top item on stack, saving result to top of stack
/// @return 
OPSTAT StackMachine::mod() {
    int a = pop();
    int b = pop();
    tosr = b % a;
    push(tosr);
    return OP_SUCCESS;
}


//show top item on stack.
OPSTAT StackMachine::show() {
    cout<<"[TOP]"<<data[top-1]<<"\n";
    return OP_SUCCESS;
}

//branch table
int StackMachine::ALU(string op, string arg) {

    //data & return stack ops
    if (op == "push") {
        return push(arg);
    } else if (op == "pop") {
        return pop();
    } else if (op == "pushr") {
        return pushr();
    } else if (op == "popr") {
        return popr();
    } else if (op == "swap") {
        return swap();
    } else if (op == "dup") {
        return dup();
    } else if (op == "over") {
        return over();
    }

    //bitwise ops
    if (op == "AND") {
        return bwAnd();
    } else if (op == "OR") {
        return bwOr();
    } else if (op == "XOR") {
        return bwXor();
    }

    //Math
    if (op == "add") {
        return add();
    } else if (op == "mult") { 
        return mult();
    } else if (op == "sub") { 
        return sub();
    } else if (op == "div") { 
        return div();
    } else if (op == "show") { 
        return show();
    } else if (op == "mod") {
        return mod();
    }

    //memory operations and branching
    if (op == "load") {          //Load from memory
        return load();
    } else if (op == "store") {  //store in memory
        return store();
    } else if (op == "call") {  //call subroutine
        return call();
    } else if (op == "retf") {  //return from subroutine
        return retf();
    } else if (op == "jmpnz") {  //jump if not zero
        return jmpnz(atoi(arg.c_str()));
    } else if (op == "jmpz") {
        return jmpz(atoi(arg.c_str()));
    }

    return OP_ERROR;
}

void StackMachine::run(vector<string>& program) {
    string op, arg;
    int opCnt = 0;
    programCounter = 0; 
    while (programCounter < program.size()) {
        string instr = program[programCounter]; //Fetch the next instruction in the program.
        vector<string> tokens = smtokenizer(instr, ' ');  //break the expression
        op = tokens[0];                                   //down into operation and argument
        arg = (tokens.size() > 1) ? tokens[1]:"";
        //for debugging:
        cout<<"["<<programCounter<<"] "<<op<<" "<<arg<<"\n";
        //pass the instruction to the arithmetic and logic unit
        OPSTAT stat = ALU(op, arg);
        switch (stat) {
            case OP_SUCCESS:
                programCounter += 1;
                break;
            case OP_BRANCH:
                cout<<"["<<stat<<"] [BRANCHING TO: "<<programCounter<<"]\n";
                break;
            case OP_ERROR:
            default:
                cout<<"["<<stat<<"] ";
                cout<<"[LAST INSTRUCTION FAILED.]\n";
                programCounter += 1;
        } 
    }
}

vector<string> StackMachine::smtokenizer(string expr, char DELIM) {
    vector<string> tokens;
    int pos = 0, i = 0, si = 0; //position, index, starting index.
    while (i < expr.size()) {
        if (expr[i] == DELIM) {
            string subexpr = expr.substr(si, pos - si);
            tokens.push_back(subexpr);
            while (i < expr.size() && expr[i] == DELIM) {
                i++; pos++;
            }
            si = pos;
        } else {
            pos++; i++;
        }
    }
    string subexpr = expr.substr(si, pos - si);
    tokens.push_back(subexpr);
    return tokens;
}

#endif
