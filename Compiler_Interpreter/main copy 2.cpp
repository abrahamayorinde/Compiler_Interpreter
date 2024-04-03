
#include <iostream>
#include <fstream>
//#include <stdio.h>
//#include <string.h>
//#include <algorithm>
//#include <cctype>
#include <cstring>
#include <sstream>
#include <list>

#include "Node.h"
#include "symbol_table.h"
//#include "Parser.h"

#define LOWERCASESTART  97
#define LOWERCASEEND    122
using namespace std;

bool isdigit(char digit);
bool nearEnd(int place, int end);
string toSymbol(string word);
void uppercase(string &word);


class Lexer{
public:
    string inputText;
    char inputChar;
    int place;
    int end;
    Token thisToken;
    int tokencount;
    int linenum;
    string program;
    SymbolTable* aSymbolTable;
    
    void getInput(string text)
    {
        inputText = text;
        
        place = 0;
        end = (int)inputText.size()-1;
        cout<<"The length of the text file: "<<end<<endl;
        tokencount = 0;
        linenum = 1;
    }
    
    Lexer (SymbolTable* thisSymbolTable)
    {
        aSymbolTable = thisSymbolTable;
    }
    
    char currentSymbol()
    {
        if(place<end)
        {
            return inputText.at(place);
        }
        else
        {
            if (end == 0)
            {
                return NULL;
            }
            else
            {
                return inputText.at(end-1);
            };
        }
    }
    
    Token getToken()
    {
        thisToken.Value = 0;
        thisToken.Type = "NULL";
        string literalValue = "";
        string truecopy = "";
        cout<<endl;
        cout<<"getToken: place: "<<place<<endl;
        //if(place < end)
        while(place<end)
        {
            cout<<"getToken: count: "<<tokencount<<endl;
            
            if (inputText.at(place) == '{')
            {
                while(inputText.at(place) != '}')
                {
                    place++;
                }
                place++;
            }
            if(place>=end)
            {
                goto labelout;
            }
            while(isspace(inputText.at(place)) && place<end)
            {
                if(nearEnd(place,end))
                {
                    place++;
                    goto labelout;
                }
                else
                {
                    place++;
                }
            }
            if(place>=end)
            {
                goto labelout;
            }
            if( isdigit(inputText.at(place)) and (place<end) )
            {
                while(isdigit(inputText.at(place)) and (place<end) )
                {
                    literalValue.append(1,inputText.at(place));
                    thisToken.Type = "INTEGER";
                    thisToken.Value = stoi(literalValue);
                    if( place+1 >= end)
                    {
                        cout<<"getToken: value at end: "<<thisToken.Value<<endl;
                        place++;
                        goto labelout;
                    }
                    //thisToken.Value = stoi(literalValue);
                    place++;
                }
                
                if( (inputText.at(place) == '.') and (place<end) )
                {
                    literalValue.append(1,inputText.at(place));
                    place++;
                    
                    while( isdigit(inputText.at(place)) and (place<end) )
                    {
                        cout<<"Here is the value just following the decimal point: "<<inputText.at(place)<<endl;
                        literalValue.append(1,inputText.at(place));
                        place++;
                    }
                    cout<<"this is the literal value"<<literalValue<<endl;
                    thisToken.Type = "FLOAT";
                    thisToken.Value = stof(literalValue);
                    cout<<"Float captured: "<<thisToken.Value<<endl;
                    goto labelout;
                }
                
                goto labelout;
            }
            if( (isalpha(inputText.at(place)) or (inputText.at(place) == '_')) and (place<end) )
            {
                literalValue.append(1,inputText.at(place));
                place++;
                thisToken.Value = 0;
                while((isdigit(inputText.at(place)) or isalpha(inputText.at(place))) and (place<end) )
                {
                    literalValue.append(1,inputText.at(place));
                    place++;
                }
                truecopy = literalValue;
                uppercase(literalValue);
                if(literalValue == "BEGIN")
                {
                    thisToken.Type = "BEGIN";
                }
                else if(literalValue == "END")
                {
                    thisToken.Type = "END";
                }
                else if(literalValue == "PROGRAM")
                {
                    thisToken.Type = "PROGRAM";
                }
                else if(literalValue == "INTEGER")
                {
                    thisToken.Type = "INTEGER_T";
                }
                else if(literalValue == "REAL")
                {
                    thisToken.Type = "REAL_T";
                }
                else if(literalValue == "VAR")
                {
                    thisToken.Type = "VAR";
                }
                else if( literalValue == "PROCEDURE")
                {
                    thisToken.Type = "PROCEDURE";
                }
                else if((literalValue == "DIV"))
                {
                    thisToken.Type = "INTEGER_DIVIDE";
                }
                else
                {
                    thisToken.Type = "ID"+truecopy;
                }
                cout<<"getToken: value before end: "<<thisToken.Value<<endl;
                goto labelout;
            }
            /*
             else if(strchr(inputText.substr(place,2),'\n'))
             {
             linenum++;
             cout<<"Line Number: "<<linenum<<endl;
             }
             */
            else if(inputText.substr(place,2) == ":=")
            {
                cout<<"Semi getToken: "<<inputText.substr(place,2)  <<endl;
                thisToken.Type = "ASSIGN";
                place+=2;
                goto labelout;
            }
            else if(inputText.substr(place,2) == "==")
            {
                thisToken.Type = "EQUALITY";
                place+=2;
                goto labelout;
            }
            else if(inputText.substr(place,2) == "<=")
            {
                thisToken.Type = "LESSTHANEQ";
                place+=2;
                goto labelout;
            }
            else if(inputText.substr(place,2) == ">=")
            {
                thisToken.Type = "GREATERTHANEQ";
                place+=2;
                goto labelout;
            }
            else if(inputText.substr(place,2) == "!=")
            {
                thisToken.Type = "NEQ";
                place+=2;
                goto labelout;
            }
            else if(inputText.at(place) == '+')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "ADD";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == '-')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "SUBTRACT";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == '*')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "MULTIPLY";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == '/')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "FLOAT_DIVIDE";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == '(')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "LPARENTH";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == ')')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "RPARENTH";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == ',')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "COMMA";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == '.')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "DOT";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == ':')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "COLON";
                place++;
                goto labelout;
            }
            else if(inputText.at(place) == ';')
            {
                cout<<"getToken: "<<inputText.at(place)  <<endl;
                thisToken.Type = "SEMI";
                place++;
                goto labelout;
            }
        }
        labelout:
        tokencount++;
        return thisToken;
    }
};

