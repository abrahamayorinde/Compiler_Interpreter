#include "Token.h"
#include "miscellaneous.h"

class Lexer
{
private:
    ofstream logfile;

public:
    
    int place;
    int end;
    int tokencount;
    int linenum;
    int colnum;
    bool error = 0;
    bool logging;

    string inputText;
    Token thisToken;

    void getInput(string text);
    char currentSymbol();
    Token getToken();
    
    Lexer(bool tologornot, string filenamed)
    {
        logging = tologornot;
        
        if(logging)
        {
            string filename;
            filename = "lexer_logging_"+filenamed;
            logfile.open (filename);
            logfile << "Writing this to a file.\n";
        }
    }
};
