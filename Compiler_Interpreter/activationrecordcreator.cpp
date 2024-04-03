#include <iostream>
#include <fstream>
#include <string>
#include "activation_record.h"
#include "activationrecordcreator.h"
#include "symbol_table.h"
#include "Node.h"
#include <cstring>
#include <string.h>

using namespace std;
    ActivationRecordCreator :: ActivationRecordCreator (ActivationRecord* activeRecord, SymbolTable* symbolTable, bool il_enabled, string filenamed)
    {
        currentActivationRecord = activeRecord;
        currentSymbolTable = symbolTable;
        nestCounter = 0;
        logging = il_enabled;
        
        if(logging)
        {
            logfile = new ofstream;
            string filename = "activation_record_logger_"+filenamed;
            logfile->open(filename);
            symboltablelogger("Begin activation record logger: \n");
        }
    }

    //Each of the nod eevaluations needs to be its own function that returns a Node, whether null or otherwise.
    Node* ActivationRecordCreator :: visit(Node* asTree)
    {
        if(asTree != NULL)
        {
            symboltablelogger("Starting 'activationrecordcreator' " + asTree->myNodeToken.Type);
            
            if(asTree->myNodeToken.Type == "UNARY +")
            {
                visit(asTree->child);
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "UNARY -")
            {
                visit(asTree->child);
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "ADD")
            {
                Node* result = new Node;
                Node* right = NULL;
                Node* left = NULL;
                
                result->myNodeToken.Type = "ADD";
                left = visit(asTree->left);
                right = visit(asTree->right);
                
                if(left->myNodeToken.Type == right->myNodeToken.Type)
                {
                    if(left->myNodeToken.Type == "FLOAT")
                    {
                        floatValue = stof(left->myNodeToken.Value) + stof(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(floatValue);
                        result->myNodeToken.Type = "FLOAT";
                    }
                    else if(left->myNodeToken.Type == "INTEGER")
                    {
                        intValue = stoi(left->myNodeToken.Value) + stoi(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(intValue);
                        result->myNodeToken.Type = "INTEGER";
                    }
                    else
                    {
                        symboltablelogger("Unrecognized type: " + left->myNodeToken.Type);
                    }
                }
                else
                {
                    symboltablelogger("This datatype: " + asTree->left->myNodeToken.Type + " != " +asTree->right->myNodeToken.Type);
                }
                return result;
            }
            else if(asTree->myNodeToken.Type == "SUBTRACT")
            {
                Node* result = new Node;
                Node* right = NULL;
                Node* left = NULL;
                
                left = visit(asTree->left);
                right = visit(asTree->right);
                if(left->myNodeToken.Type == right->myNodeToken.Type)
                {
                    if(asTree->left->myNodeToken.Type == "FLOAT")
                    {
                        floatValue = stof(left->myNodeToken.Value) - stof(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(floatValue);
                        result->myNodeToken.Type = "FLOAT";
                    }
                    else if(asTree->left->myNodeToken.Type == "INTEGER")
                    {
                        intValue = stoi(left->myNodeToken.Value) - stoi(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(intValue);
                        result->myNodeToken.Type = "INTEGER";

                    }
                    else
                    {
                        symboltablelogger("Unrecognized type: " + left->myNodeToken.Type);
                    }
                }
                else
                {
                    symboltablelogger("This datatype: " +asTree->left->myNodeToken.Type + " != " + asTree->right->myNodeToken.Type);
                }
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "MULTIPLY")
            {
                Node* result = new Node;
                Node* right = NULL;
                Node* left = NULL;
                
                left = visit(asTree->left);
                right = visit(asTree->right);
                
                if(left->myNodeToken.Type == right->myNodeToken.Type)
                {
                    if(asTree->left->myNodeToken.Type == "FLOAT")
                    {
                        floatValue = stof(left->myNodeToken.Value) * stof(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(floatValue);
                        result->myNodeToken.Type = "FLOAT";
                    }
                    else if(asTree->left->myNodeToken.Type == "INTEGER")
                    {
                        intValue = stoi(left->myNodeToken.Value) * stoi(right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(intValue);
                        result->myNodeToken.Type = "INTEGER";
                    }
                    else
                    {
                        symboltablelogger("Unrecognized type: " + left->myNodeToken.Type);
                    }
                }
                else
                {
                    symboltablelogger("This datatype: " +asTree->left->myNodeToken.Type + " != " + asTree->right->myNodeToken.Type);
                }
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "FLOAT_DIVIDE")
            {
                Node* result = new Node;
                Node* right = NULL;
                Node* left = NULL;
                
                string fValue;
                left = visit(asTree->left);
                right = visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    if(asTree->left->myNodeToken.Type == "FLOAT")
                    {
                        floatValue = stof(asTree->left->myNodeToken.Value) / stof(asTree->right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(floatValue);
                        result->myNodeToken.Type = "FLOAT";
                    }
                    else
                    {
                        symboltablelogger("Unrecognized type: " + left->myNodeToken.Type);
                    }
                }
                else
                {
                    symboltablelogger("This datatype: " +asTree->left->myNodeToken.Type + " != " + asTree->right->myNodeToken.Type);
                }
                return result;
            }
            else if(asTree->myNodeToken.Type == "INTEGER_DIVIDE")
            {
                Node* result = new Node;
                Node* right = NULL;
                Node* left = NULL;
                
                left = visit(asTree->left);
                right = visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    if(asTree->left->myNodeToken.Type == "INTEGER")
                    {
                        intValue = stoi(asTree->left->myNodeToken.Value) / stoi(asTree->right->myNodeToken.Value);
                        result->myNodeToken.Value = to_string(intValue);
                        result->myNodeToken.Type = "INTEGER";
                    }
                    else
                    {
                        symboltablelogger("Unrecognized type: " + left->myNodeToken.Type);
                    }
                }
                else
                {
                    symboltablelogger("This datatype: " +asTree->left->myNodeToken.Type + " != " + asTree->right->myNodeToken.Type);
                }
                return result;
            }
            else if(asTree->myNodeToken.Type == "INTEGER")
            {
                resultNode = new Node;
                resultNode->myNodeToken.Value = asTree->myNodeToken.Value;
                resultNode->myNodeToken.Type = "INTEGER";
                return resultNode;
            }
            else if(asTree->myNodeToken.Type == "FLOAT")
            {   
                resultNode = new Node;
                resultNode->myNodeToken.Value = asTree->myNodeToken.Value;
                resultNode->myNodeToken.Type = "FLOAT";
                return resultNode;
            }
            else if(asTree->myNodeToken.Type == "DATATYPE")
            {
                resultNode = new Node;
                resultNode->myNodeToken.Value = asTree->myNodeToken.Value;
                resultNode->myNodeToken.Type = asTree->myNodeToken.Value;

                return resultNode;
            }
            else if((asTree->myNodeToken.Type).find("PROCEDURE -") != std::string::npos)
            {
                //no corresponding action for this token
            }
            else if((asTree->myNodeToken.Type).find("PROCCALL:") != std::string::npos)
            {
                newRecord.returnType = "";
                newRecord.name = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());
                newRecord.callContext = "FUNCTIONCALL";
                newRecord.nesting_level = nestCounter++;
                
                returnNode = currentSymbolTable->getNode(newRecord.name);

                if(currentSymbolTable->find(newRecord.name))
                {
                    if(asTree->parameterlist != NULL)
                    {
                        int i = 0;
                        vector <string> parameterNames = currentSymbolTable->getparametersoftable(newRecord.name,              (int)asTree->parameterlist->size(), currentSymbolTable);
                        for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
                        {
                            //Find the name of the parameter from the parameterlist by searching through the symboltable.
                            //Search by symbol table name, symbol type 'parameter', and order the return by symbol
                            resultNode = visit(*iter);
                            temp_argumentList.argName = /*resultNode->myNodeToken.name;*/parameterNames[i];
                            temp_argumentList.argType = resultNode->myNodeToken.Type;
                            temp_argumentList.argValue = resultNode->myNodeToken.Value;
                            newRecord.argumentList.push_back(temp_argumentList);
                            newRecord.argumentCount++;
                            i++;
                        }
                    }
                    
                    currentActivationRecord->push(&newRecord);

                    if( returnNode != NULL)
                    {
                        for(list<Node*>::iterator iter = returnNode->location->child->right->compoundStatements->begin(); iter!= returnNode->location->child->right->compoundStatements->end(); ++iter)
                        {
                            visit(*iter);
                        }
                        currentActivationRecord->pop();
                    }
                    else
                    {
                    }
                }
                return asTree;
            }
            else if(((asTree->myNodeToken.Type).find("ID -") != std::string::npos))
            {
                string idtofind = asTree->myNodeToken.Type.substr(4,sizeof(asTree->myNodeToken.Type));
 
                return asTree;
            }
            else if(asTree->myNodeToken.Type.substr(0, 2) == "ID")
            {
                string idtofind;
                Node* returnValue = NULL;
                arguments* assignments = NULL;
                                
                idtofind = asTree->myNodeToken.Type.substr(2,sizeof(asTree->myNodeToken.Type));
                
                assignments = currentActivationRecord->findArguments(newRecord.name, idtofind);
                
                if(assignments != NULL)
                {
                    returnValue = new Node;
                    returnValue->myNodeToken.name = idtofind;
                    returnValue->myNodeToken.Type = assignments->argType;
                    returnValue->myNodeToken.Value = assignments->argValue;
                }

                return returnValue;
            }
            else if(asTree->myNodeToken.Type == "NONE")
            {
            }
            else if(asTree->myNodeToken.Type == "VAR")
            {
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PROGRAM -") != std::string::npos)
            {
                currentActivationRecord->name = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());;
                
                newRecord.name = tablename;
                newRecord.callContext = "PROGRAM";
                newRecord.returnType = "NONE";
                newRecord.nesting_level = 1;
                newRecord.argumentCount = 0;
                
                currentActivationRecord->push(&newRecord);
                
                visit(asTree->child);
                
                currentActivationRecord->pop();
                
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PARAMETER -") != std::string::npos)
            {
                visit(asTree->child);
                return asTree;
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
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "ASSIGN")
            {
                Node* temp = NULL;
                string name;

                name = asTree->left->myNodeToken.Type.substr(2,asTree->myNodeToken.Type.length());

                if(currentSymbolTable->findfromtop(name,currentSymbolTable))
                {
                    temp = visit(asTree->right);
                    temp_argumentList.argName = name;//temp->myNodeToken.name;
                    temp_argumentList.argType = temp->myNodeToken.Type;
                    temp_argumentList.argValue = temp->myNodeToken.Value;

                    currentActivationRecord->getCurrent()->localvarList.push_back(temp_argumentList);
                }
                return NULL;
            }
            else if(asTree->myNodeToken.Type == "COMPOUND")
            {
                for(list<Node*>::iterator iter = asTree->compoundStatements->begin(); iter!= asTree->compoundStatements->end(); ++iter)
                {
                    visit(*iter);
                }
                return asTree;
            }
            else
            {
                symboltablelogger("Unrecognized token or token has become NULL" +asTree->myNodeToken.Type);
            }
        }
        return asTree;
    }

    void ActivationRecordCreator::symboltablelogger(string message)
    {
        if(logging)
        {
            (*logfile)<<"\n" + message;
        }
    }

    void ActivationRecordCreator ::  updateSymbolTable(SymbolTable* newTable)
    {
        currentSymbolTable = newTable;
    }

    void ActivationRecordCreator :: printActivationRecord()
    {
        
    }