class Parser
{
public:
    //"""term : factor ((MUL | DIV) factor)*"""

    Lexer* thislexer;
    Token currentToken;
    Node astHead;
    Node* astPointer;
    string cursorTokenCharType;
    SymbolTable* symbols;
    int i;

    Parser(Lexer* lexer):thislexer(lexer)
    {}

    void error()
    {
        //do nothing
    }
    
    void startLexer()
    {
        currentToken = thislexer->getToken();
        cursorTokenCharType = getcursorSymbol();
    }
    
    char getcursorSymbol()
    {
        return thislexer->currentSymbol();
    }
    
    void eat(string tokenType)
    {
            if((currentToken.Type == tokenType))
            {
                currentToken = thislexer->getToken();
                cout<<"\nPARSER: JUST EATEN TOKEN : "<<currentToken.Type<<endl;
            }
            else if(currentToken.Type.substr(0,2) == tokenType)
            {
                currentToken = thislexer->getToken();
                cout<<"\nPARSER: JUST EATEN 'ID' TOKEN : "<<currentToken.Type<<endl;
            }
            else
            {
                cout<<":Mismatch Error- expected:"<<tokenType<<"<> actual: "<<currentToken.Type<<endl;
            }
    }
    
    void traverse(Node* astTreePointer)
    {
        if((astTreePointer->myNodeToken.Type == "UNARY +") || (astTreePointer->myNodeToken.Type == "UNARY -"))
        {
            cout<<"TRAVERSE - Type: "<<astTreePointer->myNodeToken.Type<<" TRAVERSE - Value: "<<astTreePointer->myNodeToken.Value<<endl;
            traverse(astTreePointer->child);
        }
        else
        {
            if (astTreePointer->left != NULL)
            {
                traverse(astTreePointer->left);
            }
         
            cout<<"TRAVERSE - Type: "<<astTreePointer->myNodeToken.Type<<endl;
            cout<<"TRAVERSE - Value: "<<astTreePointer->myNodeToken.Value<<endl;
         
            if (astTreePointer->right != NULL)
            {
                traverse(astTreePointer->right);
            }
        }
    }
    
    Node* factor()
    {
        Node* astNode;
        Node* newNode;
        
        Token current = currentToken;
        
        cout<<"PARSER: Factor Token Type: "<<currentToken.Type<<endl;
        
        if( currentToken.Type == "ADD" )
        {
            newNode = new unaryNode;
            eat("ADD");
            newNode->myNodeToken.Value = 0;
            newNode->myNodeToken.Type = "UNARY +";
            newNode->child = factor();
            astNode = newNode;
            return newNode;
        }
        if( currentToken.Type == "SUBTRACT")
        {
            newNode = new unaryNode;
            eat("SUBTRACT");
            newNode->myNodeToken.Value = 0;
            newNode->myNodeToken.Type = "UNARY -";
            newNode->child = factor();
            astNode = newNode;
            return newNode;
        }
        if( currentToken.Type == "INTEGER" )
        {
            //
            cout<<"PARSER: token #:"<<i+1<<endl;
            cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
            cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
            i++;
            //
            astNode = new Node;
            astNode->myNodeToken.Value = currentToken.Value;
            astNode->myNodeToken.Type = currentToken.Type;
            astNode->left = NULL;
            astNode->right = NULL;
            eat("INTEGER");

            return astNode;
        }
        if( currentToken.Type == "FLOAT" )
        {
            //
            cout<<"PARSER: token #:"<<i+1<<endl;
            cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
            cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
            i++;
            //
            astNode = new Node;
            astNode->myNodeToken.Value = currentToken.Value;
            astNode->myNodeToken.Type = currentToken.Type;
            astNode->left = NULL;
            astNode->right = NULL;
            eat("FLOAT");

            return astNode;
        }
        if( currentToken.Type == "LPARENTH")
        {
            //
            cout<<"PARSER: token #:"<<i+1<<endl;
            cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
            cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
            cout<<endl;
            i++;
            //
            eat("LPARENTH");
            astNode = expression();
            eat("RPARENTH");
            return astNode;
        }
        
        else //if((currentToken.Type.substr(0,3) == "VAR"))
        {
            i++;
            astNode = variable();
            return astNode;
        }
        
        return astNode;
    }
    
