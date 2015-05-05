#include "Scanner.h"

#include "Token.h"
#include "FiniteStateAutomaton.h"
#include "Resources.h"

#include <string>
#include <algorithm>

using namespace LexemeResources;

Scanner::Scanner(string filePath)
{
	_filePointer = new ifstream(filePath);
	_currentRow = 0;
	_currentColumn = 0;
}

Scanner::~Scanner()
{
	if (_filePointer){
		if (_filePointer->is_open()){
			_filePointer->close();
		}
		delete _filePointer;
	}
}

bool Scanner::isValid()
{
	bool isValid = true;
	isValid &= _filePointer->is_open();
	isValid &= _filePointer->good();
	return isValid;
}

Token Scanner::getNextToken()
{
	Token next;

	do{
		//skip all whitespace 
		next = scanNextToken();
	} while (next.getLexeme().getType() == MP_WHITESPACE);

	//only need to check identifiers
	//to see if they are reserved words
	if (next.getLexeme().getType() == MP_IDENTIFIER)
		checkReserved(next);

	return next;
}

Token Scanner::scanNextToken()
{
	//switch here
	Token nextToken;
	int count = 0;

	nextToken = FiniteStateAutomaton::greaterThan(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::equals(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::backslash(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::period(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::endOfFile(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::lessThan(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::roundLeftBracket(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::roundRightBracket(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::plus(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::colon(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::scolon(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::comma(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::minus(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::times(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::number(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::identifier(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::stringLiteral(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::comment(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	nextToken = FiniteStateAutomaton::whiteSpace(_filePointer, _currentRow, _currentColumn);
	if (nextToken.hasValidLexeme()) return nextToken;

	//no valid lexeme was found
	//out put in a error token

	std::string name;
	name += _filePointer->get();
	return Token(MP_INVALID, name, _currentRow, _currentColumn);
	_currentColumn++;
}



void Scanner::checkReserved(Token& token)
{
	std::string lowerName = token.getLexeme().getValue();
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

	int index = indexInReservedWords(lowerName);
	if (index < 0){
		return;
	}

	//otherwise this is a reserved word and we should update the token accordingly
	Lexeme newLexeme((LexemeType)index, ReservedWords[index]);
	token.setLexeme(newLexeme);
}

const int Scanner::indexInReservedWords(const std::string& name) 
{
	//kick off the binary search of reserved words 
	return indexInReservedWordsHelper(name, 32, 16, 0);
}

const int Scanner::indexInReservedWordsHelper(const std::string& name, int top, int middle, int bottom)
{
	if (middle >= top){
		return -1;
	}

	if (name == ReservedWords[middle]){
		return middle;
	}

	if (name < ReservedWords[middle]){
		top = middle;
	}
	else{
		//dont need to do this more than once
		if (middle == bottom){
			return -1;
		}
		bottom = middle;
	}
	
	middle = (top - bottom) / 2  + bottom;

	return indexInReservedWordsHelper(name, top, middle, bottom);
}

