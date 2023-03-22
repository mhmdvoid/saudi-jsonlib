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
    
    
    // Recursive.
    bool isStartOfValue();  // FIXME: 
    
    
    bool parsePrimaryValue(BasicEntry *node/*parent?*/);
    
    // single-value
    bool parseSingleValue(BasicEntry *subnode);
    
    JsonArrayValue *parseJsonArray();
    JsonObjectValue *parseJsonObject();

    BasicEntry *parseJsonArray(JsonArrayValue *parent);


    
    
    // Key
    bool parseJsonString(char *keyIdnetifier);
    
    
    // json values.
    JsonStringNode *parseJsonStringValue();

    JsonBooleanNode *parseJsonBooleanLiteral();
    JsonNumberNode *parseJsonNumberLiteral();
    char *curPtr; // current token.
private:
   
    char *beg;// TODO: FILE BUFFER
    Line line;
    Column col;
};
}

#endif /* Parser_h */
