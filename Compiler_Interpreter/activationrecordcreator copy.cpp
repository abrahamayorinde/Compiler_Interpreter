#include "activation_record.h"
#include "activationrecordcreator.h"
#include "symbol_table.h"
#include "Node.h"
#include <cstring>
#include <string.h>
//Interpreter
    ActivationRecordCreator :: ActivationRecordCreator (ActivationRecord* activeRecord, SymbolTable* symbolTable)
    {
        currentActivationRecord = activeRecord;
        currentSymbolTable = symbolTable;
        nestCounter = 0;
    }
    

    //Each of the nod eevaluations needs to be its own function that returns a Node, whether null or otherwise.
    void ActivationRecordCreator :: visit(Node* asTree)
    {
        if(asTree != NULL)
        {
            cout<<"Starting 'activationrecordcreator' "<<asTree->myNodeToken.Type<<endl;
            
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
                
                newRecord.name = asTree->myNodeToken.Type.substr(11, asTree->myNodeToken.Type.length());
                newRecord.returnType = "NONE";
                newRecord.nesting_level = 0;
                newRecord.callContext = 2;

                strcpy(characters, newRecord.name.c_str());
                if(currentActivationRecord->insert(characters, newRecord))
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
                currentActivationRecord->remove(newRecord.name);
            }
            else if((asTree->myNodeToken.Type).find("PROCCALL:") != std::string::npos)
            {
                
                /*
                 The parameters that were saved in the parser need to be processed here in the procedure call.
                 The parameters are examined by storing the evaluated parameters in the activation record.
                 
                 A new activation record instance should be pushed on top fo the stack.
                 The parameters are evaluated using the parameterList member.  This argument is evaluated within the procedureCall and its return value must be saved in the activationRecord to be called on for later use.
                 */
                
                newRecord.name = asTree->myNodeToken.Type.substr(9, asTree->myNodeToken.Type.length());
                newRecord.callContext = "FUNCTIONCALL";
                newRecord.nesting_level = nestCounter++;
                newRecord.returnType = "";
                
                strcpy(characters, newRecord.name.c_str());
                returnNode = currentSymbolTable->getNode(newRecord.name);
                if( returnNode != NULL)
                {
                     visit(returnNode->location);
                }
                else
                {
                    
                }
                
                /*
                proc_name = node.proc_name

                    ar = ActivationRecord(
                        name=proc_name,
                        type=ARType.PROCEDURE,
                        nesting_level=2,
                    )

                    proc_symbol = node.proc_symbol

                    formal_params = proc_symbol.formal_params
                    actual_params = node.actual_params

                    for param_symbol, argument_node in zip(formal_params, actual_params):
                        ar[param_symbol.name] = self.visit(argument_node)

                    self.call_stack.push(ar)

                    # evaluate procedure body
                    self.visit(proc_symbol.block_ast)
                 */
                /*


                    if(asTree->parameterlist != NULL)
                    {
                        for(list<Node*>::iterator iter = asTree->parameterlist->begin(); iter!= asTree->parameterlist->end(); ++iter)
                        {
                            newRecord.name  = (*iter)->left->myNodeToken.Type;
                            newRecord.datatype = (*iter)->right->myNodeToken.Type;
                            newRecord.argumentCount++;
                            currentActivationRecord->insert(characters, newRecord);

                        }
                    }
                    currentActivationRecord->insert(characters, newRecord);

                */
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
            }
            else if(asTree->myNodeToken.Type == "NONE")
            {
            }
            else if(asTree->myNodeToken.Type == "VAR")
            {
                currentActivationRecord->get(<#char *key#>)
                /*
                 var_name = node.value

                 ar = self.call_stack.peek()
                 var_value = ar.get(var_name)

                 return var_value
                 
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

                    }
                }
                 
                 */
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
                
                currentActivationRecord->remove(tablename);
                
            }
            else if((asTree->myNodeToken.Type).find("PARAMETER -") != std::string::npos)
            {
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
                //assign a value to a variable means that one should push data into the activation record
                //visit or encounter a var gets data out of activation record
                name = asTree->left->myNodeToken.Type.substr(2,asTree->myNodeToken.Type.length());
                strcpy(characters, name.c_str());

                if(currentSymbolTable->find(name))
                {
                    //currentActivationRecord->getIndex(characters) = visit(currentSymbolTable->getNode(name));
                    //currentActivationRecord->get(characters).values = asTree->myNodeToken.Value;
                }
                
                //Node* temp = visit(asTree->right);
                
                /*
                 var_name = node.left.value
                 var_value = self.visit(node.right)

                 ar = self.call_stack.peek()
                 ar[var_name] = var_value
                 */
                
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
  
    void ActivationRecordCreator :: printActivationRecord()
    {
        
    }
