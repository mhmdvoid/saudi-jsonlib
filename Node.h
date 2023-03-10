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

// Shared root. which Root.getinstance.jsonArray.insert();
// We need this when constructing the root only;
// []

// but now the jsonArray is really

class JsonNode {
    JsonNode(const JsonNode&);
    void operator=(const JsonNode&);
public:
    JsonNode() {}
};

class BasicEntry {
public:
    JsonNode *value;
//    bool hasChild {};
//    JsonNode *nxt;
//    int len;
};


class ArrayEntry: public BasicEntry {
    // doesn't have a key.
    // just a value.
};
class JsonObjectEntry: public BasicEntry {
    
public:
    JsonObjectEntry(char *buffer) {
        memcpy(key, buffer, 256);
    }
    char key[256]; // if key is null then it's an array.
    
    
};


class JsonRoot {
    
public:
    JsonRoot() {};
    virtual bool insertEntry(BasicEntry *entry) {return false;}
};
class JsonObjectValue: public JsonNode, public JsonRoot {
    
    bool isRoot;
public:
    std::vector<JsonObjectEntry *> entries;
    

    bool insertEntry(BasicEntry *entry) {
        auto e = (JsonObjectEntry *) entry;
        entries.push_back(e);
        return true;
    }
    void printAll() const {
        for (auto & entry : entries) {
            std::cout << entry->key << std::endl;
//            if (JsonBooleanNode* v = dynamic_cast<JsonBooleanNode*>(entry->value)) {
//
//            }
        }
    }
};
class JsonArrayValue: public JsonNode, public JsonRoot  {
    bool isRoot;
    std::vector<JsonNode *> values;
public:
    JsonArrayValue() {}
    bool insertEntry(BasicEntry *entry) {
        auto v = entry->value;
        values.push_back(v);
        return true;
    }

    void insertNode(JsonNode *value) {
        values.push_back(value);
    }
};

// Holder;
class Root {
    
    Root(const Root&);
    void operator=(const Root&);
    
private:
    Root()= default;
    ~Root()= default;
public:
    
    enum class rootType {
        array,
        object
    };
    
    JsonRoot *root;  // base class -will be a pointer to a heap value object.
    // either a
    //  root should have an interface.
    // jsonEntery;  basic
    // object entey;
    // arrayEntry;
    // root.insert(jsonEntry);
    static Root& getInstance() {
        static Root instance;
        return instance;
    }
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
