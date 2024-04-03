#include "symboltablecreator.h"
#include <fstream>
//Semantic Analyzer
SymbolTableCreator :: SymbolTableCreator (SymbolTable* symbolTable, bool sl_enabled, string filenamed)
{
    currentSymbolTable = symbolTable;
    logging = sl_enabled;
    if(logging)
    {
        logfile = new ofstream;
        string filename;
        filename = "symboltable_logging_"+filenamed;
        (*logfile).open (filename);
        (*logfile) << "Begin symbol table logger: .\n";
    }
}

SymbolTableCreator :: ~SymbolTableCreator()
{
    if(logging)
    {
        (*logfile).close();
    }
}

void SymbolTableCreator :: visit(Node* asTree)
{
    int vardepth = 0;
    
    if(asTree != NULL)
    {
        symboltablelogger("Starting 'symbolcreator' " + asTree->myNodeToken.Type);
        
        if(asTree->myNodeToken.Type == "UNARY +")
        {
            symboltablelogger("Unary + visit");
            visit(asTree->child);
        }
        else if(asTree->myNodeToken.Type == "UNARY -")
        {
            symboltablelogger("Unary - visit");
            visit(asTree->child);
        }
        else if(asTree->myNodeToken.Type == "ADD")
        {
            symboltablelogger("Unary 'ADD'");
            visit(asTree->left);
            visit(asTree->right);
        }
        else if(asTree->myNodeToken.Type == "SUBTRACT")
        {
            symboltablelogger("Unary 'SUBTRACT'");
            visit(asTree->left);
            visit(asTree->right);
        }
        else if(asTree->myNodeToken.Type == "MULTIPLY")
        {
            symboltablelogger("Unary 'MULTIPLY'");
            visit(asTree->left);
            visit(asTree->right);
        }
        else if(asTree->myNodeToken.Type == "FLOAT_DIVIDE")
        {
            symboltablelogger("Unary 'FLOAT DIVIDE'");
            visit(asTree->left);
            visit(asTree->right);
        }
        else if(asTree->myNodeToken.Type == "INTEGER_DIVIDE")
        {
            symboltablelogger("Unary 'INTEGER DIVIDE'");
            visit(asTree->left);
            visit(asTree->right);
        }
        else if(asTree->myNodeToken.Type == "INTEGER")
        {
            symboltablelogger("Unary 'INTEGER'");
        }
        else if(asTree->myNodeToken.Type == "FLOAT")
        {
            symboltablelogger("Unary 'FLOAT'");
        }
        else if(asTree->myNodeToken.Type == "DATATYPE")
        {
            symboltablelogger("Unary 'DATATYPE'");
        }
        else if((asTree->myNodeToken.Type).find("PROCEDURE - ") != std::string::npos)
        {
            string newsymbol;
            string newsymboltype;
            string thisname = asTree->myNodeToken.Type.substr(12, asTree->myNodeToken.Type.length());
            int depth = 0;
            vardepth = 0;
            currentSymbolTable->insert(thisname, "void", 0, "PROCEDURE", asTree->myNodeToken.line, asTree);
            SymbolTable* procedureSymbolTable = new SymbolTable(thisname,(currentSymbolTable->getscopelevel()+1), currentSymbolTable);
            currentSymbolTable->childTables.push_back(procedureSymbolTable);
            procedureSymbolTable->parent = currentSymbolTable;
            currentSymbolTable = procedureSymbolTable;
            
            if(asTree->parameterlist != NULL)
            {
                for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!=asTree->parameterlist->end(); ++iter)
                {
                    newsymbol = (*iter)->left->myNodeToken.name;
                    newsymboltype = (*iter)->right->myNodeToken.Type;
                    currentSymbolTable->insert(newsymbol, newsymboltype, depth, "PARAMETER", (*iter)->right->myNodeToken.line, *iter);
                    depth++;
                }
            }
            visit(asTree->child);
            
            currentSymbolTable = currentSymbolTable->parent;
        }
        else if(asTree->myNodeToken.Type.substr(0, 9) == "PROCCALL:")
        {
            for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
            {
                visit(*iter);
            }
        }
        else if(((asTree->myNodeToken.Type).find("ID -") != std::string::npos))
        {
            string idtofind = asTree->myNodeToken.Type.substr(4,sizeof(asTree->myNodeToken.Type));

            if(currentSymbolTable->find(idtofind) == 1)
            {
                currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", vardepth, "VARIABLE", asTree->myNodeToken.line, asTree);
            }
            else
            {
                symboltablelogger("Error: The symbol '" + idtofind +"' is not defined. ");
            }
        }
        else if(asTree->myNodeToken.Type.substr(0, 2) == "ID")
        {
            string idtofind;

            idtofind = asTree->myNodeToken.Type.substr(2,sizeof(asTree->myNodeToken.Type));
            
            if( (currentSymbolTable->find(idtofind)) == 1)
            {
                //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
            }
            else
            {
                symboltablelogger("Error: The symbol '" + idtofind +"' is not defined. ");
            }
        }
        else if((asTree->myNodeToken.Type).find("ATTRIBUTE -") != std::string::npos)
        {
            //TODO: DELETE THIS BRANCH. THERE IS NO TOKEN NAMED 'ATTRIBUTE - '
            string name;
            name = asTree->myNodeToken.Type.substr(11, asTree->myNodeToken.Type.length());

            if(currentSymbolTable->find(name))
            {
            }
            else
            {
            }
        }
        else if(asTree->myNodeToken.Type == "NONE")
        {
        }
        else if(asTree->myNodeToken.Type == "VAR")
        {
            for(list<list<Node*>*>::iterator iter = asTree->variableDeclarations->begin(); iter!=asTree->variableDeclarations->end(); ++iter)
            {
                for(list<Node*>::iterator iter2 = (*iter)->begin(); iter2!= (*iter)->end(); ++iter2)
                {
                    string name = (*iter2)->left->myNodeToken.Type;
                    visit((*iter2)->left);
                    visit((*iter2)->right);
                    if(currentSymbolTable->find(name))
                    {
                        symboltablelogger("Error: The symbol '" + name+" already defined.");
                    }
                    else
                    {
                        vardepth++;
                        result = currentSymbolTable->insert(name,(*iter2)->right->myNodeToken.Type, vardepth, "VARIABLE",  0, *iter2);
                    }
                }
            }
        }
        else if(asTree->myNodeToken.Type.substr(0, 9) == "PROGRAM -")
        {
            vardepth = 0;
            tablename = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());
            SymbolTable* globalSymbolTable = new SymbolTable(tablename,0, (SymbolTable*)NULL);
            currentSymbolTable = globalSymbolTable;
            visit(asTree->child);
        }
        else if((asTree->myNodeToken.Type).find("PARAMETER -") != std::string::npos)
        {
            currentSymbolTable->insert(asTree->left->myNodeToken.Type,asTree->right->myNodeToken.Type, vardepth, "PARAMETER", asTree->myNodeToken.line, asTree);
            visit(asTree->child);
        }
        else if(asTree->myNodeToken.Type == "BLOCK")
        {
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
            string name = asTree->left->myNodeToken.Type.substr(2,asTree->myNodeToken.Type.length());
            if(currentSymbolTable->find(name))
            {
                visit(asTree->right);
            }
            else
            {
                symboltablelogger("Error.  Undeclared token: "+name);
            }
        }
        else if(asTree->myNodeToken.Type == "COMPOUND")
        {
            for(list<Node*>::iterator iter = asTree->compoundStatements->begin(); iter!=asTree->compoundStatements->end(); ++iter)
            {
                visit(*iter);
            }
        }
        else
        {
            symboltablelogger("Unrecognized token:" + asTree->myNodeToken.Type);
        }
    }
}

void SymbolTableCreator::symboltablelogger(string message)
{
    if(logging)
    {
        (*logfile)<<"\n" + message;
    }
}

void SymbolTableCreator::printsymboltable(SymbolTable* tempSymbolTable)
{
    while(tempSymbolTable != NULL)
    {
        tempSymbolTable->print();
        
        if(!tempSymbolTable->childTables.empty())
        {
            for (std::list<SymbolTable*>::iterator it = tempSymbolTable->childTables.begin(); it != tempSymbolTable->childTables.end(); ++it)
            {
                printsymboltable(*it);
            }
        }
        goto finished;
    }
finished:;
}

SymbolTable* SymbolTableCreator :: returnTable()
{
    return currentSymbolTable;
}

void SymbolTableCreator :: printAllTables()
{
    printsymboltable(currentSymbolTable);
}

