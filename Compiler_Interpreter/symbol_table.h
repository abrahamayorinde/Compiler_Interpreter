//
//  symbol_table.h
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//

#ifndef symbol_table_h
#define symbol_table_h
#include "symbol_node.h"
#include <list>

#define MAX 100

using namespace std;

class SymbolTable 
{
    SymbolNode* head[MAX];
public:
    string tablename;
    int scopelevel;
    SymbolTable* parent;
    list <SymbolTable*> childTables;

    SymbolTable()
    {
        for (int i = 0; i < MAX; i++)
        {
            head[i] = NULL;
        }
        tablename = "global";
        scopelevel = 0;
        parent = NULL;
    }
    SymbolTable(string name, int scopelevl, SymbolTable* progenitor)
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
    bool insert(string id, string Type, int depth, string typeclass, int lineno, Node* treeLocation);
    bool find(string id);
    bool findfromtop(string id, SymbolTable* referenceTable);
    bool deleteRecord(string id);
    bool modify(string id, string Type, int lineno);
    int getscopelevel();
    void setscopelevel(int newscopelevel);
    SymbolNode* getNode(string id);
    vector <string> getparametersoftable(string table,int argCount,SymbolTable* referenceTable);
    void print();
};

#endif /* symbol_table_h */
