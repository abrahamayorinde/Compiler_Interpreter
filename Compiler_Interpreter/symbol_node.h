//
//  symbol_node.h
//  
//
//  Created by Abraham Ayorinde on 9/9/22.
//

#ifndef symbol_node_h
#define symbol_node_h
#include <list>
#include "Node.h"

#include <string>
using namespace std;

struct SymbolNode{
public:
    string identifier;
    string type;
    string classification;
    int depth;
    int lineNo;
    list <string>arguments;
    Node* location;
    SymbolNode* next;

    SymbolNode()
    {
        this->identifier = "";
        this->type = "";
        this->lineNo = 0;
        next = NULL;
    }
    SymbolNode(string key, string type, int depth, string typeclass, int lineNo, Node* location)
    {
        this->identifier = key;
        this->type = type;
        this->depth = depth;
        this->lineNo = lineNo;
        this->classification = typeclass;
        this->location = location;
        next = NULL;
    }
};

#endif /* symbol_node_h */
