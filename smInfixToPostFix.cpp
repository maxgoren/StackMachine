#include <iostream>
#include <fstream>
#include <ctype.h>
using namespace std;


class smToyCompiler {
        void term();
        void match(char);
        void error();
        void expr();
        char lookahead;
        string input;
        ofstream output;
        int pos;
        char getchar() {
            return input[pos++];
        }
    public:
        smToyCompiler() {
            pos = 0;
        }
        void compile(string in) {
            output.open("out.smc", ios::out);
            input = in;
            lookahead = getchar();
            expr();
            output<<"\n";
            output.close();
        }
};

void smToyCompiler::expr() {
    term();
    while (1) {
        if (lookahead == '+') {
            match('+'); term(); output<<"add\n";
        } else if (lookahead == '-') {
            match('-'); term(); output<<"sub\n";
        } else break;
    }
}

void smToyCompiler::term() {
    if (isdigit(lookahead)) {
        output<<"push "<<lookahead<<"\n";
        match(lookahead);
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
    smToyCompiler compiler;
    StackMachine sm(false, false);
    compiler.compile("9-3*7");
    return 0;
}

