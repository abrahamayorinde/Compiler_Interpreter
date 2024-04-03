//
//  Token.h
//  
//
//  Created by Abraham Ayorinde on 9/8/22.
//

#ifndef Token_h
#define Token_h
#include <iostream>
#include <string>
using namespace std;

class Token
{
public:
    string name;
    string Type;
    string Value;
    int line;
    int col;
};

#endif /* Token_h */