    Node* term( )
    {
        Node* leftNode;
        Node* newNode;
        bool change = false;
        Token current = currentToken;
        
        leftNode = factor();
        
        cout<<"PARSER: Term Token Type: "<<currentToken.Type<<endl;

        while( (currentToken.Type == "MULTIPLY") || (currentToken.Type == "FLOAT_DIVIDE") || (currentToken.Type == "INTEGER_DIVIDE") )
        {
            change = true;
            if(currentToken.Type == "MULTIPLY")
            {
                //
                cout<<"PARSER: token #:"<<i+1<<endl;
                cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
                cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
                i++;
                //
                newNode = new Node;
                newNode->myNodeToken.Type = "MULTIPLY";
                newNode->left = leftNode;
                eat("MULTIPLY");
                newNode->right = factor();
                leftNode = newNode;
            }
            if(currentToken.Type == "FLOAT_DIVIDE")
            {
                //
                cout<<"PARSER: token #:"<<i+1<<endl;
                cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
                cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
                i++;
                //
                newNode = new Node;
                newNode->myNodeToken.Type = "FLOAT_DIVIDE";
                newNode->left = leftNode;
                eat("FLOAT_DIVIDE");
                newNode->right = factor();
                leftNode = newNode;
            }
            if(currentToken.Type == "INTEGER_DIVIDE")
            {
                //
                cout<<"PARSER: token #:"<<i+1<<endl;
                cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
                cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
                i++;
                //
                newNode = new Node;
                newNode->myNodeToken.Type = "INTEGER_DIVIDE";
                newNode->left = leftNode;
                eat("INTEGER_DIVIDE");
                newNode->right = factor();
                leftNode = newNode;
            }
        }
        return leftNode;
    }
    
    
    // pre: currentToken.Type exists
    // post:returns the root node of the abstract syntax token tree
     
    Node* expression()
    {
        Node* leftNode;
        Node* newNode;
        bool change = false;
        
        leftNode = term();

        cout<<"PARSER: Expression Token Type: "<<currentToken.Type<<endl;
        
        while ((currentToken.Type == "ADD") || (currentToken.Type == "SUBTRACT"))
        {
            change = true;
            if(currentToken.Type == "ADD")
            {
                cout<<"PARSER: token #:"<<i+1<<endl;
                cout<<"PARSER: Token - Type: "<<currentToken.Type<<" PARSER: Token - Value: "<<currentToken.Value<<endl;
                i++;
 
                newNode = new Node;
                newNode->left = leftNode;
                newNode->myNodeToken.Type = "ADD";
                eat("ADD");
                newNode->right = term();
                leftNode = newNode;
            }
            if(currentToken.Type == "SUBTRACT")
            {
                //
                cout<<"PARSER: token #:"<<i+1<<endl;
                cout<<"PARSER: Token - Type: "<<currentToken.Type<<endl;
                cout<<"PARSER: Token - Value: "<<currentToken.Value<<endl;
                i++;
                //
                newNode = new Node;
                newNode->left = leftNode;
                newNode->myNodeToken.Type = "SUBTRACT";
                eat("SUBTRACT");
                newNode->right = term();
                leftNode = newNode;
            }
        }
        
        return leftNode;
    }
    
    void empty()
    {
        //cout<<"NullOp"<<endl;
    }
    
    Node* variable()
    {
        Node* astNode;

        astNode = new Node;
        astNode->myNodeToken.Value = currentToken.Value;
        astNode->myNodeToken.Type = currentToken.Type;
        eat("ID");
        return astNode;
    }
    
    Node* assignmentStatement()
    {
        Node* newNode = NULL;
        //new expression begin and end
        //eat("BEGIN");
        newNode = new Node;
        newNode->myNodeToken.Type = "ASSIGN";
        newNode->left = variable();
        eat("ASSIGN");
        newNode->right = expression();
        return newNode;
    }
     
    Node* statement()
    {
        Node* newNode = NULL;
        
        if(currentToken.Type == "BEGIN")
        {
            cout<<"Tokens match: "<< currentToken.Type<<endl;
            newNode = compoundStatements();
        }
        else if(currentToken.Type.substr(0,2) == "ID")
        {
            newNode = assignmentStatement();
        }
        else
        {
            newNode = NULL;
        }
        return newNode;
    }
    
    list<Node*>* statementList()
    {
        list<Node*>* newList;
        newList = new list<Node*>;
        newList->push_back(statement());
        
        while(currentToken.Type == "SEMI")
        {
            eat("SEMI");
            newList->push_back(statement());
        }
        if((currentToken.Type.substr(0,3) == "VAR"))
        {
        }
        return newList;
    }
    
    Node* compoundStatements()
    {
        Node* newNode = NULL;
        
        eat("BEGIN");
        newNode = new Node;
        newNode->myNodeToken.Type = "COMPOUND";
        newNode->compoundStatements = statementList();
        eat("END");
        if(currentToken.Type == "SEMI")
        {
            eat("SEMI");
        }
        /*
        if(currentToken.Type == "DOT")
        {
            eat("DOT");
        }
        */
        return newNode;
    }
    
