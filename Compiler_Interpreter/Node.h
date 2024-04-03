//
//  Node.h
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//

#ifndef Node_h
#define Node_h
#include "Token.h"
#include <list>
#include <string>
struct Node
{
public:
    Token myNodeToken;
    Node* left;
    Node* right;
    Node* child;
    list<Node*>* compoundStatements;
    list<list<Node*>*>* variableDeclarations;
    list<Node*>* parameterlist;
    //const static Token keywords[] = {Token("PROGRAM"), Token(" VAR"), "INTEGER_DIV", "INTEGER", "REAL", "BEGIN", "END"};
    Node()
    {
        myNodeToken.Type = "";
        myNodeToken.Value = "";
        left = NULL;
        right = NULL;
        child = NULL;
    }
};

class unaryNode: public Node
{
public:
    void addNode(Node* aNode, string nodeType)
    {
        
    }
};

#endif
