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
    char* json = "[\n"
                        "\"foo\": true\n"
    "]";
                        
    saudi_json::Parser parser {json};
    if (parser.parseJsonDecl())
        std::cout << "Error found\n";
    return 0;
}
