//
//  symbol_table.h
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//

#ifndef activation_table_h
#define activation_table_h
#include "activation_node.h"
#include <list>
#define MAX 100
using namespace std;
  
//const int MAX = 100;

class ActivationTable 
{
    ActivationNode* head[MAX];
public:
    string tablename;
    int scopelevel;
    ActivationTable* parent;
    list <ActivationTable*> childTables;
    
    ActivationTable()
    {
        for (int i = 0; i < MAX; i++)
        {
            head[i] = NULL;
        }
        tablename = "global";
        scopelevel = 0;
        parent = NULL;
    }
    ActivationTable(string name, int scopelevl, ActivationTable* progenitor)
    {
        for (int i = 0; i < MAX; i++)
        {
            head[i] = NULL;
        }
        tablename = name;
        scopelevel = scopelevl;
        parent = progenitor;
    }

    int hashf(string id); // hash function
    bool insert(string id, string Type, string typeclass, int lineno);
    bool find(string id);
    bool deleteRecord(string id);
    bool modify(string id, string Type, int lineno);
    int getscopelevel();
    void setscopelevel(int newscopelevel);
    ActivationNode* getNode(string id);
    void print();
};

#endif /* activation_table_h */
