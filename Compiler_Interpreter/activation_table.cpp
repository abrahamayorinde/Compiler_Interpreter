//
//  symbol_table.cpp
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//


#include "activation_table.h"
#include <iostream>

using namespace std;
// Function to modify an identifier
bool ActivationTable::modify(string id, string t, int l)
{
    int index = hashf(id);
    ActivationNode* start = head[index];
  
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
bool ActivationTable::deleteRecord(string id)
{
    int index = hashf(id);
    ActivationNode* tmp = head[index];
    ActivationNode* par = head[index];
  
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
bool ActivationTable::find(string id)
{
    cout<<"Element being searched for: "<<id<<endl;
    int index = hashf(id);
    ActivationNode* start = head[index];
    ActivationTable* traverser  = NULL;
    
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
    
    if(traverser->parent != ( (ActivationTable*)NULL) )
    {
        traverser = traverser->parent;
        start = traverser->head[0];
        goto findit;
    }
    
    cout << "Element: "<< id<<" Not found in this table: "<<tablename<<endl;
    return 0; // not found
}
  
// Function to insert an identifier
bool ActivationTable::insert(string id, string Type, string typeclass, int lineno)
{
    int index = hashf(id);
    ActivationNode* p = new ActivationNode(id, Type, typeclass, lineno);
  
    if (head[index] == NULL)
    {
        head[index] = p;
        cout << "\n"<<"Head created: "<<id << " inserted"<<endl;
        cout<< "\n<<"<<"Symbol table name: "<<tablename<<endl;
  
        return true;
    }
    else
    {
        ActivationNode* start = head[index];
        while (start->next != NULL )
        {
            start = start->next;
        }
        start->next = p;
        cout << "\n"<<"Child inserted: "<< id << " inserted"<<endl;
        return true;
    }
    return false;
}

void ActivationTable::print()
{
    int i = 0;
    ActivationNode* start = head[i];

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

int ActivationTable::hashf(string id)
{
    int asciiSum = 0;
  
    for (int i = 0; i < id.length(); i++)
    {
        asciiSum = asciiSum + id[i];
    }
  
    return (asciiSum % MAX);
}

void ActivationTable::setscopelevel(int newscopelevel)
{
    scopelevel = newscopelevel;
}

ActivationNode* ActivationTable :: getNode(string id)
{
    cout<<"Element being searched for: "<<id<<endl;
    int index = hashf(id);
    ActivationNode* start = head[index];
    ActivationTable* traverser  = NULL;
    
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
    
    if(traverser->parent != ( (ActivationTable*)NULL) )
    {
        //start = this->parent->head[0];
        traverser = traverser->parent;
        start = traverser->head[index];
        goto findit;
    }
    
    cout << "Element: "<< id<<" Not found in this table: "<<tablename<<endl;
    return NULL; // not found
}


int ActivationTable::getscopelevel()
{
    return scopelevel;
}
