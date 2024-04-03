#include "parser.h"
#include <fstream>

    Parser :: Parser(Lexer* lexer, bool pl_enabled, string filenamed)
    {
        thislexer = lexer;
        logging = pl_enabled;
        
        if(logging)
        {
            string filename;
            filename = "parser_logging_"+filenamed;
            logfile.open (filename);
            logfile << "Writing this to a file.\n";
        }
    }

    void Parser :: error()
    {
        //do nothing
    }
    
    void Parser :: startLexer()
    {
        currentToken = thislexer->getToken();
        logfile<<"\nPARSER: JUST EATEN TOKEN : "<<currentToken.Type<<endl;
        cursorTokenCharType = getcursorSymbol();
    }
    
    char Parser :: getcursorSymbol()
    {
        return thislexer->currentSymbol();
    }
    
    void Parser :: eat(string tokenType)
    {
            if((currentToken.Type == tokenType))
            {
                currentToken = thislexer->getToken();
                logfile<<"\nPARSER: JUST EATEN TOKEN : "<<currentToken.Type<<endl;
            }
            else if(currentToken.Type.substr(0,2) == tokenType)
            {
                currentToken = thislexer->getToken();
                logfile<<"\nPARSER: JUST EATEN 'ID' TOKEN : "<<currentToken.Type<<endl;
            }
            else
            {
                logfile<<":Mismatch Error- expected:"<<tokenType<<"<> actual: "<<currentToken.Type<<endl;
            }
    }
    /*
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
    */
    Node* Parser :: factor()
    {
        Node* astNode;
        Node* newNode;
        
        Token current = currentToken;
                
        if( currentToken.Type == "ADD" )
        {
            newNode = new unaryNode;
            eat("ADD");
            newNode->myNodeToken.Value = "0";
            newNode->myNodeToken.Type = "UNARY +";
            newNode->myNodeToken.line = currentToken.line;
            newNode->myNodeToken.col = currentToken.col;
            newNode->child = factor();
            astNode = newNode;
            return newNode;
        }
        if( currentToken.Type == "SUBTRACT")
        {
            newNode = new unaryNode;
            eat("SUBTRACT");
            newNode->myNodeToken.Value = "0";
            newNode->myNodeToken.Type = "UNARY -";
            newNode->myNodeToken.line = currentToken.line;
            newNode->myNodeToken.col = currentToken.col;
            newNode->child = factor();
            astNode = newNode;
            return newNode;
        }
        if( currentToken.Type == "INTEGER" )
        {

            astNode = new Node;
            astNode->myNodeToken.Value = currentToken.Value;
            astNode->myNodeToken.Type = currentToken.Type;
            astNode->myNodeToken.line = currentToken.line;
            astNode->myNodeToken.col = currentToken.col;
            astNode->left = NULL;
            astNode->right = NULL;
            eat("INTEGER");

            return astNode;
        }
        if( currentToken.Type == "FLOAT" )
        {
            astNode = new Node;
            astNode->myNodeToken.Value = currentToken.Value;
            astNode->myNodeToken.Type = currentToken.Type;
            astNode->myNodeToken.line = currentToken.line;
            astNode->myNodeToken.col = currentToken.col;
            astNode->left = NULL;
            astNode->right = NULL;
            eat("FLOAT");

            return astNode;
        }
        if( currentToken.Type == "LPARENTH")
        {
            eat("LPARENTH");
            astNode = expression();
            eat("RPARENTH");
            return astNode;
        }
        
        else //if((currentToken.Type.substr(0,3) == "VAR"))
        {
            astNode = variable();
            return astNode;
        }
        
        return astNode;
    }
    
    Node* Parser :: term()
    {
        Node* leftNode = NULL;
        Node* newNode = NULL;
        bool change = false;
        Token current = currentToken;
        
        leftNode = factor();
        
        logfile<<"PARSER: Term Token Type: "<<currentToken.Type<<endl;

        while( (currentToken.Type == "MULTIPLY") || (currentToken.Type == "FLOAT_DIVIDE") || (currentToken.Type == "INTEGER_DIVIDE") )
        {
            change = true;
            if(currentToken.Type == "MULTIPLY")
            {
                newNode = new Node;
                newNode->myNodeToken.Type = currentToken.Type;
                newNode->myNodeToken.line = currentToken.line;
                newNode->myNodeToken.col = currentToken.col;
                newNode->left = leftNode;
                eat("MULTIPLY");
                newNode->right = factor();
                leftNode = newNode;
            }
            if(currentToken.Type == "FLOAT_DIVIDE")
            {

                newNode = new Node;
                newNode->myNodeToken.Type = currentToken.Type;
                newNode->left = leftNode;
                newNode->myNodeToken.line = currentToken.line;
                newNode->myNodeToken.col = currentToken.col;
                eat("FLOAT_DIVIDE");
                newNode->right = factor();
                leftNode = newNode;
            }
            if(currentToken.Type == "INTEGER_DIVIDE")
            {
                newNode = new Node;
                newNode->myNodeToken.Type = currentToken.Type;
                newNode->left = leftNode;
                newNode->myNodeToken.line = currentToken.line;
                newNode->myNodeToken.col = currentToken.col;
                eat("INTEGER_DIVIDE");
                newNode->right = factor();
                leftNode = newNode;
            }
        }
        return leftNode;
    }
    
    
    // pre: currentToken.Type exists
    // post:returns the root node of the abstract syntax token tree
     
    Node* Parser :: expression()
    {
        Node* leftNode = NULL;
        Node* newNode = NULL;
        bool change = false;
        
        leftNode = term();

        logfile<<"PARSER: Expression Token Type: "<<currentToken.Type<<endl;
        
        while ((currentToken.Type == "ADD") || (currentToken.Type == "SUBTRACT"))
        {
            change = true;
            if(currentToken.Type == "ADD")
            {
                newNode = new Node;
                newNode->left = leftNode;
                newNode->myNodeToken.Type = "ADD";
                newNode->myNodeToken.line = currentToken.line;
                newNode->myNodeToken.col = currentToken.col;
                eat("ADD");
                newNode->right = term();
                leftNode = newNode;
            }
            if(currentToken.Type == "SUBTRACT")
            {
                newNode = new Node;
                newNode->left = leftNode;
                newNode->myNodeToken.line = currentToken.line;
                newNode->myNodeToken.col = currentToken.col;
                newNode->myNodeToken.Type = "SUBTRACT";
                eat("SUBTRACT");
                newNode->right = term();
                leftNode = newNode;
            }
        }
        
        return leftNode;
    }
    
    void Parser :: empty()
    {
    }
    
    Node* Parser :: variable()
    {
        Node* astNode = NULL;
        astNode = new Node;
        astNode->myNodeToken.Value = currentToken.Value;
        astNode->myNodeToken.Type = currentToken.Type;
        astNode->myNodeToken.line = currentToken.line;
        astNode->myNodeToken.col = currentToken.col;
        eat("ID");
        return astNode;
    }
    
    Node* Parser :: assignmentStatement()
    {
        Node* newNode = NULL;
        newNode = new Node;
        newNode->myNodeToken.Type = "ASSIGN";
        newNode->myNodeToken.line = currentToken.line;
        newNode->myNodeToken.col = currentToken.col;
        newNode->left = variable();
        eat("ASSIGN");
        newNode->right = expression();
        return newNode;
    }
     
    Node* Parser :: statement()
    {
        Node* newNode = NULL;
        
        if(currentToken.Type == "BEGIN")
        {
            newNode = compoundStatements();
        }
        else if((currentToken.Type.substr(0,2) == "ID") && ((thislexer->currentSymbol() == '(')))
        {
            newNode = procedureCall();
        }
        else if(currentToken.Type.substr(0,2) == "ID")
        {
            newNode = assignmentStatement();
        }

        return newNode;
    }
    
    list<Node*>* Parser :: statementList()
    {
        list<Node*>* newList = NULL;
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
    
    Node* Parser :: compoundStatements()
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
        return newNode;
    }
    
    Node* Parser :: type_spec()
    {
        Node* datatype = NULL;
        
        if(currentToken.Type == "INTEGER_T")
        {
            datatype = new Node;
            datatype->myNodeToken.Type = "INTEGER_T";
            datatype->myNodeToken.Value = "0";
            eat("INTEGER_T");
        }
        else if(currentToken.Type == "REAL_T")
        {
            datatype = new Node;
            datatype->myNodeToken.Type = "REAL_T";
            datatype->myNodeToken.Value = "0";
            eat("REAL_T");
        }
        else
        {
            datatype->myNodeToken.Type = "UNEXPECTED TOKEN: " + currentToken.Type;
        }
        return datatype;
    }
    
    list<Node*>* Parser :: variable_declaration()
    {
        list<Node*>* declarationList = NULL;
        Node* variable = NULL;
        Node* newId = NULL;
        Node* dataType = NULL;
        
        declarationList = new list<Node*>;
        string name;
        logfile<<"Variable declaration current token full type: "<<currentToken.Type<<endl;
        if(currentToken.Type.substr(0,2) == "ID")
        {
            variable = new Node;
            variable->myNodeToken.Type = "ID";
            variable->myNodeToken.Value = "0";
            name = currentToken.Type.substr(2,currentToken.Type.length());
            variable->left = new Node;
            variable->left->myNodeToken.Type = name;
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
                    variable->myNodeToken.Value = "0";
                    name = currentToken.Type.substr(2,currentToken.Type.length());
                    variable->left = new Node;
                    variable->left->myNodeToken.Type = name;
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
                (*it)->right->myNodeToken.Value = "0";
                logfile<<"Parent Node:"<<(*it)->myNodeToken.Type<<" Variable Name:"<<(*it)->left->myNodeToken.Type<<" Variable Datatype:"<<(*it)->right->myNodeToken.Type<<endl;
            }
        }
        return declarationList;
    }
    
    Node* Parser :: declarations()
    {
        Node* declarationNode = NULL;

        if(currentToken.Type == "VAR")
        {
            declarationNode = new Node;
            declarationNode->myNodeToken.Type = "VAR";
            declarationNode->myNodeToken.Value = "0";
            declarationNode->variableDeclarations = new list<list<Node*>*>;
            eat("VAR");

            while(currentToken.Type.substr(0,2) == "ID")
            {
                declarationNode->variableDeclarations->push_back(variable_declaration());
                eat("SEMI");
            }
        
            logfile<<"Printing out the lists"<<endl;
            //START: Code to verify that the list of list of nodes is being created correctly
            /*
            for(list<list<Node*>*>::iterator iter = declarationNode->variableDeclarations->begin(); iter!= declarationNode->variableDeclarations->end(); ++iter)
            {
                for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                {
                    cout<<((*iter2)->left->myNodeToken.Type);
                    cout<<((*iter2)->right->myNodeToken.Type);
                }
            }

            return declarationNode;
             */
        }
        
        return declarationNode;
    }
    
    list<Node*>* Parser :: parameters()
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
    
    list<Node*>* Parser :: getparameter()
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
            parameterNode->left->myNodeToken.name = currentToken.Type.substr(2, currentToken.Type.length());
            parameterList->push_back(parameterNode);
            eat("ID");
        }
        
        while(currentToken.Type == "COMMA")
        {
            eat("COMMA");
            parameterNode = new Node;
            parameterNode->myNodeToken.Type = "PARAMETER";
            parameterNode->left = new Node;
            parameterNode->left->myNodeToken.name = currentToken.Type.substr(2, currentToken.Type.length());
            parameterList->push_back(parameterNode);
            eat("ID");
        }
    
        eat("COLON");
        parameterNode = type_spec();
        
        for(list<Node*>::iterator it = parameterList->begin(); it != parameterList->end(); ++it)
        {
            (*it)->right = new Node;
            (*it)->right->myNodeToken.Type = parameterNode->myNodeToken.Type;
            (*it)->left->myNodeToken.Type = parameterNode->myNodeToken.Type;

        }
        return parameterList;
    }
    
    Node* Parser :: procedureCall()
    {
        Node* procedurecallNode = NULL;
        
        while(currentToken.Type.substr(0,2) == "ID")
        {
            procedurecallNode = new Node;
            procedurecallNode->myNodeToken.Type = "PROCCALL:" + currentToken.Type.substr(2,currentToken.Type.length());
            procedurecallNode->myNodeToken.Value = "0";
            eat("ID");
            logfile<<"The parsed procedure call: "<<procedurecallNode->myNodeToken.Type;
            procedurecallNode->parameterlist = new list<Node*>;
            if(currentToken.Type == "LPARENTH")
            {
                eat("LPARENTH");
                procedurecallNode->parameterlist->push_back(expression());
                while(currentToken.Type == "COMMA")
                {
                    eat("COMMA");

                    procedurecallNode->parameterlist->push_back(expression());
                }
                eat("RPARENTH");
            }
        }
        return procedurecallNode;
    }
    
    Node* Parser :: procedure()
    {
        Node* procedureNode = NULL;
        Node* nameNode = NULL;
        
        while(currentToken.Type == "PROCEDURE")
        {
            procedureNode = new Node;
            procedureNode->myNodeToken.Type = "PROCEDURE - ";
            procedureNode->myNodeToken.Value = "0";
            procedureNode->myNodeToken.line = currentToken.line;
            procedureNode->myNodeToken.col = currentToken.col;

            eat("PROCEDURE");
            nameNode = variable();
            logfile<<"The name of the found procedure is: "<<nameNode->myNodeToken.Type;
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

    Node* Parser :: block()
    {
        Node* blockNode = NULL;
        blockNode = new Node;
        
        blockNode->myNodeToken.Type = "BLOCK";
        blockNode->left = declarations();
        blockNode->child = procedure();
        blockNode->right = compoundStatements();

        return blockNode;
    }
    
    Node* Parser :: program()
    {
        Node* programNode = NULL;
        Node* nameNode = NULL;
        
        if(currentToken.Type == "PROGRAM")
        {
            programNode = new Node;
            programNode->myNodeToken.Value = "0";
            programNode->myNodeToken.line = currentToken.line;
            programNode->myNodeToken.col = currentToken.col;

            eat("PROGRAM");
        
            nameNode = variable();
            
            programNode->myNodeToken.Type = "PROGRAM - " + nameNode->myNodeToken.Type.substr(2,nameNode->myNodeToken.Type.length());
        
            delete nameNode;
            nameNode = NULL;
            eat("SEMI");
            programNode->child = block();
            eat("DOT");
        }
        if(logging)
        {
            logfile.close();
        }
        return programNode;
    }

