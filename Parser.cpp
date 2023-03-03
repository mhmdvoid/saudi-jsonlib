//
//  Parser.cpp
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//


#include "Parser.h"

using namespace saudi_json;

#define MAX_KEY 8192 // 8 K

namespace {

// move the global ptr if == token;
bool skip(char *&pointer, char token) {
    
    while (*pointer == ' ' || *pointer == '\t')
        ++pointer;
    auto mv = pointer;
    if (*mv == token) {
        ++pointer;
        return true;
    }
    return false;
    
}
bool validJsonKey(const char c) {
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'g': case 'h': case 'i': case 'j': case 'k':
        case 'l': case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I': case 'J': case 'K':
        case 'L': case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            return true;
            
        default:
            break;
    }
    return false;
}
bool lexIdentifier(char *beg, char *&ptr, char *keyContainer) {
    if (!validJsonKey(*beg))
        return true; // your json key string starts with an invalid state.
    unsigned defaultMax = 0;
    while (validJsonKey(*ptr) && defaultMax++ < 256)
        ++ptr;
    
    

    unsigned len = ptr-beg;
    // key is ptr to 256 bytes container on stack;
    for (unsigned i = 0; i != len; ++i)
        keyContainer[i] = *beg++;
    len++;
    keyContainer[len] = '\0';
    return false;
}

bool valueDigit(char c) {
    return c == '0' || c == '1' || c == '2' || c == '3' ||
           c == '4' || c == '5' || c == '6' || c == '7' ||
           c == '8' || c == '9';
}
}
bool Parser::parseJsonDecl() {
    // lexer method should be clearly on call;
    if (skip(curPtr, '{')) {
        // We could pass in instead of returing
        // optim, if key filled json correct.
        do {
            char *start = curPtr;
            char keyContainer[256]; // TODO: Move to identifier object.
            // parseJsonDecl allocates 256 bytes on stack
            
            
            
            if (parseJsonString(keyContainer) && !skip(curPtr, ':')) {
                return true; // FIXME: this propegates the erorr, failur
            }
            
            // call parse_object if no object starter found, then immediely exit. and tell where we stop
            if (parseJsonValue()) {
                return true;
            }
            
        } while(skip(curPtr, ','));
    }
    return false;
}

bool Parser::parseJsonString(char *keyContainer) {
    if (!skip(curPtr, '\"')) {
        return true; // json key start "
    }
    
    char *beg = curPtr;
    
    if (!lexIdentifier(beg, curPtr, keyContainer) && !skip(curPtr, '\"'))
        return false; // fail to lex identifer & ";
    
    return true;
}

bool Parser::parseJsonValue() {
    
    char *beg = curPtr;
    switch (*curPtr++) {
        case '\"':
            parseJsonString(nullptr);  //
            break;
        case '{':
            parseJsonObject();
            break;
        case '[':
            // array value
            parseJsonArray();
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            // number value
            parseJsonNumberLiteral();
            break;
        case 't':
            // possible true literal
            parseJsonBooleanLiteral();
            break;
        case 'f':
            // possible false literal
            parseJsonBooleanLiteral();
            break;
            
        default:
            break;
    }
    

    return true;
    
    
}

void Parser::parseJsonString()  {}
void Parser::parseJsonObject()  {}
void Parser::parseJsonArray()   {}
void Parser::parseJsonBooleanLiteral() {
    
    // parse like an identifer, then compare if its true, or false valid bool literal, otherwise invalid literal, and red flag.
    
}
void Parser::parseJsonNumberLiteral() {}
