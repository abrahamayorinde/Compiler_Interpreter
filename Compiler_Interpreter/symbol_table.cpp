//
//  symbol_table.cpp
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//


#include "symbol_table.h"
#include <iostream>

using namespace std;
// Function to modify an identifier
bool SymbolTable::modify(string id, string t, int l)
{
    int index = hashf(id);
    SymbolNode* start = head[index];
  
    if (start == NULL)
        return "-1";
  
    while (start != NULL)
    {
        if (start->identifier == id)
        {
            start->type = t;
            start->lineNo = l;
            return true;
        }
        start = start->next;
    }
  
    return false; // id not found
}
  
// Function to delete an identifier
bool SymbolTable::deleteRecord(string id)
{
    int index = hashf(id);
    SymbolNode* tmp = head[index];
    SymbolNode* par = head[index];
  
    // no identifier is present at that index
    if (tmp == NULL)
    {
        return false;
    }
    // only one identifier is present
    if (tmp->identifier == id && tmp->next == NULL)
    {
        tmp->next = NULL;
        delete tmp;
        return true;
    }
  
    while (tmp->identifier != id && tmp->next != NULL)
    {
        par = tmp;
        tmp = tmp->next;
    }
    if (tmp->identifier == id && tmp->next != NULL)
    {
        par->next = tmp->next;
        tmp->next = NULL;
        delete tmp;
        return true;
    }
    else
    {
        par->next = NULL;
        tmp->next = NULL;
        delete tmp;
        return true;
    }
    return false;
}
  
// Function to find an identifier
bool SymbolTable::find(string id)
{
    cout<<"Element being searched for: "<<id<<endl;
    int index = hashf(id);
    SymbolNode* start = head[index];
    SymbolTable* traverser  = NULL;
    
    traverser = this;
    
    findit:
    while (start != NULL )
    {
        if (start->identifier == id)
        {
            return 1;
        }
        start = start->next;
    }
    
    if(traverser->parent != ( (SymbolTable*)NULL) )
    {
        traverser = traverser->parent;
        start = traverser->head[0];
        goto findit;
    }
    
    cout << "Element: "<< id<<" Not found in this table: "<<tablename<<endl;
    return 0; // not found
}

vector <string> SymbolTable::getparametersoftable(string searchname, int argCount, SymbolTable* referenceTable)
{
    vector <string> names;
    SymbolTable* traverser = referenceTable;

    if(traverser != NULL)
    {
        if(searchname != traverser->tablename)
        {
            if(!traverser->childTables.empty())
            {
                for(list<SymbolTable*>::iterator iter = traverser->childTables.begin(); iter!= traverser->childTables.end(); ++iter)
                {
                    if((*iter)->tablename == searchname)
                    {
                        return getparametersoftable(searchname, argCount, (*iter));
                    }
                }
            }
        }
        else
        {
            makelist:
            int arguments = 0;
            //start = traverser->head[0];
            for(int i = 0; i<100; i++)
            {
                if(traverser->head[i] != NULL)
                {
                    if(traverser->head[i]->classification == "PARAMETER")
                    {
                        names.push_back(traverser->head[i]->identifier);
                        arguments++;
                        if(arguments > argCount)
                        {
                            cout<<"ERROR: PARAMETERS NOT BEING READ OR STORED CORRECTLY."<<endl;
                        }
                    }
                }
            }
        }
    }
    return names;
}

bool SymbolTable::findfromtop(string id, SymbolTable* referenceTable)
{
    cout<<"Element being searched for: "<<id<<endl;
    int index = hashf(id);
    SymbolNode* start;;
    SymbolTable* traverser = referenceTable;
    
    //traverser = this;
    start = traverser->head[index];
    findit:
    while (start != NULL )
    {
        if (start->identifier == id)
        {
            return 1;
        }
        start = start->next;
    }
    //traverser = start;
    if(!traverser->childTables.empty())
    {
        for(list<SymbolTable*>::iterator iter = traverser->childTables.begin(); iter!= traverser->childTables.end(); ++iter)
        {
            if((*iter)->head[index]->identifier == id)
            {
                return findfromtop(id, *iter);
            }
        }
    }
    cout << "Element: "<< id<<" Not found in this table: "<<tablename<<endl;
    return 0; // not found
}


// Function to insert an identifier
bool SymbolTable::insert(string id, string Type, int depth, string typeclass, int lineno, Node* treeLocation)
{
    int index = hashf(id);
    SymbolNode* p = new SymbolNode(id, Type, depth, typeclass, lineno, treeLocation);
  
    if (head[index] == NULL)
    {
        head[index] = p;
        head[index]->location = treeLocation;
        cout << "\n"<<"Head created: "<<id << " inserted"<<endl;
        cout<< "\n<<"<<"Symbol table name: "<<tablename<<endl;
  
        return true;
    }
    else
    {
        SymbolNode* start = head[index];
        while (start->next != NULL )
        {
            start = start->next;
        }
        start->next = p;
        start->location = treeLocation;
        cout << "\n"<<"Child inserted: "<< id << " inserted"<<endl;
        return true;
    }
    return false;
}

void SymbolTable::print()
{
    int i = 0;
    SymbolNode* start = head[i];

    cout<<endl<<"Printing Symbol Table contents of: "<<tablename<<endl;
    while(i < MAX)
    {
        start = head[i];
        
        while(start!= NULL)
        {
            cout<<"Identifier: "<<start->identifier<<" ";
            cout<<"Type: "<<start->type<<" ";
            cout<<"Line No: "<<start->lineNo<<" ";
            cout<<endl;
            if(start->next != NULL)
            {
                start = start->next;
            }
            else
            {
                break;
            }
            
        }
        ++i;
    }
}

int SymbolTable::hashf(string id)
{
    int asciiSum = 0;
  
    for (int i = 0; i < id.length(); i++)
    {
        asciiSum = asciiSum + id[i];
    }
  
    return (asciiSum % MAX);
}

void SymbolTable::setscopelevel(int newscopelevel)
{
    scopelevel = newscopelevel;
}

SymbolNode* SymbolTable :: getNode(string id)
{
    cout<<"Element being searched for: "<<id<<endl;
    int index = hashf(id);
    SymbolNode* start = head[index];
    SymbolTable* traverser  = NULL;
    
    traverser = this;
    
    findit:
    while (start != NULL )
    {
        if (start->identifier == id)
        {
            return start;
        }
        start = start->next;
    }
    
    if(traverser->parent != ( (SymbolTable*)NULL) )
    {
        //start = this->parent->head[0];
        traverser = traverser->parent;
        start = traverser->head[0];
        goto findit;
    }
    
    cout << "Element: "<< id<<" Not found in this table: "<<tablename<<endl;
    return NULL; // not found
}


int SymbolTable::getscopelevel()
{
    return scopelevel;
}
