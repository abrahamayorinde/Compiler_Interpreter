#include "lexer.h"
#include <cstdio>
#include <iostream>

void Lexer :: getInput(string text)
{
    inputText = text;
    place = 0;
    end = (int)inputText.size()-1;
    tokencount = 0;
    linenum = 0;
    colnum = 0;
}

char Lexer :: currentSymbol()
{
    if(place<end)
    {
        return inputText.at(place);
    }
    else
    {
        if (end == 0)
        {
            return NULL;
        }
        else
        {
            return inputText.at(end-1);
        };
    }
}

Token Lexer :: getToken()
{
    thisToken.Value = "0";
    thisToken.Type = "NULL";
    string literalValue = "";
    string truecopy = "";
    
    cout<<"getToken: place: "<<place<<endl;
    while(place<end)
    {
        cout<<"getToken: count: "<<tokencount<<endl;
        while(inputText.at(place) == '\n')
        {
            linenum++;
            place++;
            colnum = 0;
        }
        if (inputText.at(place) == '{')
        {
            while((inputText.at(place) != '}' || (inputText.at(place) == '\n')) && (place<end))
            {
                linenum++;
                place++;
                colnum++;
            }
            place++;
            colnum++;
        }
        if(place>=end)
        {
            goto labelout;
        }
        while(isspace(inputText.at(place)) && place<end)
        {
            if(nearEnd(place,end))
            {
                place++;
                colnum++;
                goto labelout;
            }
            else
            {
                place++;
                colnum++;
            }
        }
        if(place>=end)
        {
            goto labelout;
        }
        if( isdigit(inputText.at(place)) and (place<end) )
        {
            thisToken.line = linenum;
            thisToken.col = colnum;
            while(isdigit(inputText.at(place)) and (place<end) )
            {
                literalValue.append(1,inputText.at(place));
                thisToken.Type = "INTEGER";
                //thisToken.Value = stoi(literalValue);
                thisToken.Value = literalValue;
                if( place+1 >= end)
                {
                    cout<<"getToken: value at end: "<<thisToken.Value<<endl;
                    place++;
                    colnum++;
                    goto labelout;
                }
                //thisToken.Value = stoi(literalValue);
                place++;
                colnum++;
            }
            
            if( (inputText.at(place) == '.') and (place<end) )
            {
                literalValue.append(1,inputText.at(place));
                place++;
                colnum++;
                
                while( isdigit(inputText.at(place)) and (place<end) )
                {
                    cout<<"Here is the value just following the decimal point: "<<inputText.at(place)<<endl;
                    literalValue.append(1,inputText.at(place));
                    place++;
                    colnum++;
                }
                cout<<"this is the literal value"<<literalValue<<endl;
                thisToken.Type = "FLOAT";
                thisToken.Value = literalValue;

                cout<<"Float captured: "<<thisToken.Value<<endl;
                goto labelout;
            }
            
            goto labelout;
        }
        if( (isalpha(inputText.at(place)) or (inputText.at(place) == '_')) and (place<end) )
        {

            thisToken.line = linenum;
            thisToken.col = colnum;
            
            while((isdigit(inputText.at(place)) or isalpha(inputText.at(place))) and (place<end) )
            {
                literalValue.append(1,inputText.at(place));
                place++;
                colnum++;
            }
            truecopy = literalValue;
            uppercase(literalValue);
            if(literalValue == "BEGIN")
            {
                thisToken.Type = "BEGIN";
            }
            else if(literalValue == "END")
            {
                thisToken.Type = "END";
            }
            else if(literalValue == "PROGRAM")
            {
                thisToken.Type = "PROGRAM";
            }
            else if(literalValue == "INTEGER")
            {
                thisToken.Type = "INTEGER_T";
            }
            else if(literalValue == "REAL")
            {
                thisToken.Type = "REAL_T";
            }
            else if(literalValue == "VAR")
            {
                thisToken.Type = "VAR";
            }
            else if( literalValue == "PROCEDURE")
            {
                thisToken.Type = "PROCEDURE";
            }
            else if((literalValue == "DIV"))
            {
                thisToken.Type = "INTEGER_DIVIDE";
            }
            else
            {
                thisToken.Type = "ID"+truecopy;
            }

            cout<<"getToken: line #: "<<thisToken.line<<endl;
            cout<<"getToken: column #: "<<thisToken.col<<endl;

            goto labelout;
        }
        else if(inputText.substr(place,2) == ":=")
        {
            cout<<"Semi getToken: "<<inputText.substr(place,2)  <<endl;
            thisToken.Type = "ASSIGN";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place+=2;
            colnum+=2;
            goto labelout;
        }
        else if(inputText.substr(place,2) == "==")
        {
            thisToken.Type = "EQUALITY";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place+=2;
            colnum+=2;
            goto labelout;
        }
        else if(inputText.substr(place,2) == "<=")
        {
            thisToken.Type = "LESSTHANEQ";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place+=2;
            colnum+=2;
            goto labelout;
        }
        else if(inputText.substr(place,2) == ">=")
        {
            thisToken.Type = "GREATERTHANEQ";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place+=2;
            colnum+=2;
            goto labelout;
        }
        else if(inputText.substr(place,2) == "!=")
        {
            thisToken.Type = "NEQ";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place+=2;
            colnum+=2;
            goto labelout;
        }
        else if(inputText.at(place) == '+')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "ADD";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == '-')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "SUBTRACT";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == '*')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "MULTIPLY";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == '/')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "FLOAT_DIVIDE";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == '(')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "LPARENTH";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == ')')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "RPARENTH";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == ',')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "COMMA";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == '.')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "DOT";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == ':')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "COLON";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else if(inputText.at(place) == ';')
        {
            cout<<"getToken: "<<inputText.at(place)  <<endl;
            thisToken.Type = "SEMI";
            thisToken.col = colnum;
            thisToken.line = linenum;
            place++;
            colnum++;
            goto labelout;
        }
        else
        {
            logfile << "Unexpected character: "<< inputText.at(place)<<endl;
        }
    }
    labelout:
    if(place>=end)
    {
        logfile.close();
    }
    tokencount++;
    return thisToken;
}
