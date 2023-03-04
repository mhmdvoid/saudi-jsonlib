//
//  Node.h
//  JsonLib
//
//  Created by Mohammed Mohsen on 04/03/2023.
//

#ifndef Node_h
#define Node_h
#include <string>
#include <vector>
#include <iostream>
namespace saudi_json {
class JsonPrimitiveNode;
class JsonNode;
class JsonBooleanNode;

class Location {
    unsigned line, col;
public:
    Location(): line(0), col(0) {}
    unsigned getLine() const     { return line; }
    unsigned getColumn() const   { return col; }
    void setLine(unsigned l)     { line = l; }
    void setColumn(unsigned c)   { col = c; }
    
};

class JsonEntry {
    
public:
    JsonEntry(char *buffer) {
        memcpy(key, buffer, 256);
    }
    char key[256];
    JsonNode *value;
    
};

class JsonRoot {
public:
    std::vector<JsonEntry *> entries;
    
    
  
    void printAll() const {
        for (auto & entry : entries) {
            std::cout << entry->key << std::endl;
//            if (JsonBooleanNode* v = dynamic_cast<JsonBooleanNode*>(entry->value)) {
//
//            }
        }
    }
    
};

class JsonNode {
    JsonNode(const JsonNode&);
    void operator=(const JsonNode&);
public:
    JsonNode() {}
};

class JsonPrimitiveNode: public JsonNode {
    
public:
    JsonPrimitiveNode(){}
    JsonPrimitiveNode(char *v): value(v) {}
    char *value;

};


class JsonStringNode: public JsonPrimitiveNode {
public:
    JsonStringNode(char *buff, unsigned long s) {  // TODO: Move this code to a dyn_string allocator.
        value = new char[s+1];
        memcpy(value, buff, s+1);
    }
    
};

class JsonNumberNode: public JsonPrimitiveNode {
    
public:
    explicit JsonNumberNode(char *buff, unsigned long s/*, Location &location*/) /*, loc(location)*/ {
        
        value = new char[s+1];
        memcpy(value, buff, s+1);
        
    }

private:
//    char *value; // could be static allocated. pre-defined value for max_number_range.
    bool isNegative;
//    Location &loc;
};

class JsonBooleanNode: public JsonPrimitiveNode {
    
    
public:
    
    JsonBooleanNode(char *v/*, Location &location*/): JsonPrimitiveNode(v)/*, loc(location)*/ {}
    void setValue(bool b) {
        is_true = b;
    }
    bool isTrue() const { return is_true; }
    bool isFalse() const { return !is_true; }
private:

    /*Location &loc;*/
    bool is_true;
};

}

#endif /* Node_h */
