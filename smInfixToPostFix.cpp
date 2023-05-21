#include <iostream>
#include <vector>
#include "smStackMAchine.hpp"
using namespace std;


class smToyCompiler {
        void term();
        void factor();
        void expr();
        void match(char);
        void error();
        char lookahead;
        string input;
        vector<string> output;
        int pos;
        char getchar() {
            return input[pos++];
        }
    public:
        smToyCompiler() {
            pos = 0;
        }
        vector<string> compile(string in) {
            input = in;
            lookahead = getchar();
            expr();
            output.push_back("show");
            return output;
        }
};

void smToyCompiler::expr() {
    term();
    while (1) {
        if (lookahead == '+') {
            match('+'); term(); output.push_back("add");
        } else if (lookahead == '-') {
            match('-'); term(); output.push_back("sub");
        } else break;
    }
}

void smToyCompiler::term() {
    factor();
    while (1) {
        if (lookahead == '*') {
            match('*'); factor(); output.push_back("mult");
        } else if (lookahead == '/') {
            match('/'); factor(); output.push_back("div\n");
        } else break;
    }
}

void smToyCompiler::factor() {
    if (isdigit(lookahead)) {
        string cmd = "push ";
        while (isdigit(lookahead)) {
            cmd.push_back(lookahead);
            lookahead = getchar();
        }
        pos--;
        output.push_back(cmd);
        match(lookahead);
    } else if (lookahead == '(') {
        match('('); expr(); match(')');
    } else error();
}

void smToyCompiler::match(char t) {
    if (lookahead == t)
        lookahead = getchar();
    else error();
}

void smToyCompiler::error() {
    cout<<"syntax error.\n";
    exit(1);
}

int main() {
    string expression = "(19*3)-17+(6*2)";
    smToyCompiler compiler;
    StackMachine sm(false, false);
    vector<string> program = compiler.compile(expression);
    cout<<"Evaluting: "<<expression<<"\nAnswer: ";
    sm.run(program);
    return 0;
}

