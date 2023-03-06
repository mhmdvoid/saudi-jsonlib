//
//  Parser.h
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//

#ifndef Parser_h
#define Parser_h
#include "Node.h"
namespace saudi_json {
class Parser {
    
    Parser(const Parser&);
    void operator=(const Parser&);
    
public:
    typedef unsigned long Line;
    typedef unsigned long Column;
    explicit Parser(char *src): curPtr(src) {};
    
    
    
    
    // entry point.
    bool parseJsonDecl(); // init.
    
    // handlers.
    bool parseJsonString(char *keyIdnetifier);
    JsonNode *parseJsonValue();
    
    JsonStringNode *parseJsonStringValue();
    void parseJsonObject();
    JsonNode *parseJsonArray();
    JsonBooleanNode *parseJsonBooleanLiteral();
    JsonNumberNode *parseJsonNumberLiteral();
    
private:
    char *curPtr; // current token.
    Line line;
    Column col;
};
}

#endif /* Parser_h */
