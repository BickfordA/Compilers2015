#include "Parser.h"

#include <string>

using namespace std;


Parser::Parser(TokenStream* tokens, Grammar* grammar)
	:_tokens(tokens)
{
	if (!grammar){
		_grammar = new Grammar();
	}
}

Parser::~Parser()
{
	//dont assume ownership of the tokenStream

	delete _grammar;
}

void Parser::parseTokens()
{
	_grammar->_semanticAnalyser = new SemanticAnalyser();
	_grammar->setTokenStream(_tokens);
	try{
		try{
			_grammar->systemGoal();
		}
		catch (ParseException parseErrors){
			string error;// = _grammar->();
			fprintf(stdout, error.c_str());
		}
	}
	catch (SemanticAnaylserException exception)
	{
		string error = _grammar->_semanticAnalyser->errorMsg();
		error += exception.what();
		fprintf(stdout, error.c_str());
	}
	delete _grammar->_semanticAnalyser;
}

string Parser::errMsgs()
{
	return _grammar->getError();
}

void Parser::printLog()
{
	_grammar->printLog();
}