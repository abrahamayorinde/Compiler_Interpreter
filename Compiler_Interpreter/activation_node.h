//
//  symbol_node.h
//  
//
//  Created by Abraham Ayorinde on 9/9/22.
//

#ifndef activation_node_h
#define activation_node_h
#include <list>
#include <string>

using namespace std;


struct ActivationNode{
public:
    string identifier;
    string type;
    string classification;
    int lineNo;
    list <string>arguments;
    ActivationNode* next;

    ActivationNode()
    {
        this->identifier = "";
        this->type = "";
        this->lineNo = 0;
        this->classification = "";
        next = NULL;
    }
    ActivationNode(string key, string type, string typeclass, int lineNo)
    {
        this->identifier = key;
        this->type = type;
        this->lineNo = lineNo;
        this->classification = typeclass;
        next = NULL;
    }
};

#endif /* activation_node_h */
