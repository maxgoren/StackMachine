#ifndef SM_LEXER_HPP
#define SM_LEXER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include "smGrammarDefs.hpp"
#include "smSymbolTable.hpp"
using namespace std;


class smLexer {
    private:
        vector<string> source;
        int pos;
        char lookahead;
        int lineno;
        string buffer;
        smSymbolTable<string, Symbol> keywords;
        Symbol getToken();
        char nextchar();
        void error(string msg);
    public:
        smLexer(vector<string> src);
        smLexer();
        smLexer(const smLexer&);
        TokenList* lex();
};

smLexer::smLexer() {

}

smLexer::smLexer(vector<string> src) {
    source = src;
    lineno = 0;
    pos = 0;
    keywords.insert("begin", beginsym);
    keywords.insert("end", endsym);
    keywords.insert("procedure", procsym);
    keywords.insert("call", callsym);
    keywords.insert("if", ifsym);
    keywords.insert("do", dosym);
    keywords.insert("then", thensym);
    keywords.insert("while", whilesym);
    keywords.insert("var", varsym);
    keywords.insert("odd", oddsym);
    keywords.insert("const", constsym);
    cout<<"Symbol table Initialized.\n";
}

smLexer::smLexer(const smLexer& o) {
    source = o.source;
    pos = o.pos;
    buffer = o.buffer;
    lookahead = o.lookahead;
    lineno = o.lineno;
    keywords = o.keywords;
}
void smLexer::error(string msg) {
    cout<<msg;
    exit(0);
}

Symbol smLexer::getToken() {
    buffer.clear();
    if (lookahead == ' ') {
        return whitespace;
    } else if (lookahead == '~') {
        return eofsym;
    } else if (lookahead == '(') {
        buffer = "(";
        return lparen;
    } else if (lookahead == ')') {
        buffer = ")";
        return rparen;
    } else if (lookahead == '+') {
        buffer = "+";
        return add;
    } else if (lookahead == '-') {
        buffer = "-";
        return sub;
    } else if (lookahead == '*') {
        buffer = "*";
        return multiply;
    } else if (lookahead == ';') {
        buffer = ";";
        return semicolon;
    } else if (lookahead == ',') {
        buffer = ",";
        return comma;
    } else if (lookahead == '.') {
        buffer = ".";
        return period;
    } else if (lookahead == '=') {
        lookahead = nextchar();
        if (lookahead == '=') {
            buffer = "==";
            return equals;
        } else {
            string msg = "unknown symbol on line " + to_string(lineno) + " near '" + lookahead + "'";
            error(msg);
        }
    } else if (lookahead == ':') {
        lookahead = nextchar();
        if (lookahead == '=') {
            buffer = ":=";
            return assignsym;
        } else {
            string msg = "unknown symbol on line " + to_string(lineno) + " near '" + lookahead + "'";
            error(msg);
        }
    } else if (isalpha(lookahead)) {
        buffer.clear();
        while (1) {
            if (isalnum(lookahead)) {
                buffer.push_back(lookahead);
            } else {
                pos--;
                break;
            }
            lookahead = nextchar();
        }
        Symbol ret = keywords.lookup(buffer);
        if (ret == errsym) {
            return idsym;
        } else {
            return ret;
        }
    } else if (isdigit(lookahead)) {
        buffer.push_back(lookahead);
        while (1) {
            lookahead = nextchar();
            if (isdigit(lookahead)) {
                buffer.push_back(lookahead);
            } else {
                pos--;
                break;
            }
        }
        return number;
    } else {
        if (lookahead == '~')
            return eofsym;
        string msg = "Coult not determine token on line " + to_string(lineno) + " positon " + to_string(pos) + " near " + lookahead;
        error(msg);
    }
}

 char smLexer::nextchar() {
    if (pos < source[lineno].size()) {
        return source[lineno][pos++];
    } else if (lineno < source.size()) {
            lineno += 1;
            pos = 0;
            cout<<"Analyzing line: "<<lineno<<"\n";
            if (lineno == source.size()) {
                cout<<"Lexing Complete.\n";
                return '~';
            }
            return nextchar();
    }
}

//Lexer's Gonna Lex.
TokenList* smLexer::lex() {
    auto start = chrono::steady_clock::now();
    cout<<"Lexer Starting...\n";
    TokenList dummy("dummy", whitespace, 0);
    TokenList* list = &dummy;
    lookahead = 'a';
    while (lookahead != '~') {
        lookahead = nextchar();
        Symbol curr = getToken();
        if (curr != whitespace) {
            list->next = new TokenList(buffer, curr, lineno);
            list = list->next;
        }
    }
    auto end = chrono::steady_clock::now();
    auto timing = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout<<"Lexing Completed in: "<<timing.count()<<"ms.\n";
    return dummy.next;
}

#endif