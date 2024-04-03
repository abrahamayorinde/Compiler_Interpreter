//
//  activation_record.h
//  
//
//  Created by Abraham Ayorinde on 9/7/22.
//

#ifndef activation_record_h
#define activation_record_h
#include <stdio.h>
#include <string>
#include <cstring>
#include "typedefarguments.h"
#include "ActivationRecordType.h"

#define MAX_SIZE 100 // Maximum number of elements in the map
using namespace std;


class ActivationRecord
{
public:

    int size = 0; // Current number of elements in the map
    recordType* values[MAX_SIZE];
    const int STACKMAX = 100;

    string name;
    recordType* find(string id);
    arguments* findArguments(string tablename, string findThis);
    arguments* findLocals(string tablename, string findThis);
    recordType* getCurrent();
    bool push(recordType* record);
    bool pop();
    void set(recordType* candidateRecord);
    int getCurrentIndex();
    void printMap();
};

#endif /* activation_record_h */
