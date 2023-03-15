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




bool Parser::parseJsonDecl() {
    
    JsonRoot *root = 0;
    bool isArray = false;
    char *slice = curPtr;
    if (skip(curPtr, '{') || skip(curPtr, '[')) {
        if (*slice == '[') {
            // now we
            root = new JsonArrayValue(); // null otherwise
            isArray = true;
            do {
                
                
                auto value = parseJsonArray((JsonArrayValue*)root);
                root->insertEntry(value);
                
            } while (skip(curPtr, ','));
            goto clean;;
        }
        do {
            char *start = curPtr;
            char keyContainer[256]; // TODO: Move to identifier object.
            // parseJsonDecl allocates 256 bytes on stack
            
            
            
            if (parseJsonString(keyContainer) && !skip(curPtr, ':')) {
                // FIXME: Memory leak.
                return true; // FIXME: this propegates the erorr, failur
            }
            JsonObjectEntry* entry = new JsonObjectEntry(keyContainer);
            
            
            JsonNode *node = parseJsonValue();
            
            // call parse_object if no object starter found, then immediely exit. and tell where we stop
            if (node == 0) {
                // FIXME: Memory leak.
                return true;
            }
            entry->value = node;
//            global_root.object_root->entries.push_back(entry);
        } while(skip(curPtr, ','));
    }
    clean:
    auto js = (JsonArrayValue*) root;
    js->printAll();
//    endTopLevelDecl:
//    if (isArray) { // FIXME: move to union pointer.
//        if (!skip(curPtr, ']')) {
//            return true;
//        }
//    } else {
//        if (!skip(curPtr, '}')) {
//            return true;
//        }
//    }
//    assert(*curPtr == '\0' && "json ends!");
//    global_root.object_root->printAll();
    return false;
}

bool Parser::parseJsonString(char *keyContainer) {
    if (!skip(curPtr, '\"'))
        return true; // json key start "
    
    char *beg = curPtr;
    
    if (!lexIdentifier(beg, curPtr, keyContainer) && !skip(curPtr, '\"'))
        return false; // fail to lex identifer & ";
    
    return true;
}

JsonNode *Parser::parseJsonValue() {
    skipWhitespace(curPtr);
    char *beg = curPtr;
    JsonNode *nodeValue = 0;
    switch (*curPtr++) {
        default:
            break;
        case '\"':
            nodeValue = parseJsonStringValue();  // FIXME: _
            break;
        case '{':
            parseJsonObject();
            break;
        case '[':
            // array value
//            auto parent = new JsonArrayValue();
            parseJsonArray(0);
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            // number value
            
            
            nodeValue = parseJsonNumberLiteral();
            // if node value is null, then error parsin;
            break;
        
        case 't': case 'f': // possible true/false literal
            
            nodeValue = parseJsonBooleanLiteral();
            break;
         
    }
    
    return nodeValue;
}
JsonStringNode *Parser::parseJsonStringValue()  {


    char *beg = curPtr-1;
    if (!skip(beg, '\"'))
        return 0;
    
    while (validJsonKey(*curPtr))
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

static std::vector<JsonArrayValue*> stack;
void Parser::parseJsonObject()  {}
BasicEntry *Parser::parseJsonArray(JsonArrayValue *parent)   {
    // parseJsonArray: if no nested structire found, is on demand parsing method.
    // one call one array basic_value
    // if find nested structure we loop for every nested structure.
    // this also true when we found a json_object {} which will be one call except that json is nested. which eventually we will be waiting.
    skipWhitespace(curPtr);
    char *beg = curPtr-1;

//    assert(*beg == '[' && "Json array doesn't start [ !!");
    
    int Depth = 1;
    // We're sure weather we're coming from a root_array or object_value.
    
    BasicEntry *entrySlot = new BasicEntry(); // This is actually outter value. depth 1;
//    Expecting 'STRING', 'NUMBER', 'NULL', 'TRUE', 'FALSE', '{', '[', ']'
        skipWhitespace(curPtr);
        switch (*curPtr++) {
            default:
//                goto error;
                break;
//
            case '\"': // string value;
                entrySlot->value = parseJsonStringValue();

                return entrySlot;
            case 't': case 'f':
                
                entrySlot->value = parseJsonBooleanLiteral();
                return entrySlot;

            case '{':
                break; // Json object;
            case '[':
                
                start:
                stack.push_back(parent);
                
                auto current = new JsonArrayValue();
                
                parent->insertNode(current);
//                current->prev = parent; // later.
                loop:
                do {
                    skipWhitespace(curPtr);
                    switch (*curPtr++) {
                        case ',':
                            // we're already ahead of ourself by one;
                            --curPtr;
                            break;
                        case 't': case 'f':
                            current->insertNode(parseJsonBooleanLiteral());
                            break;
                        case '[':
                            parent = current;
                            goto start;
                            
                        default:
                            char *possible_bracket = curPtr-1;
                            if (*possible_bracket == ']') --curPtr;
                            break;
                    }
                    if (skip(curPtr, ']')) {
                        //
                        if (stack.size() != 0) {
                            parent = stack.back(); // Access last_element.
                            stack.pop_back();
                            current = parent;
                            goto loop;
                        }
                    }
                } while(skip(curPtr, ','));
                
                break;
        }
    
  
//    parent;
    for (int i = 0; i < parent->getList().size(); ++i) {
        auto cur = parent->getList()[i];
        cur->getString();
    }
//
//    if (!skip(curPtr, ']')) { // check outer.
//        std::cout << "[Syntax error] For depth 1 outter array type\n";
//        // TODO: To return 0, or to skip ahead?
//    }
    return entrySlot;
}

// null is retunred if fail to parse json literal.
JsonBooleanNode *Parser::parseJsonBooleanLiteral() {
    char* beg = curPtr-1;
    if (!validJsonKey(*beg))
        return 0;
    
    while (validJsonKey(*curPtr))
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
