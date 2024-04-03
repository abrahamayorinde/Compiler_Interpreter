
#include <string>
#include <list>
#include "activation_table.h"
#include "Node.h"
using namespace std;

typedef struct ActivationRecordType
{
    string name;  //name of the variable or function being activated
    string value; //interpreted literal value of the variable or function
    string returnType; //data type of the variable or function TODO: CHANGE THIS TO AN ENUM TYPE
    string callContext; // Not needed?  What did I mean by call context?  Function?  Parameter?  Assign?
    int nesting_level; //scope of function call
    int argumentCount; //number of arguments if the activation record is a function
    list <arguments> argumentList;//list of arguments in the current activation record
    list <arguments> localvarList;//list of local variables in the current activation record
    //ActivationTable* argument = NULL;
} recordType;
