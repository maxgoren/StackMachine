#ifndef SM_PARSER_HPP
#define SM_PARSER_HPP
#include <iostream>
#include "smGrammarDefs.hpp"
#include <chrono>
using namespace std;


class smParser {
    private:
        Symbol lookahead;
        TokenList* src;
        TokenList* _curr;
        void program();
        void block();
        void statement();
        void condition();
        void expression();
        void term();
        void factor();
        void nextsym();
        bool expect(Symbol);
        bool match(Symbol);
        bool symbolTracing;
    public:
        smParser();
        smParser(TokenList* _tokens, bool tracing);
        void parse(bool tracing);
};

smParser::smParser() {
    symbolTracing = false;
}

smParser::smParser(TokenList* _tokens, bool tracing) {
    symbolTracing = tracing;
    src = _tokens;
}
void smParser::nextsym() {
    if (_curr != nullptr && _curr->next != nullptr) {
        if (symbolTracing)
            cout<<"<["<<tokennames[lookahead]<<", "<<_curr->text<<"]>   -> ";
        _curr = _curr->next;
        lookahead = _curr->token;
        if (symbolTracing)
            cout<<"<["<<tokennames[lookahead]<<", "<<_curr->text<<"]>\n";
    }
}

bool smParser::match(Symbol t) {
    if (lookahead == t) {
        nextsym();
        return true;
    }
    return false;
}

bool smParser::expect(Symbol t) {
    if (match(t))
        return true;
    cout<<"Unexpect Symbol: "<<tokennames[t]<<"\n";
    return false;
}

void smParser::program() {
    block();
    if (match(period)) {
        cout<<"Perfect!\n";
    }
}

void smParser::block() {
    if (match(constsym)) {
        do {
            expect(idsym);
            expect(assignsym);
            expect(number);
        } while (match(comma));
        expect(semicolon);
    }
    if (match(varsym)) {
        expect(idsym);
        while (match(comma)) {
            expect(idsym);
        }
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


void smParser::statement() {
    if (match(idsym)) {
        expect(assignsym);
        expression();
    } else if (match(callsym)) {
        expect(idsym);
    } else if (match(beginsym)) {
        do {
            if (lookahead == endsym)
                break;
            statement();
        } while (match(semicolon));
        expect(endsym);
    } else if (match(ifsym)) {
        condition();
        expect(thensym);
        statement();
    } else if (match(whilesym)) {
        condition();
        expect(dosym);
        statement();
    } else {
        cout<<"statement: error on line: "<<_curr->lineno<<" near "<<_curr->text<<"\n";
        nextsym();
    }
}


void smParser::condition() {
    if (match(oddsym)) expression();
    else {
        expression();
        switch (lookahead) {
            case equals:
            case neqsym:
            case ltsym:
            case ltesym:
            case gtsym:
            case gtesym:
                        nextsym();
                        expression();
                        break;
            default:
                cout<<"condition(): unexpected symbol: "<<_curr->text<<" on line: "<<_curr->lineno<<"\n";
                nextsym();
                break;
        }
    }
}

void smParser::expression() {
   if (lookahead == add || lookahead == sub) {
        nextsym();
   }
    term();
    while (lookahead == add || lookahead == sub) {
        nextsym();
        term();
   }
}

void smParser::term() {
    factor();
    while (lookahead == multiply || lookahead == divide) {
        nextsym();
        factor();
    }
}

void smParser::factor() {
    if (match(idsym)) {
        []() {};
    } else if (match(number)) {
        []() {};
    } else if (match(lparen)) {
        expression();
        expect(rparen);
    } else {
        cout<<"factor(): syntax error on line"<<_curr->lineno<<" near "<<_curr->text<<"\n";
        nextsym();
    }
}

void smParser::parse(bool tracing = true) {
    auto start = chrono::steady_clock::now();
    cout<<"Parser Staring...\n";
    symbolTracing = tracing;
    _curr = src;
    lookahead = _curr->token;
    program();
    auto end = chrono::steady_clock::now();
    auto timing = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout<<"Parsing Completed in: "<<timing.count()<<"ms.\n";
}

#endif