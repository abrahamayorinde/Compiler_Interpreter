#include "activation_record.h"
#include "activationrecordcreator.h"
#include "symbol_table.h"
#include "Node.h"
#include <cstring>
#include <string.h>
//Interpreter
    ActivationRecordCreator :: ActivationRecordCreator (ActivationRecord* activeRecord)
    {
        currentActivationRecord = activeRecord;
    }
    

    //Each of the nod eevaluations needs to be its own function that returns a Node, whether null or otherwise.
    Node* ActivationRecordCreator :: visit(Node* asTree)
    {
        if(asTree != NULL)
        {
            cout<<"Starting 'activationrecordcreator' "<<asTree->myNodeToken.Type<<endl;
            
            if(asTree->myNodeToken.Type == "UNARY +")
            {
                cout<<"Unary + visit"<<endl;
                return(visit(asTree->child));
            }
            else if(asTree->myNodeToken.Type == "UNARY -")
            {
                cout<<"Unary - visit"<<endl;
                return(visit(asTree->child));
            }
            
            else if(asTree->myNodeToken.Type == "ADD")
            {
                Node* result = NULL;
                cout<<"Add visit"<<endl;
                //visit(asTree->left);
                //visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    result->myNodeToken.Type = visit(asTree->left).myNodeToken.Value + *visit(asTree->right).myNodeToken.Value;
                }
                
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "SUBTRACT")
            {
                cout<<"Subtract visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    result->myNodeToken.Type = visit(asTree->left).myNodeToken.Value - *visit(asTree->right).myNodeToken.Value;
                }
                
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "MULTIPLY")
            {
                cout<<"Multiply visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    result->myNodeToken.Type = visit(asTree->left).myNodeToken.Value * *visit(asTree->right).myNodeToken.Value;
                }
                
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "FLOAT_DIVIDE")
            {
                cout<<"float divide visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    result->myNodeToken.Type = visit(asTree->left).myNodeToken.Value / *visit(asTree->right).myNodeToken.Value;
                }
                
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "INTEGER_DIVIDE")
            {
                cout<<"integer divide visit"<<endl;
                visit(asTree->left);
                visit(asTree->right);
                
                if(asTree->left->myNodeToken.Type == asTree->right->myNodeToken.Type)
                {
                    result->myNodeToken.Type = visit(asTree->left).myNodeToken.Value / *visit(asTree->right).myNodeToken.Value;
                }
                
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "INTEGER")
            {
                cout<<"Do I fail here?"<<endl;
                result = asTree->myNodeToken.Value;
                cout<<"Value of 'INTEGER' node: "<<result;
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "FLOAT")
            {
                cout<<"Float visit"<<endl;
                result = asTree->myNodeToken.Value;
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "DATATYPE")
            {
                cout<<"Datatype visit"<<endl;
                cout<<asTree->myNodeToken.Type<<endl;
                result = asTree->myNodeToken.Value;
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PROCEDURE -") != std::string::npos)
            {
                cout<<"Procedure visit starting found."<<endl;
                string newsymbol;
                string newsymboltype;
                
                newRecord.name = asTree->myNodeToken.Type.substr(11, asTree->myNodeToken.Type.length());
                newRecord.returnType = "NONE";
                newRecord.nesting_level = 0;
                newRecord.callContext = 2;

                strcpy(characters, newRecord.name.c_str());
                if(currentActivationRecord->insert(characters, newRecord) == (-1))
                {
                    cout<<"Procedure declaration successfully entered into activation record."<<endl;
                }
                
                if(asTree->parameterlist != NULL)
                {
                    for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
                    {
                        newsymbol = (*iter)->left->myNodeToken.Type;
                        newsymboltype = (*iter)->right->myNodeToken.Type;
                        cout<<newsymboltype<<endl;
                        temp_argumentList.argName = newsymbol;
                        temp_argumentList.argType = newsymboltype;
                        newRecord.argumentList.push_back(temp_argumentList);
                        newRecord.argumentCount++;
                    }
                }
                
                currentActivationRecord->insert(characters, newRecord);
                cout<<asTree->myNodeToken.Type<<endl;
                visit(asTree->child);
                currentActivationRecord->remove();
                
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PROCCALL:") != std::string::npos)
            {
                /*
                cout<<"Procedure call visit found; 'visiting'"<<endl;
                string newsymbol;
                string newsymboltype;
                
                newRecord.name = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());


                strcpy(characters, newRecord.name.c_str());
                if(currentActivationRecord->getIndex(characters)  == -1 )
                {
                    newRecord.returnType = "";
                    newRecord.nesting_level = 0;
                    newRecord.callContext = 1;
                    newRecord.argumentCount = 0;
                    temp_argumentList.argName = newsymbol;
                    temp_argumentList.argType = newsymboltype;
                    newRecord.argumentList.push_back(temp_argumentList);
                    
                    if(asTree->parameterlist != NULL)
                    {
                        for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
                        {
                            newsymbol = (*iter)->left->myNodeToken.Type;
                            newsymboltype = (*iter)->right->myNodeToken.Type;
                            cout<<newsymboltype<<endl;
                            newRecord.argumentCount++;
                        }
                    }
                    currentActivationRecord->insert(characters, newRecord);
                }
                else
                {
                    cout<<"Stack overflow."<<endl;
                }


                cout<<asTree->myNodeToken.Type<<endl;
                visit(asTree->child);
                currentActivationRecord->remove();
                */
                return asTree;
            }
            else if(((asTree->myNodeToken.Type).find("ID -") != std::string::npos))
            {
                string idtofind = asTree->myNodeToken.Type.substr(4,sizeof(asTree->myNodeToken.Type));

                cout<<asTree->left->myNodeToken.Type<<endl;
                cout<<asTree->right->myNodeToken.Type<<endl;
                /*
                if(currentSymbolTable->find(idtofind) == 1)
                {
                    cout<<"Symbol Found: The symbol "<<idtofind<<" is defined."<<endl;
                    currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
                }
                else
                {
                    cout<<"Error: The symbol '"<<idtofind<<"' is not defined. "<<endl;
                }
                 */
                return asTree;
            }
            else if(asTree->myNodeToken.Type.substr(0, 2) == "ID")
            {
                cout<<"ID Visit"<<endl;
                string idtofind;
                
                idtofind = asTree->myNodeToken.Type.substr(2,sizeof(asTree->myNodeToken.Type));
                /*
                if( (currentSymbolTable->find(idtofind)) == 1)
                {
                    cout<<"Symbol Found: The symbol "<<idtofind<<" is defined."<<endl;
                    //result = currentSymbolTable->insert(asTree->myNodeToken.Type,"Global", "Variable", 0);
                }
                else
                {
                    cout<<"Error: The symbol '"<<idtofind<<"' is not defined. "<<endl;
                }
                */
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("ATTRIBUTE -") != std::string::npos)
            {
                string name;
                name = asTree->myNodeToken.Type.substr(11, asTree->myNodeToken.Type.length());
                cout<<"Printing 'Attribute -' Type: "<<asTree->myNodeToken.Type<<" AKA "<<name<<endl;
                //visit(asTree->left);
                //visit(asTree->right);
                /*
                if(currentSymbolTable->find(name))
                {
                }
                else
                {
                    cout<<" - "<<asTree->myNodeToken.Type<<endl;
                }
                 */
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "NONE")
            {
                return asTree;
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
                        /*
                        if(currentSymbolTable->find(name))
                        {
                            cout<<"Token "<<name<<" already defined."<<endl;
                        }
                        else
                        {
                            cout<<"Token: "<<name<<" added to the table."<<endl;
                            result = currentSymbolTable->insert(name,(*iter2)->right->myNodeToken.Type, "variable",  0);
                        }
                        */
                    }
                }
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PROGRAM -") != std::string::npos)
            {
                tablename = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());
                currentActivationRecord->name = tablename;

                strcpy(characters, tablename.c_str());
                
                newRecord.name = tablename;
                newRecord.callContext = "PROGRAM";
                newRecord.returnType = "NONE";
                newRecord.nesting_level = 1;
                newRecord.argumentCount = 0;
                
                currentActivationRecord->insert(characters, newRecord);
                
                visit(asTree->child);
                
                currentActivationRecord->remove();
                return asTree;
            }
            else if((asTree->myNodeToken.Type).find("PARAMETER -") != std::string::npos)
            {
                visit(asTree->child);
                return asTree;
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
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "ASSIGN")
            {
                int index;
                string name;
                recordType* tempActivation = NULL;
                name = asTree->left->myNodeToken.Type.substr(2,asTree->myNodeToken.Type.length());
                index = currentActivationRecord->getIndex(characters);
                tempActivation->value = "thing";
                visit(asTree->right);
                return asTree;
            }
            else if(asTree->myNodeToken.Type == "COMPOUND")
            {
                cout<<"Compound visit"<<endl;
                for(list<Node*>::iterator iter = asTree->compoundStatements->begin(); iter!= asTree->compoundStatements->end(); ++iter)
                {
                    visit(*iter);
                }
                return asTree;
            }
            else
            {
                cout<<"Did I skip all of it"<<endl;
            }
        }
    }
  
    void ActivationRecordCreator :: printActivationRecord()
    {
    }
