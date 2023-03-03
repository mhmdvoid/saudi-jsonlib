//
//  main.cpp
//  JsonLib
//
//  Created by Mohammed Mohsen on 03/03/2023.
//

#include <iostream>
#include "Parser.h"
int main(int argc, const char * argv[]) {
    
    saudi_json::Parser parser {"{ \"foo\": truer"};
    parser.parseJsonDecl();
    return 0;
}