    Node* type_spec()
    {
        Node* datatype = NULL;
        
        if(currentToken.Type == "INTEGER_T")
        {
            datatype = new Node;
            datatype->myNodeToken.Type = "INTEGER_T";
            datatype->myNodeToken.Value = 0;
            eat("INTEGER_T");
        }
        if(currentToken.Type == "REAL_T")
        {
            datatype = new Node;
            datatype->myNodeToken.Type = "FLOAT";
            datatype->myNodeToken.Value = 0;
            eat("REAL_T");
        }
        return datatype;
    }
    
    list<Node*>* variable_declaration()
    {
        list<Node*>* declarationList;
        Node* variable;
        Node* newId;
        Node* dataType;
        
        declarationList = new list<Node*>;
        string name;
        cout<<"Variable declaration current token full type: "<<currentToken.Type<<endl;
        if(currentToken.Type.substr(0,2) == "ID")
        {
            variable = new Node;
            variable->myNodeToken.Type = "ID";
            variable->myNodeToken.Value = 0;
            name = currentToken.Type.substr(2,currentToken.Type.length());
            variable->left = new Node;
            variable->left->myNodeToken.Type = /*"ATTRIBUTE - " + */name;
            newId = variable;
            declarationList->push_back(newId);
            eat("ID");

            while (currentToken.Type == "COMMA")
            {
                eat("COMMA");
                if(currentToken.Type.substr(0,2) == "ID")
                {
                    variable = new Node;
                    variable->myNodeToken.Type = "ID";
                    variable->myNodeToken.Value = 0;
                    name = currentToken.Type.substr(2,currentToken.Type.length());
                    variable->left = new Node;
                    variable->left->myNodeToken.Type = /*"ATTRIBUTE - " + */name;
                    newId = variable;
                    declarationList->push_back(newId);
                    eat("ID");
                }
            }
            
            eat("COLON");
            dataType = type_spec();

            for(list<Node*>::iterator it = declarationList->begin(); it != declarationList->end(); ++it)
            {
                (*it)->right = new Node;
                (*it)->right->myNodeToken.Type = dataType->myNodeToken.Type;
                (*it)->right->myNodeToken.Value = 0;
                cout<<"Parent Node:"<<(*it)->myNodeToken.Type<<" Variable Name:"<<(*it)->left->myNodeToken.Type<<" Variable Datatype:"<<(*it)->right->myNodeToken.Type<<endl;
            }
        }
        return declarationList;
    }
    
    Node* declarations()
    {
        Node* declarationNode = NULL;

        if(currentToken.Type == "VAR")
        {
            declarationNode = new Node;
            declarationNode->myNodeToken.Type = "VAR";
            declarationNode->myNodeToken.Value = 0;
            declarationNode->variableDeclarations = new list<list<Node*>*>;
            eat("VAR");

            while(currentToken.Type.substr(0,2) == "ID")
            {
                declarationNode->variableDeclarations->push_back(variable_declaration());
                eat("SEMI");
            }
        
            cout<<"Printing out the lists"<<endl;
            //START: Code to verify that the list of list of nodes is being created correctly
            for(list<list<Node*>*>::iterator iter = declarationNode->variableDeclarations->begin(); iter!= declarationNode->variableDeclarations->end(); ++iter)
            {
                for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                {
                    cout<<((*iter2)->left->myNodeToken.Type);
                    cout<<((*iter2)->right->myNodeToken.Type);
                }
            }
            return declarationNode;
        }
        
        /*
        while(currentToken.Type == "PROCEDURE")
        {
            procedureNode = new Node;
            procedureNode->myNodeToken.Type = "PROCEDURE -";
            procedureNode->myNodeToken.Value = 0;
            eat("PROCEDURE");
        
            //string proc_name = currentToken.Type.substr(2, currentToken.Type.length());
            nameNode = variable();
            cout<<"The name of the found procedure is: "<<nameNode->myNodeToken.Type;
            procedureNode->myNodeToken.Type = procedureNode->myNodeToken.Type + nameNode->myNodeToken.Type.substr(2,nameNode->myNodeToken.Type.length());
            delete nameNode;
            nameNode = NULL;
            //procedureNode->myNodeToken.Type = procedureNode->myNodeToken.Type + proc_name;
            eat("SEMI");
            procedureNode->child = block();
            declarationNode->child = procedureNode;
            
            return declarationNode;
        }
        //END
        */
        return declarationNode;
    }
    
    list<Node*>* parameters()
    {
        list<Node*>* parameterList = NULL;
        
        parameterList = new list<Node*>;
        
        if(!(currentToken.Type.substr(0,2) == "ID"))
        {
            return parameterList;
        }
        
        parameterList->merge(*getparameter());
        
        while (currentToken.Type == "SEMI")
        {
            eat("SEMI");
            parameterList->merge(*getparameter());
        }

        return parameterList;
    }
    
