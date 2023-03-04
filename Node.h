//
//  Node.h
//  JsonLib
//
//  Created by Mohammed Mohsen on 04/03/2023.
//

#ifndef Node_h
#define Node_h
#include <string>
namespace saudi_json {

class Location {
    unsigned line, col;
public:
    Location(): line(0), col(0) {}
    unsigned getLine() const     { return line; }
    unsigned getColumn() const   { return col; }
    void setLine(unsigned l)     { line = l; }
    void setColumn(unsigned c)   { col = c; }
    
};

class Root {
    Root(const Root&);
    void operator=(const Root&);
public:
    Root() {}
};

class JsonNumberNode: public Root {
    
public:
    explicit JsonNumberNode(char *buff, unsigned long s/*, Location &location*/) /*, loc(location)*/ {
        
        value = new char[s+1];
        memcpy(value, buff, s+1);
        
    }

private:
    char *value; // could be static allocated. pre-defined value for max_number_range.
    bool isNegative;
//    Location &loc;
};

class JsonBooleanNode: public Root {
    
    
public:
    
    JsonBooleanNode(char *v/*, Location &location*/): value(v)/*, loc(location)*/ {}
    void setValue(bool b) {
        is_true = b;
    }
    bool isTrue() const { return is_true; }
    bool isFalse() const { return !is_true; }
private:
    char *value;
    /*Location &loc;*/
    bool is_true;
};

}

#endif /* Node_h */
