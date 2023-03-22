//
//  Parser.cpp
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//


#include "Parser.h"

#include <string>
using namespace saudi_json;

#define MAX_KEY 8192 // 8 K
#define DEFAULT_STRING_VALUE MAX_KEY
#define MAX_STRING_VALUE 5368709121  // 5 GB
/*
 
 foo: {
    fsdf: "SDF,
    fo: {
 
 }
 }
 */
namespace {


void skipWhitespace(char *&pointer) {
    while (*pointer == ' ' || *pointer == '\t' || *pointer == '\n')
        ++pointer;
}

bool match(char *&ptr, char token) {
    skipWhitespace(ptr);
    if (*ptr == token) {
        return true;
    }
    return false;
    
}
// move the global ptr if == token;
bool skip(char *&pointer, char token) {
    skipWhitespace(pointer);
    auto mv = pointer;
    if (*mv == token) {
        ++pointer;
        return true;
    }
    return false;
    
}
void skipUntil(char *&curPtr, char tkn) {
    while (*curPtr != '\0' && *curPtr != tkn) {
        skip(curPtr, tkn);
    }
}


bool validateJsonString(const char c) {
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
        case ' ': case '\t': case '\\':
            return true;
            
        default:
            break;
    }
    return false;
}
bool lexIdentifier(char *beg, char *&ptr, char *keyContainer) {
    if (!validateJsonString(*beg))
        return true; // your json key string starts with an invalid state.
    unsigned defaultMax = 0;
    while (validateJsonString(*ptr) && defaultMax++ < 256)
        ++ptr;
    
    

    unsigned len = ptr-beg;
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

} // end anonymous namesapce


JsonObjectValue *Parser::parseJsonObject() {
    if (!match(curPtr, '{')) {
        std::cout << "Your json doesn't start with [ opening array root\n";
        return 0;  // fail
    }
    auto entry_root = new BasicEntry();

    bool parseResult = parsePrimaryValue(entry_root);
    
    if (entry_root->value == 0/* || cast(value) != arrayValue*/ || parseResult) {
        std::cout << "A top level parsing error has happend\n";
    }
    
    return (JsonObjectValue *)entry_root->value;
}

JsonArrayValue *Parser::parseJsonArray() {
    
    
    if (!match(curPtr, '[')) {
        std::cout << "Your json doesn't start with [ opening array root\n";
        return 0;  // fail
    }
    auto entry_root = new BasicEntry();

    
    bool parseResult = parsePrimaryValue(entry_root);
    
    if (entry_root->value == 0/* || cast(value) != arrayValue*/ || parseResult) {
        std::cout << "A top level parsing error has happend\n";
    }
    // TODO: if not eof, expecting eof or something
    return (JsonArrayValue *)entry_root->value;
}


bool Parser::isStartOfValue() { return false; } // TODO:

// single-value.
bool Parser::parseSingleValue(BasicEntry *subNode) {
    if (parsePrimaryValue(subNode)) {
        std::cout << "Error\n";
        return true; // fail
    }
    return false;
}