    list<Node*>* getparameter()
    {
        list<Node*>* parameterList = NULL;
        Node* parameterNode = NULL;
        string nameNode = "";
        
        parameterList = new list<Node*>;

        if((currentToken.Type.substr(0,2) == "ID"))
        {
            parameterNode = new Node;
            parameterNode->myNodeToken.Type = "PARAMETER";
            parameterNode->left = new Node;
            parameterNode->left->myNodeToken.Type = /*"ATTRIBUTE " + */currentToken.Type.substr(2, currentToken.Type.length());
            parameterList->push_back(parameterNode);
            eat("ID");
        }
        
        while(currentToken.Type == "COMMA")
        {
            eat("COMMA");
            parameterNode = new Node;
            parameterNode->myNodeToken.Type = "PARAMETER";
            parameterNode->left = new Node;
            parameterNode->left->myNodeToken.Type = /*"ATTRIBUTE " + */currentToken.Type.substr(2, currentToken.Type.length());
            parameterList->push_back(parameterNode);
            eat("ID");
        }
        
        eat("COLON");
        parameterNode = type_spec();
        
        for(list<Node*>::iterator it = parameterList->begin(); it != parameterList->end(); ++it)
        {
            (*it)->right = new Node;
            (*it)->right->myNodeToken.Type = parameterNode->myNodeToken.Type;
        }
        return parameterList;
    }
    
    Node* procedure()
    {
        Node* procedureNode = NULL;
        Node* nameNode = NULL;
        
        while(currentToken.Type == "PROCEDURE")
        {
            procedureNode = new Node;
            procedureNode->myNodeToken.Type = "PROCEDURE -";
            procedureNode->myNodeToken.Value = 0;
            eat("PROCEDURE");
            nameNode = variable();
            cout<<"The name of the found procedure is: "<<nameNode->myNodeToken.Type;
            procedureNode->myNodeToken.Type = procedureNode->myNodeToken.Type + nameNode->myNodeToken.Type.substr(2,nameNode->myNodeToken.Type.length());
            delete nameNode;
            nameNode = NULL;
            if(currentToken.Type == "LPARENTH")
            {
                eat("LPARENTH");
                procedureNode->parameterlist = parameters();
                eat("RPARENTH");
            }
            eat("SEMI");
            procedureNode->child = block();
        }
        return procedureNode;
    }

    Node* block()
    {
        Node* blockNode = NULL;
        blockNode = new Node;
        
        blockNode->myNodeToken.Type = "BLOCK";
        blockNode->left = declarations();
        blockNode->child = procedure();
        blockNode->right = compoundStatements();

        return blockNode;
    }
    
    Node* program()
    {
        Node* programNode = NULL;
        Node* nameNode = NULL;
        i=0;
        
        if(currentToken.Type == "PROGRAM")
        {
            programNode = new Node;
            programNode->myNodeToken.Type = "PROGRAM - ";
            programNode->myNodeToken.Value = 0;
            eat("PROGRAM");
        
            nameNode = variable();
            
            programNode->myNodeToken.Type = programNode->myNodeToken.Type + nameNode->myNodeToken.Type.substr(2,nameNode->myNodeToken.Type.length());
        
            delete nameNode;
            nameNode = NULL;
            eat("SEMI");

            //cout<<"Program node token name: "<<programNode->myNodeToken.Type<<endl;
            
            programNode->child = block();
       
            eat("DOT");
        }
        return programNode;
    }
};

class SymbolTableCreator
{
public:
    /*"""term : factor ((MUL | DIV) factor)*"""*/
    int result;
    float result_float;
    
    SymbolTable* currentSymbolTable;
    int symbolTableScope;
    
    SymbolTableCreator(SymbolTable* symbolTable)
    {
        currentSymbolTable = symbolTable;
    }

