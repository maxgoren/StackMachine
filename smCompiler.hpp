#ifndef SMCOMPILER_HPP
#define SMCOMPILER_HPP
#include <iostream>
#include <vector>
#include <chrono>
#include "smGrammarDefs.hpp"
#include "smParser.hpp"
#include "smLexer.hpp"
using namespace std;
class smCompiler {
        smLexer* lexer;
        TokenList* tokenList;
        smParser* parser;
        TokenList* _tokens;
        vector<string> src;
    public:
        smCompiler(vector<string> source) {
            src = source;
        }
        void compile(bool tracing);
};

void smCompiler::compile(bool tracing = false) {
        lexer = new smLexer(src);
        TokenList* tokenList = lexer->lex();
        parser = new smParser(tokenList, tracing);
        parser->parse(tracing);
}

#endif