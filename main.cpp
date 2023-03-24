//
//  main.cpp
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//

#include <iostream>
#include "Parser.h"
int main(int argc, const char * argv[]) {
    /*
     
     */
//    char* json = "[\n"
//                        "\"foo\": true\n"
//    "]";
//
    
    
    // [    true,     [  true,[false, [false], false]           ]
    
    
    char *buffer;
    auto handler = fopen("fail7.json", "r");
    long string_size, read_size;
    if (handler) {
        // Seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        // go back to the start of the file
        rewind(handler);
        
        // Allocate a string that can hold it all
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
        
        // Read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, handler);
        
        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';
        
        if (string_size != read_size) {
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
            return 9;
        }
    
        saudi_json::Parser parser {"[\"Extra close\"]]"};// this should fail. but how?
    
        auto my_obarject = parser.parseJsonArray();

//         Always remember to close the file.
        fclose(handler);
    }
    
        saudi_json::Parser parser {"[nullable]"};// this should fail. but how?
    
    auto my_obarject = parser.parseJsonArray();
    return 0;
}
