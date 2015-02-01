#ifndef SCANNER_H
#define SCANNER_H

#include "Lexeme.h"
#include "Token.h"

#include <string>
#include <fstream>


using namespace std;
    
class Scanner {
    
public:
    Scanner(string filePath);

    Token getNextToken(); //<- this is the dispatcher 

	bool hasError();
	string getError();

private:


	bool moveToNextTockenStart();

	int _currentColumn;
	int _currentRow;

	bool _hasError;
	string _errorString;

	ifstream* _filePointer;
	string _filePath;
};


#endif //SCANNER_H