    void visit(Node* asTree)
    {
        
        if(asTree != NULL)
        {
            cout<<"Starting 'symbolcreator' "<<asTree->myNodeToken.Type<<endl;
            
            if(asTree->myNodeToken.Type == "UNARY +")
            {
                cout<<"Unary + visit"<<endl;
                visit(asTree->child);
            }
            else if(asTree->myNodeToken.Type == "UNARY -")
            {
                cout<<"Unary - visit"<<endl;
                visit(asTree->child);
            }
            else if(asTree->myNodeToken.Type == "ADD")
            {
                cout<<"Add visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
            }
            else if(asTree->myNodeToken.Type == "SUBTRACT")
            {
                cout<<"Subtract visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
            }
            else if(asTree->myNodeToken.Type == "MULTIPLY")
            {
                cout<<"Multiply visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
            }
            else if(asTree->myNodeToken.Type == "FLOAT_DIVIDE")
            {
                cout<<"float divide visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
            }
            else if(asTree->myNodeToken.Type == "INTEGER_DIVIDE")
            {
                cout<<"integer divide visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
            }
            else if(asTree->myNodeToken.Type == "INTEGER")
            {
                cout<<"Do I fail here?"<<endl;
                result = asTree->myNodeToken.Value;
                cout<<"Value of 'INTEGER' node: "<<result;
            }
            else if(asTree->myNodeToken.Type == "FLOAT")
            {
                cout<<"Float visit"<<endl;
                result = asTree->myNodeToken.Value;
            }
            else if(asTree->myNodeToken.Type == "DATATYPE")
            {
                cout<<"Datatype visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                result = asTree->myNodeToken.Value;
            }
            else if((asTree->myNodeToken.Type).find("PROCEDURE -") != std::string::npos)
            {
                cout<<"Procedure visit starting found."<<endl;
                string newsymbol;
                string newsymboltype;
                
                string thisname = asTree->myNodeToken.Type.substr(11, asTree->myNodeToken.Type.length());

                currentSymbolTable->insert(thisname, "void", "procedure", 0);

                SymbolTable* procedureSymbolTable = new SymbolTable(thisname, (currentSymbolTable->getscopelevel()+1), currentSymbolTable);
                //procedureSymbolTable->parent = currentSymbolTable;
                currentSymbolTable = procedureSymbolTable;
                
                for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
                {
                    cout<<"Test code"<<endl;
                    newsymbol = (*iter)->left->myNodeToken.Type;
                    cout<<newsymbol<<endl;
                    newsymboltype = (*iter)->right->myNodeToken.Type;
                    cout<<newsymboltype<<endl;
                    currentSymbolTable->insert(newsymbol, newsymboltype, "parameter", 0);

                   /* for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                    {
                        currentSymbolTable->insert((*iter2)->myNodeToken.Type, thisname, (*iter2)->myNodeToken.Type, 0);
                        cout<<((*iter2)->left->myNodeToken.Type);
                        cout<<((*iter2)->right->myNodeToken.Type);
                    }
                    */
                }
                /*
                for(list<list<Node*>*>::iterator iter = asTree->variableDeclarations->begin(); iter!= asTree->variableDeclarations->end(); ++iter)
                {
                    for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                    {
                        cout<<((*iter2)->left->myNodeToken.Type);
                        cout<<((*iter2)->right->myNodeToken.Type);
                    }
                }
                */
                cout<<"Procedure - visit 2"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                visit(asTree->child);
                
                currentSymbolTable = currentSymbolTable->parent;
            }
            else if(((asTree->myNodeToken.Type).find("ID -") != std::string::npos))
            {
                string idtofind = asTree->myNodeToken.Type.substr(4,sizeof(asTree->myNodeToken.Type));

                cout<<"Printing 'IDsubstr' Type: "<<asTree->myNodeToken.Type<<endl;
                cout<<asTree->left->myNodeToken.Type<<endl;
                cout<<asTree->right->myNodeToken.Type<<endl;
                if(currentSymbolTable->find(idtofind) == 1)
                {
                    cout<<"Symbol Found: The symbol "<<idtofind<<" is defined."<<endl;
                    currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
                }
                else
                {
                    cout<<"Error: The symbol '"<<idtofind<<"' is not defined. "<<endl;
                }
            }
            else if(asTree->myNodeToken.Type.substr(0, 2) == "ID")
            {
                cout<<"ID Visit"<<endl;
                string idtofind;
                
                idtofind = asTree->myNodeToken.Type.substr(2,sizeof(asTree->myNodeToken.Type));
                
                if( (currentSymbolTable->find(idtofind)) == 1)
                {
                    cout<<"Symbol Found: The symbol "<<idtofind<<" is defined."<<endl;
                    //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
                }
                else
                {
                    cout<<"Error: The symbol '"<<idtofind<<"' is not defined. "<<endl;
                }
            }
            else if((asTree->myNodeToken.Type).find("ATTRIBUTE -") != std::string::npos)
            {
                string name;
                name = asTree->myNodeToken.Type.substr(12, asTree->myNodeToken.Type.length());
                cout<<"Printing 'Attribute -' Type: "<<asTree->myNodeToken.Type<<" AKA "<<name<<endl;
                //visit(asTree->left);
                //visit(asTree->right);
                if(currentSymbolTable->find(name))
                {
                }
                else
                {
                    cout<<" - "<<asTree->myNodeToken.Type<<endl;
                }
            }
            else if(asTree->myNodeToken.Type == "NONE")
            {
            }
            else if(asTree->myNodeToken.Type == "VAR")
            {
                cout<<"VAR visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                                
                for(list<list<Node*>*>::iterator iter = asTree->variableDeclarations->begin(); iter!= asTree->variableDeclarations->end(); ++iter)
                {
                    for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                    {
                        string name = (*iter2)->left->myNodeToken.Type;
                        //name = name.substr(12,name.length());
                        cout<<(*iter2)->myNodeToken.Type<<endl;
                        visit((*iter2)->left);
                        visit((*iter2)->right);
                        cout<<(*iter2)->right->myNodeToken.Type<<endl;
                        if(currentSymbolTable->find(name))
                        {
                            cout<<"Token "<<name<<" already defined."<<endl;
                        }
                        else
                        {
                            result = currentSymbolTable->insert(name,(*iter2)->right->myNodeToken.Type, "variable",  0);
                        }
                    }
                }
            }
            else if((asTree->myNodeToken.Type).find("PROGRAM -") != std::string::npos)
            {
                SymbolTable* globalSymbolTable = new SymbolTable("name",0, (SymbolTable*)NULL);
                currentSymbolTable = globalSymbolTable;
                cout<<"Program visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                visit(asTree->child);
            }
            else if((asTree->myNodeToken.Type).find("PARAMETER -") != std::string::npos)
            {
                currentSymbolTable->insert(asTree->left->myNodeToken.Type,asTree->right->myNodeToken.Type, "parameter", 0);
                cout<<"Program visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                visit(asTree->child);
            }
            else if(asTree->myNodeToken.Type == "BLOCK")
            {
                cout<<"Block visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                if(asTree->left!=NULL)
                {
                    visit(asTree->left);
                }
                if(asTree->child != NULL)
                {
                    visit(asTree->child);
                };
                if(asTree->right != NULL)
                {
                    visit(asTree->right);
                };
            }
            else if(asTree->myNodeToken.Type == "ASSIGN")
            {
                string name;
                name = asTree->left->myNodeToken.Type.substr(2,asTree->myNodeToken.Type.length());

                cout<<"Assign visit"<<endl;
                cout<<name<<endl;
                if(currentSymbolTable->find(name))
                {
                    visit(asTree->right);
                }
                else
                {
                    cout<<"Error.  Token not found: "<<name<<endl;
                }
                
                //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
                
            }
            else if(asTree->myNodeToken.Type == "COMPOUND")
            {
                cout<<"Compound visit"<<endl;
                for(list<Node*>::iterator iter = asTree->compoundStatements->begin(); iter!= asTree->compoundStatements->end(); ++iter)
                {
                    visit(*iter);
                }
            }
            else
            {
                cout<<"Did I skip all of it"<<endl;
            }
        }
    }
  
