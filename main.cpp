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
    
    
    saudi_json::Parser parser {"[   [     true, [false], {.........}, {}     ]        ]"};
    if (parser.parseJsonDecl())
        std::cout << "Error found\n";
    return 0;
}
