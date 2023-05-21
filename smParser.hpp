#ifndef SM_PARSER_HPP
#define SM_PARSER_HPP
#include <iostream>
#include "smGrammarDefs.hpp"
using namespace std;

class smParser {
    private:
        void factor();
        void term();
        void expr();
        void cond();
        void statement();
        void block();
        void program();
        bool match(Symbol t);
        bool expect(Symbol t);
        void error(string s);
        TokenList* _tokens;
        TokenList* _curr;
        void nextsym();
        Symbol lookahead;
    public:
        smParser(TokenList* tokens);
        void parse();
};

smParser::smParser(TokenList* tokens) {
    _tokens = tokens;
}

void smParser::nextsym() {
    if (_curr != nullptr && _curr->next != nullptr) {
        _curr = _curr->next;
        lookahead = _curr->token;
    }
}

void smParser::parse() {
    _curr = _tokens;
    lookahead = _curr->token;
    program();
}

void smParser::error(string s) {
    cout<<s<<"\n";
    cout<<"Continue? ";
    char yesno;
    cin>>yesno;
    if (yesno == 'y' || yesno == 'Y')
        return;
    else
        exit(0);
}

bool smParser::match(Symbol s) {
    if (lookahead == s) {
        cout<<"Matched: "<<tokennames[s]<<"\n";
        nextsym();
        cout<<"lookahed = "<<tokennames[_curr->token]<<"("<<_curr->text<<")\n";
        return true;
    }
    return false;
}

bool smParser::expect(Symbol s) {
    if (match(s))
        return true;
    string msg = "Unexpected Symbol on line " + to_string(_curr->lineno) + ": " + tokennames[s] + "(" + _curr->text + ")";
    error(msg);
    return false;
}

void smParser::factor() {
    if (match(idsym)) {
        ;
    } else if (match(number)) {
        ;
    } else if (match(lparen)) {
        expr();
        expect(rparen);
    } else {
        string msg = "factor: syntax error on line: " + to_string(_curr->lineno) + " near: " + _curr->text;
        error(msg);
        nextsym();
    }
}

void smParser::term() {
    factor();
    while (lookahead == multiply || lookahead == divide) {
        nextsym();
        factor();
    }
}

void smParser::expr() {
    if (lookahead == add || lookahead == sub)
        nextsym();
    term();
    while (lookahead == add || lookahead == sub) {
        nextsym();
        term();
    }
}

void smParser::cond() {
    if (match(oddsym)) {
        expr();
    } else {
        expr();
        switch (lookahead) {
            case equals:
            case neqsym:
            case ltsym:
            case gtsym:
            case ltesym:
            case gtesym:
                        nextsym();
                        expr();
                        break;
            default:
                string msg = "condition: syntax error on line: " + to_string(_curr->lineno) + " near: " + _curr->text;
                error(msg);
                nextsym();
        }
    }
}

void smParser::statement() {
    if (match(idsym)) {
        expect(assignsym);
        expr(); 
    } else if (match(callsym)) {
        expect(idsym);
    } else if (match(beginsym)) {
        cout<<"Matched begin...\n";
        do {
            if (lookahead == endsym)
                break;
            statement();
            cout<<"Matching semicolon...\n";
        } while (match(semicolon));
        puts("No more statements in block.. searching for end...");
        expect(endsym);
        if (lookahead == period) {
            cout<<"Done Parseing.\n";
            exit(0);
        } else if (lookahead == semicolon) {
           ;
        }
    } else if (match(ifsym)) {
        cond();
        expect(thensym);
        statement();
    } else if (match(whilesym)) {
        cond();
        expect(dosym);
        statement();
    } else {
        string msg = "statement: syntax error on line: " + to_string(_curr->lineno) + " near: " + _curr->text;
        error(msg);
        nextsym();
    }
}

void smParser::block() {
    if (match(constsym)) {
        do {
            expect(idsym);
            expect(equals);
            expect(idsym);
        } while (match(comma));
        expect(semicolon);
    }
    if (match(varsym)) {
        do {
            expect(idsym);
        } while (match(comma));
        expect(semicolon);
    }
    while (match(procsym)) {
        expect(idsym);
        expect(semicolon);
        block();
        expect(semicolon);
    }
    statement();
}

void smParser::program() {
    int ident;
    block();
    expect(period);
    cout<<"Done Parsing.\n";
}


#endif