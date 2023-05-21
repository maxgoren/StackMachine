#ifndef SM_GRAMMAR_DEFS_HPP
#define SM_GRAMMAR_DEFS_HPP
#include <string>
#include <vector>
using std::vector;
using std::string;

vector<string> tokennames = { "lparen", "rparen", "add", "sub", "multiply", "divide", "semicolon", "comma", "period",
                             "equals", "assignsym", "beginsym", "endsym", "procsym", "callsym",  "whilesym", "idsym", "ifsym", "dosym",
                             "thensym", "number", "errsym", "whitespace", "varsym", "eofsym", "oddsym", "neqsym" "ltsym", "gtsym", "gtesym", "ltesym", "constsym"};

typedef enum syms {
    lparen,rparen,add,sub,multiply,divide,semicolon,comma,period,equals,assignsym,beginsym,endsym,
    procsym,callsym,whilesym,idsym,ifsym, dosym, thensym, number,
    errsym,whitespace,varsym, eofsym, oddsym, neqsym, ltsym, gtsym, gtesym, ltesym, constsym
} Symbol;

struct TokenList {
    string text;
    Symbol token;
    int lineno;
    TokenList* next;
    TokenList(string t, Symbol s, int ln) {
        text = t;
        token = s;
        lineno = ln;
        next = nullptr;
    }
};

#endif