    void printsymboltable()
    {
    }
};


class Visitor
{
public:
    /*"""term : factor ((MUL | DIV) factor)*"""*/
    int result;
    string rpn;
    string lisp;
    SymbolTable* currentSymbolTable;
    //
    
    Visitor(SymbolTable* symbolTable)
    {
        currentSymbolTable = symbolTable;
    }
    
    int visit(Node* asTree)
    {
        int result = 0;
        
        if(asTree != NULL)
        {
            cout<<"Starting 'visit' "<<asTree->myNodeToken.Type<<endl;
            
            if(asTree->myNodeToken.Type == "UNARY +")
            {
                cout<<"Unary + visit"<<endl;
                result = visit(asTree->child);
                return result;
            }
            else if(asTree->myNodeToken.Type == "UNARY -")
            {
                cout<<"Unary - visit"<<endl;
                result = -visit(asTree->child);
                return result;
            }
            else if(asTree->myNodeToken.Type == "ADD")
            {
                cout<<"Add visit"<<endl;
                result = visit(asTree->left) + visit(asTree->right);
                return result;
            }
            else if(asTree->myNodeToken.Type == "SUBTRACT")
            {
                cout<<"Subtract visit"<<endl;
                result = visit(asTree->left) - visit(asTree->right);
                return result;
            }
            else if(asTree->myNodeToken.Type == "MULTIPLY")
            {
                cout<<"Multiply visit"<<endl;
                result = visit(asTree->left) * visit(asTree->right);
                return result;
            }
            else if(asTree->myNodeToken.Type == "FLOAT_DIVIDE")
            {
                cout<<"float divide visit"<<endl;
                result = visit(asTree->left) / visit(asTree->right);
                return result;
            }
            else if(asTree->myNodeToken.Type == "INTEGER_DIVIDE")
            {
                cout<<"integer divide visit"<<endl;
                result = visit(asTree->left) % visit(asTree->right);
                return result;
            }
            else if(asTree->myNodeToken.Type == "INTEGER")
            {
                cout<<"Do I fail here?"<<endl;
                result = asTree->myNodeToken.Value;
                cout<<"Value of 'INTEGER' node: "<<result;
                return result;
            }
            else if(asTree->myNodeToken.Type == "FLOAT")
            {
                cout<<"Float visit"<<endl;
                result = asTree->myNodeToken.Value;
                return result;
            }
            else if(asTree->myNodeToken.Type == "DATATYPE")
            {
                cout<<"Datatype visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                result = asTree->myNodeToken.Value;
                return result;
            }
            
            else if(((asTree->myNodeToken.Type).find("ID -") != std::string::npos))
            {
                cout<<"Printing 'IDsubstr' Type: "<<asTree->myNodeToken.Type<<endl;
                cout<<asTree->left->myNodeToken.Type<<endl;
                cout<<asTree->right->myNodeToken.Type<<endl;
                //result = visit(asTree->left);
                //result = visit(asTree->right);
                result = 0;
                //return currentSymbolTable->find(asTree->myNodeToken.Type);'
                return result;
            }
            
            else if(asTree->myNodeToken.Type.substr(0, 2) == "ID")
            {
                string idtofind;
                
                idtofind = asTree->myNodeToken.Type.substr(2,sizeof(asTree->myNodeToken.Type));
                
                result = currentSymbolTable->find(idtofind);

                //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
            }
            
            else if((asTree->myNodeToken.Type).find("ATTRIBUTE -") != std::string::npos)
            {
                cout<<"Printing 'Attribute -' Type: "<<asTree->myNodeToken.Type<<endl;
                //visit(asTree->left);
                //visit(asTree->right);
                currentSymbolTable->find(asTree->myNodeToken.Type);
                result = 0;//
                return result;
            }
            
            else if(asTree->myNodeToken.Type == "NONE")
            {
                return 0;
            }
            
            else if(asTree->myNodeToken.Type == "VAR")
            {
                cout<<"VAR visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                
                for(list<list<Node*>*>::iterator iter = asTree->variableDeclarations->begin(); iter!= asTree->variableDeclarations->end(); ++iter)
                {
                    for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                    {
                        //visit(*iter2);
                        cout<<(*iter2)->myNodeToken.Type<<endl;
                        cout<<visit((*iter2)->left)<<endl;
                        cout<<visit((*iter2)->right)<<endl;
                        cout<<(*iter2)->right->myNodeToken.Type<<endl;
                    }
                }
                result = 0;
                return result;
            }
            
            else if((asTree->myNodeToken.Type).find("PROGRAM -") != std::string::npos)
            {
                cout<<"Program visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                result = visit(asTree->child);
                result = 0;
                return result;
            }
            else if((asTree->myNodeToken.Type).find("PROCEDURE -") != std::string::npos)
            {
                cout<<"Procedure visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                result = visit(asTree->child);
                result = 0;
                return result;
            }
            else if(asTree->myNodeToken.Type == "BLOCK")
            {
                cout<<"Block visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                
                result = visit(asTree->left);
                
                result = visit(asTree->child);
                
                result = visit(asTree->right);
                
                return result;
            }
            
            else if(asTree->myNodeToken.Type == "ASSIGN")
            {
                string name = asTree->left->myNodeToken.Type;
                name = name.substr(2,name.length());

                cout<<"Assign visit"<<endl;
                if(currentSymbolTable->find(name))
                {
                    result = visit(asTree->right);
                }
                else
                {
                    cout<<"Token not found: "<<asTree->left->myNodeToken.Type<<endl;
                }
                
                //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);

                return result;
            }
            else if(asTree->myNodeToken.Type == "COMPOUND")
            {
                cout<<"Compound visit"<<endl;
                for(list<Node*>::iterator iter = asTree->compoundStatements->begin(); iter!= asTree->compoundStatements->end(); ++iter)
                {
                    //cout<<(*iter)->myNodeToken.Type<<endl;
                    result = visit(*iter);
                }
                return result;
            }
            else
            {
                cout<<"Did I skip all of it"<<endl;
            }
        }
        return result;
    }
    
