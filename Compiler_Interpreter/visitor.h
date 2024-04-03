class Visitor
{
public:
    //"""term : factor ((MUL | DIV) factor)*"""//
    int result;
    string rpn;
    string lisp;
    SymbolTable* currentSymbolTable = NULL;
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
                    cout<<"Found the entity: "<<name<<endl;
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
