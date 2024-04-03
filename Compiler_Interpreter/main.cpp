
#include <iostream>
#include <fstream>
#include <cstdio>

#include <cstring>
#include <sstream>
#include <list>

#include "miscellaneous.h"
#include "Node.h"
#include "symbol_table.h"
#include "parser.h"
#include "symboltablecreator.h"
#include "activation_record.h"
#include "activationrecordcreator.h"

#define LOWERCASESTART  97
#define LOWERCASEEND    122



using namespace std;

int main(int argc, char** argv)
{
    string toupper;
    string argument;
    string filenamed;
    string textfile;
    
    bool ll_enabled = false;
    bool pl_enabled = false;
    bool sl_enabled = false;
    bool il_enabled = false;

    string lloption = "ll";
    string ploption = "pl";
    string sloption = "sl";
    string iloption = "il";
    
    bool found = false;
    string test;
    for (int i = 0; i < argc; ++i)
    {
        cout<<"Argument #: "<< i <<" "<< argv[i]<<endl;
        argument = argv[i];

        if(argument == lloption) ll_enabled = true;
        if(argument == ploption) pl_enabled = true;
        if(argument == sloption) sl_enabled = true;
        if(argument == iloption) il_enabled = true;
        
        if(argument.size()>3)
        {
            if(argument.substr(argument.size() - 3) ==  "txt")
            {
                filenamed = argument;
                found = true;
            }
        }
    }
    
    if(found == true)
    {
        string textfile = exists_test0 (filenamed);
        
        SymbolTable* mySymbolTable = new SymbolTable;
        
        ActivationRecord* callrecordbook = new ActivationRecord;
        
        Lexer thisLexer3(ll_enabled,filenamed);
        
        thisLexer3.getInput(textfile);
        
        Parser parser(&thisLexer3, pl_enabled, filenamed);
        
        SymbolTableCreator createsymbolTable(mySymbolTable, sl_enabled, filenamed);
        
        ActivationRecordCreator createactivationRecord(callrecordbook, mySymbolTable, il_enabled, filenamed);
        
        Node* testNode;
        
        parser.startLexer(); //Extracts the relevant symbols from the input text.
        
        testNode = parser.program();
        
        createsymbolTable.visit(testNode); //Traverses the the abstract syntax tree argument starting at the root node.
        
        createsymbolTable.printAllTables();
        
        createactivationRecord.updateSymbolTable(createsymbolTable.returnTable());
        
        createactivationRecord.visit(testNode); //Traverses the abstract syntax tree to create a record of variable and function access
    }
    else
    {
        cout<<"Invalid file type extension: " + filenamed;
    }
    return 0;
}

