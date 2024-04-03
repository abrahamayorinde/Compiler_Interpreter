//#include <iostream>
//#include <fstream>
//#include <cstring>
//#include <sstream>
//using namespace std;

#include "miscellaneous.h"

using namespace std;

string stream_as_string( istream& stm )
{
    string str ;
    char c ;
    while( stm.get(c) ) str += c ;
    return str ;
}

string exists_test0(const std::string& name)
{
    stringstream strStream;
    ifstream filenamed(name.c_str());
    strStream << filenamed.rdbuf(); //read the file
    string str = strStream.str();
    return str;
}

bool isdigit(char digit)
{
    if(digit == '1' || digit == '2' || digit == '3' || digit == '4' || digit == '5' || digit == '6' || digit == '7' || digit == '8' || digit == '9' || digit == '0')
    {
        return true;
    }
    return false;
}


bool nearEnd(int place, int end)
{
    if( (place+1) >= end)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string toSymbol(string word)
{
    if(word == "ADD")
    {
        return "+";
    }
    if(word == "SUBTRACT")
    {
        return "-";
    }
    if(word == "DIVIDE")
    {
        return "/";
    }
    if(word == "MULTIPLY")
    {
        return "*";
    }
    return "";
}

void uppercase(string &word)
{
    for(int i = 0; i < word.size(); i++)
    {
        //if(  ((int)word[i] >= LOWERCASESTART) or ((int)word[i] <= LOWERCASEEND) )
        if(  ((int)word[i] >= 'a') && ((int)word[i] <= 'z') )
        {
            word[i] = word[i] - 32;
        }
    }
}

/*
 string lastN(string input, int n)
  {
      int inputSize = input.size();
      return (n > 0 && inputSize > n) ? input.substr(inputSize - n) : "";
  }
 */
/*
bool isdigit(char digit);
bool nearEnd(int place, int end);
string toSymbol(string word);
void uppercase(string &word);
*/