    void rpnTranslator(Node* asTree)
    {
        if(asTree->left!=NULL)
        {
            if(asTree->left->myNodeToken.Type == "INTEGER")
            {
                rpn.append(to_string(asTree->left->myNodeToken.Value));
            }
            else
            {
                rpnTranslator(asTree->left);
            }
        }
 
        if(asTree->right!=NULL)
        {
            if(asTree->right->myNodeToken.Type == "INTEGER")
            {
                rpn.append(to_string(asTree->right->myNodeToken.Value));
            }
            else
            {
                //The type of token at this particular node is, by inspection, an operator
                rpnTranslator(asTree->right);
            }
        }
 
        if((asTree != NULL) && (asTree->myNodeToken.Type != "INTEGER"))
        {
            rpn.append(toSymbol(asTree->myNodeToken.Type));
        }
    }
    
    void lispTranslator(Node* asTree)
    {
        lisp.append("(");
        lisp.append(toSymbol(asTree->myNodeToken.Type));
        
        if(asTree->left!=NULL)
        {
            if(asTree->left->myNodeToken.Type == "INTEGER")
            {
                lisp.append(to_string(asTree->left->myNodeToken.Value));
            }
            else
            {
                lispTranslator(asTree->left);
            }
        }
        
        if(asTree->right!=NULL)
        {
            if(asTree->right->myNodeToken.Type == "INTEGER")
            {
                lisp.append(to_string(asTree->right->myNodeToken.Value));
            }
            else
            {
                lispTranslator(asTree->right);
            }
        }
        lisp.append(")");
    }
    
    void printrpn()
    {
        cout<<rpn<<endl;
    }
    
    void printlisp()
    {
        cout<<lisp<<endl;
    }
};


string stream_as_string( istream& stm )
{
    string str ;
    char c ;
    while( stm.get(c) ) str += c ;
    return str ;
}

string exists_test0 (const std::string& name)
{
    stringstream strStream;
    ifstream filenamed(name.c_str());
    strStream << filenamed.rdbuf(); //read the file
    string str = strStream.str();
    return str;
}

int main()
{
    string toupper;
    string filenamed = "sample_program_param.txt";
    string textfile = exists_test0 (filenamed);
    
    SymbolTable* mySymbolTable = new SymbolTable;

    Lexer thisLexer3(mySymbolTable);
    thisLexer3.getInput(textfile);

    float complete;
    //Parser Object
    Parser parser(&thisLexer3);
    
    SymbolTableCreator createsymbolTable(mySymbolTable);
   
    Node* testNode;
    
    parser.startLexer();
    
    testNode =  parser.program();
    
    createsymbolTable.visit(testNode);
    
    mySymbolTable->print();
   
    Visitor aVisitor(mySymbolTable);
    
    complete = (float)aVisitor.visit(testNode);
    
    return 0;
}


bool isdigit(char digit)
{
    if(digit == '1' || digit == '2' || digit == '3' || digit == '4' || digit == '5' || digit == '6' || digit == '7' || digit == '8' || digit == '9' || digit == '0')
    {
        return true;
    }
    return false;
}


bool nearEnd(int place, int end)
{
    if( (place+1) >= end)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string toSymbol(string word)
{
    if(word == "ADD")
    {
        return "+";
    }
    if(word == "SUBTRACT")
    {
        return "-";
    }
    if(word == "DIVIDE")
    {
        return "/";
    }
    if(word == "MULTIPLY")
    {
        return "*";
    }
    return "";
}

void uppercase(string &word)
{
    for(int i = 0; i < word.size(); i++)
    {
        if(  ((int)word[i] >= LOWERCASESTART) or ((int)word[i] <= LOWERCASEEND) )
        {
            word[i] = word[i] - 32;
        }
    }
}
