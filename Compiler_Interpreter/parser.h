#include "Node.h"
#include "Token.h"
#include "lexer.h"

class Parser
{
private:
    bool logging;
    ofstream logfile;

public:
    Lexer* thislexer = NULL;
    Token currentToken;
    Node astHead;
    Node* astPointer = NULL;
    string cursorTokenCharType;

    Parser(Lexer* lexer, bool pl_enabled, string filenamed);
    
    void error();
    
    void startLexer();
    
    char getcursorSymbol();
    
    void eat(string tokenType);
    
    Node* factor();
    
    Node* term();
    // pre: currentToken.Type exists
    // post:returns the root node of the abstract syntax token tree
     
    Node* expression();
    
    void empty();
    
    Node* variable();
    
    Node* assignmentStatement();
     
    Node* statement();
    
    list<Node*>* statementList();
    
    Node* compoundStatements();
    
    Node* type_spec();
    
    list<Node*>* variable_declaration();
    
    Node* declarations();
    
    list<Node*>* parameters();
    
    list<Node*>* getparameter();
    
    Node* procedureCall();
    
    Node* procedure();

    Node* block();
    
    Node* program();
};
