//
//  main.cpp
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//

#include <iostream>
#include "Parser.h"
int main(int argc, const char * argv[]) {
    
    saudi_json::Parser parser {"{ \"foo\": true, \"valid\": false, \"employees\": 2343}"};
    if (parser.parseJsonDecl())
        std::cout << "Error found\n";
    return 0;
}
