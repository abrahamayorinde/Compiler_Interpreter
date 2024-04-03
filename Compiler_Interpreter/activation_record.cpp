//
//  activation_record.h
//

//#include "ActivationRecordType.h"
#include "activation_record.h"
#include <string>
#include <cstring>
// Function to get the index of a key in the keys array
recordType* ActivationRecord::find(string id)
{
    for(int i = 0; i < size; i++)
    {
      if(values[i]->name == id )
      {
          return values[i];
      }
    }
    return NULL; // not found
}

arguments* ActivationRecord::findArguments(string tablename, string findThis)
{
    arguments* resultArgument = NULL;
    
    for(int i = 0; i < size; i++)
    {
        if(values[i]->name == tablename )
        {
            for(list <arguments>::iterator it = values[i]->argumentList.begin(); it != values[i]->argumentList.end(); ++it)
            {
                if( it->argName == findThis)
                {
                    resultArgument = new arguments;
                    resultArgument->argName = it->argName;
                    resultArgument->argType = it->argType;
                    resultArgument->argValue = it->argValue;
                }
            }
        }
    }
    return resultArgument;
}

arguments* ActivationRecord::findLocals(string tablename, string findThis)
{
    arguments* resultArgument = NULL;
    
    for(int i = 0; i < size; i++)
    {
        if(values[i]->name == tablename )
        {
            for(list <arguments>::iterator it = values[i]->localvarList.begin(); it != values[i]->localvarList.end(); ++it)
            {
                if( it->argName == findThis)
                {
                    resultArgument = new arguments;
                    resultArgument->argName = it->argName;
                    resultArgument->argType = it->argType;
                    resultArgument->argValue = it->argValue;
                }
            }
        }
    }
    return resultArgument;
}

//push activation record onto the stack
bool ActivationRecord::push(recordType* record)
{
    if((size) < MAX)
    {
        values[size] = new recordType;
        *values[size] = *record;
        size++;
        return 1;
    }
    else
    {
        return 0;
    }
}

//pop acviation record from stack
bool ActivationRecord::pop()
{
    if(size>0)
    {
        values[size-1] = NULL;
        delete values[size-1];
        size--;
        return 1;
    }
    else
    {
        return 0;
    }
}

//set values of an activation record
void ActivationRecord :: set(recordType* candidateRecord)
{    
    if (values[size] == NULL)
    {
        values[size] = candidateRecord;
    }
}

recordType*  ActivationRecord :: getCurrent()
{
    return values[size-1];
}


//print the activation record table
void ActivationRecord :: printMap()
{
    for (int i = 0; i < size; i++)
    {
        printf("%s:\n", values[i]->name.c_str());
    }
}