bool Parser::parsePrimaryValue(BasicEntry *node) {
    skipWhitespace(curPtr);
    switch (*curPtr) {
        default: // Handle unknown cases.
            return true;
//            break;
            
        case '\"':
            curPtr++;
            node->value = parseJsonStringValue();
            break;
        case 't': case 'f':
            curPtr++;
            node->value = parseJsonBooleanLiteral();
            break;
            
        case '{': { // main loop for array
            auto leftLoc = beg - curPtr;
            skip(curPtr, '{');

            auto nestedObject = new JsonObjectValue(); // malloc.
            
            node->value = nestedObject; // move the heap to owning pointer.
            bool foundError = false;
            if (!match(curPtr, '}')) {
                
                
                // We parse key then : then a value for once. then do ,
                char *start = curPtr;
                char keyContainer[256]; // TODO: Move to identifier object.
                
                
                
                // parsing the key
                if (parseJsonString(keyContainer) && !skip(curPtr, ':')) {
                    // FIXME: Memory leak.
                    std::cout << "Expect json_key followed by `:`\n";
                    return true; // FIXME: this propegates the erorr, failur
                }
                JsonObjectEntry* entry = new JsonObjectEntry(keyContainer); // sibling.
                //
                if (parseSingleValue(entry/*, expected value in []*/)) {
                    std::cout << "Expect value error\n";
                    
                    return true; // fail yes.
                }
                nestedObject->insertEntry(entry);
                while (match(curPtr, ',')) {
                    curPtr++;
                    skipWhitespace(curPtr);
                    
                    
                    // TODO: What is this?!
                    char *start = curPtr;
                    char keyContainer[256]; // TODO: Move to identifier object.
                    
                    
                    
                    // parsing the key
                    if (parseJsonString(keyContainer) && !skip(curPtr, ':')) {
                        // FIXME: Memory leak.
                        std::cout << "Expect json_key followed by `:`\n. Trailing comma";
                        return true; // FIXME: this propegates the erorr, failur
                    }
                    entry = new JsonObjectEntry(keyContainer); // sibling.
//                    entry->value = 0;
                    if (parseSingleValue(entry)) {
                        std::cout << "missing value!\n";
                        
                        return true; // missing value after key.
                    }
                    nestedObject->insertEntry(entry);
                }
            }
            
            if (!skip(curPtr, '}')) {
                std::cout << "expected '}' in json expression\n";
                return true;
            }
            
            break;
        }
        case '[': // main loop for array
            
            auto leftLoc = beg - curPtr;
            skip(curPtr, '[');
            auto nestedArray = new JsonArrayValue(); // malloc.
            node->value = nestedArray; // move the heap to owning pointer.
            bool foundError = false;
            if (!match(curPtr, ']')) {
                // todo: jsonNode;
                auto js = new BasicEntry(); // malloc sibling entry.
                // into a mainstream one.
                
                if (parseSingleValue(js/*, expected value in []*/)) {
                    std::cout << "Expect value error\n";

                    return true; // fail yes.
                }
                nestedArray->insertNode(js->value);
                
                while (match(curPtr, ',')) {
                    ++curPtr;
                    js->value = 0;
                    if (parseSingleValue(js)) {
                        std::cout << "Trailing comma error\n";

                        return true; // trailing_comma.
                    }
                    nestedArray->insertNode(js->value);
                }
            }
            
            if (!skip(curPtr, ']')) {
                std::cout << "expected ']' in json expression\n";
                return true;
            }
            break;
        
    }
    return false;  // fail no
}

bool Parser::parseJsonString(char *keyContainer) {
    if (!skip(curPtr, '\"'))
        return true; // json key start "
    
    char *beg = curPtr;
    
    if (!lexIdentifier(beg, curPtr, keyContainer) && !skip(curPtr, '\"'))
        return false; // fail to lex identifer & ";
    
    return true;
}


JsonStringNode *Parser::parseJsonStringValue()  {


    char *beg = curPtr-1;
    if (!skip(beg, '\"'))
        return 0;
    
    while (validateJsonString(*curPtr))
        ++curPtr;
    
    assert(*curPtr == '\"' && "doesn't escape \" char!");
    // FIXME: if size > default: allocate dyn 5 GB!
    char stringValue[DEFAULT_STRING_VALUE]; // 8k on stack.
    unsigned long valueSize = curPtr++ - beg;
    unsigned i = 0;
    while (i != valueSize) {
        stringValue[i] = *beg++;
        ++i;
    }
    stringValue[i] = '\0';
    
    JsonStringNode *node = new JsonStringNode(stringValue, valueSize);
    return node;
    
}
// null is retunred if fail to parse json literal.
JsonBooleanNode *Parser::parseJsonBooleanLiteral() {
    char* beg = curPtr-1;
    if (!validateJsonString(*beg))
        return 0;
    
    while (validateJsonString(*curPtr))
        ++curPtr;
    
    JsonBooleanNode *binResult = 0;
    if (strncmp(beg, "true", curPtr-beg) == 0) {
        binResult = new JsonBooleanNode("true");
        binResult->setValue(true);
    }
    else if (strncmp(beg, "false", curPtr-beg) == 0) {
        binResult = new JsonBooleanNode("false");
        binResult->setValue(false);
    }
    
    return binResult;
}

JsonNumberNode *Parser::parseJsonNumberLiteral() {
    char *beg = curPtr-1;
    if (!valueDigit(*beg))
        return 0;
    
    while (valueDigit(*curPtr))
        ++curPtr;
    
    JsonNumberNode *numberResult = 0;
    unsigned long size = curPtr-beg;
    
    char numberBuffer[size+1]; // Data structure can't be copied auto for you.
    unsigned i = 0;
    for (; i < size; ++i)
        numberBuffer[i] = *beg++;
    
    numberBuffer[i] = '\0';
    
    numberResult = new JsonNumberNode(numberBuffer, size);
    
    
    return numberResult;
}
