#include "activation_record.h"
#include "Node.h"
#include "symbol_table.h"
#include <cstring>
#include <string.h>

class ActivationRecordCreator
{
private:
    bool logging;
    ofstream* logfile;

public:
    int intValue;
    int nestCounter;
    int symbolTableScope;
    
    float floatValue;
    
    string result;
    string tablename;
    
    recordType newRecord;
    arguments temp_argumentList;
    
    char characters[MAX_SIZE];

    Node* resultNode;
    SymbolNode* returnNode;
    ActivationRecord* currentActivationRecord = NULL;
    SymbolTable* currentSymbolTable = NULL;
    void symboltablelogger(string message);
    Node* visit(Node* asTree);
    void updateSymbolTable(SymbolTable* newTable);
    void printActivationRecord();
    
    ActivationRecordCreator(ActivationRecord* activeRecord, SymbolTable* symbolTable, bool il_enabled, string filenamed);
};
