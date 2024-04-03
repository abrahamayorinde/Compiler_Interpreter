#include "Node.h"
#include "symbol_table.h"

class SymbolTableCreator
{
private:
    bool logging;
    ofstream* logfile;
    void symboltablelogger(string message);

public:
    /*"""term : factor ((MUL | DIV) factor)*"""*/
    int result;
    int symbolTableScope;
    string tablename;
    SymbolTable* currentSymbolTable = NULL;
    //int vardepth;
    
    SymbolTableCreator(SymbolTable* symbolTable, bool sl_enabled, string filenamed);
    ~SymbolTableCreator();
    void visit(Node* asTree);
    void printsymboltable(SymbolTable* tempSymbolTable);
    void printAllTables();
    SymbolTable* returnTable();
};
