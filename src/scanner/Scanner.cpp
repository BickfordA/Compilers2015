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
	Token next = scanNextToken();
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
	//out put a error token

	std::string name;
	name += _filePointer->get();
	return Token(LexemeType::MP_INVALID, name, _currentRow, _currentColumn);
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
	int index = -1;
	for (int i = 0; i < 32; i++){
		if (ReservedWords[i] == name){
			return i;
		}
	}
	return -1;

	//return indexInReservedWordsHelper(name, 16, 32, 0);
}

const int Scanner::indexInReservedWordsHelper(const std::string& name, int location, int upperBound, int lowerBound)
{
	if (location >= upperBound){
		return -1;
	}
	if (location <= lowerBound){
		return -1;
	}
	if (name == ReservedWords[location]){
		return location;
	}

	if (name < ReservedWords[location]){
		upperBound = location;
		location -= ((upperBound - 1 - lowerBound) / 2);
	}
	else{
		//name > reservedWords[location]
		lowerBound = location;
		location += ((upperBound + 1 - lowerBound) / 2) - 1;
	}

	return indexInReservedWordsHelper(name, location, upperBound, lowerBound);
}

