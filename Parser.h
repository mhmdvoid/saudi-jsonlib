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
    explicit Parser(char *src): curPtr(src), beg(src), line(0), col(0) {}
    
    
    
    
    // entry point.
    bool parseJsonDecl(); // init.
    
    // handlers.
    bool parseJsonString(char *keyIdnetifier);
    JsonNode *parseJsonValue();
    
    JsonStringNode *parseJsonStringValue();
    JsonObjectValue *parseJsonObject();
    BasicEntry *parseJsonArray(JsonArrayValue *parent);
    JsonBooleanNode *parseJsonBooleanLiteral();
    JsonNumberNode *parseJsonNumberLiteral();
    
private:
    char *curPtr; // current token.
    char *beg;// TODO: FILE BUFFER
    Line line;
    Column col;
};
}

#endif /* Parser_h */
