#include "FiniteStateAutomaton.h"

#include <iostream>

bool FiniteStateAutomaton::charIsWhiteSpace(char c)
{
	if (c == ' ') return true;
	if (c == '\t') return true;
	if (c == '\n') return true;

	return false;
}

bool FiniteStateAutomaton::charIsDigit(char c)
{
	//48 -57
	if ((int)c > 47 && (int)c < 58)
		return true;
	
	return false;
}

bool FiniteStateAutomaton::charIsUpperAlphabet(char c)
{
	if ((int) c > 64 && (int)c < 91)
		return true;
	return false;
}

bool FiniteStateAutomaton::charIsLowerAlphabet(char c)
{
	//97 - 122
	if (c > 96 && c < 123)
		return true;

	return false;
}

Token FiniteStateAutomaton::singleCharFSA(istream* stream, char c, Lexeme::LexemeType type, int& line, int& currentColumn)
{
	char next;
	string name;
	//Start State
	{
		next = stream->peek();
		if (next == c){ //transition
			name += stream->get();
			currentColumn++;
			goto Accept;
		}
		goto Reject;
	}
Accept:
	{
		return Token(type, name, line, currentColumn);
	}
Reject:
	{
		return Token();
	}
}

Token FiniteStateAutomaton::greaterThan(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;
//Start State
	{
		//look without taking
		next = stream->peek();
		if (next == '>'){ //transition
			//move ahead next char
			name += stream->get(); //we already know it is a '>'
			currentColumn++;
			goto GreaterThan;
		}
		//default condition
		goto Reject;
	}

GreaterThan:
	{
		next = stream->peek();
		if (next == '='){ //transition
			//take the '='
			name += stream->get();
			currentColumn++;
			goto GreaterThanOrEqual;
		}
		//accept greater than
		return Token(Lexeme::LexemeType::MP_GTHAN, name, line, currentColumn);
	}

GreaterThanOrEqual:
	{
		//can go no further
		return Token(Lexeme::LexemeType::MP_GEQUAL, name, line, currentColumn);
	}

Reject:
	{
		//nothing here, return default init token (invalid)
		return Token();
	}
}


Token FiniteStateAutomaton::lessThan(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;
	//Start State
	{
		//look without taking
		next = stream->peek();
		if (next == '<'){ //transition
			//move ahead next char
			name += stream->get(); //we already know it is a '<'
			currentColumn++;
			goto LessThan;
		}
		//default condition
		goto Reject;
	}

LessThan:
	{
		next = stream->peek();
		if (next == '='){ //transition
			//take the '='
			name += stream->get();
			currentColumn++;
			goto LessThanOrEqual;


		}

		if (next == '>'){//transition
			//take the '>'
			name += stream->get();
			currentColumn++;
			goto Negation;

		}
		//accept less than
		return Token(Lexeme::LexemeType::MP_LTHAN, name, line, currentColumn);
	}

LessThanOrEqual:
	{
		//can go no further
		return Token(Lexeme::LexemeType::MP_LEQUAL, name, line, currentColumn);
	}

Negation:
	{
		//accept negation
		return Token(Lexeme::LexemeType::MP_NEQUAL, name, line, currentColumn);
	}
Reject:
	{
		//nothing here, return default init token (invalid)
		return Token();
	}
}

Token FiniteStateAutomaton::colon(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;

//Start State
	{
		//look without taking
		next = stream->peek();
		if (next == ':'){//transition
			//move ahead next character
			name += stream->get();
			currentColumn++;
			goto Colon;
		}
		//default
		goto Reject;
	}

Colon:
	{
		next = stream->peek();
		if (next == '='){
			name += stream->get();
			currentColumn++;
			goto Assign;
		}
		//accept colon
		return Token(Lexeme::LexemeType::MP_COLON, name, line, currentColumn);
	}

Assign:
	{
		//accept assign
		return Token(Lexeme::LexemeType::MP_ASSIGN, name, line, currentColumn);
	}

Reject:
	{
		return Token();
	}
}

Token FiniteStateAutomaton::equals(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '=', Lexeme::LexemeType::MP_EQUAL, line, currentColumn);
}

Token FiniteStateAutomaton::backslash(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '/', Lexeme::LexemeType::MP_FLOAT_DIVIDE, line, currentColumn);
}

Token FiniteStateAutomaton::period(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '.', Lexeme::LexemeType::MP_PERIOD, line, currentColumn);
}

Token FiniteStateAutomaton::plus(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '+', Lexeme::LexemeType::MP_PLUS, line, currentColumn);
}

Token FiniteStateAutomaton::roundLeftBracket(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '(', Lexeme::LexemeType::MP_LPAREN, line, currentColumn);
}

Token FiniteStateAutomaton::roundRightBracket(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, ')', Lexeme::LexemeType::MP_RPAREN, line, currentColumn);
}

Token FiniteStateAutomaton::scolon(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, ';', Lexeme::LexemeType::MP_SCOLON, line, currentColumn);
}

Token FiniteStateAutomaton::comma(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, ',', Lexeme::LexemeType::MP_COMMA, line, currentColumn);
}

Token FiniteStateAutomaton::minus(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '-', Lexeme::LexemeType::MP_MINUS, line, currentColumn);
}

Token FiniteStateAutomaton::times(istream* stream, int& line, int& currentColumn)
{
	return singleCharFSA(stream, '*', Lexeme::LexemeType::MP_TIMES, line, currentColumn);
}


Token FiniteStateAutomaton::endOfFile(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;
	if (stream->peek() == EOF){//then this is the end of the file
		next = stream->get();
		return Token(Lexeme::LexemeType::MP_EOF, name , line, currentColumn);
	}
	return Token();
}


Token FiniteStateAutomaton::identifier(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;

	Lexeme::LexemeType lastGoodType = Lexeme::LexemeType::MP_INVALID;
	int lastGoodPosition = stream->tellg();
	string temp;

//start state	
	next = stream->peek();

	if (charIsUpperAlphabet(next) || charIsLowerAlphabet(next))
	{
		name += stream->get();
		currentColumn++;
		goto identifier;
	}
	goto Reject;

	identifier:
	{
		next = stream->peek();

		if (charIsDigit(next) || charIsUpperAlphabet(next) || charIsLowerAlphabet(next))
		{
			name += stream->get();
			currentColumn++;
			goto identifier;
		}

		

		if (next == '_'){
			lastGoodPosition = stream->tellg();
			temp += stream->get();
			goto underscore;
		}
		lastGoodType = Lexeme::LexemeType::MP_IDENTIFIER;
		return Token(lastGoodType, name, line, currentColumn);
	}

underscore:
	{
		next = stream->peek();

		if (charIsDigit(next) || charIsUpperAlphabet(next) || charIsLowerAlphabet(next))
		{
			name += stream->get();
			currentColumn++;
			goto identifier;
		}

		stream->seekg(lastGoodPosition);
		return Token(lastGoodType, name, line, currentColumn);
	}
Reject:
	{
		if (lastGoodType == Lexeme::LexemeType::MP_INVALID){
			return Token();
		}
		stream->seekg(lastGoodPosition);
		return Token(lastGoodType, name, line, currentColumn);
	}
}

Token FiniteStateAutomaton::number(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;

	Lexeme::LexemeType lastGoodType = Lexeme::LexemeType::MP_INVALID;
	int lastGoodPosition = stream->tellg();
	string temp;

	//start state
	{
		next = stream->peek();
		if (charIsDigit(next)){
			name += stream->get();
			currentColumn++;
			goto IntegerLit;
		}
		goto Reject;
	}

IntegerLit://digit{digit}
	{
		next = stream->peek();

		lastGoodType = Lexeme::LexemeType::MP_INTEGER_LIT;

		if (charIsDigit(next)){
			name += stream->get();
			currentColumn++;
			goto IntegerLit;
		}

		if (next == '.'){
			lastGoodPosition = stream->tellg();
			temp += stream->get();
			goto FixedLit;
		}

		if (next == 'e' || next == 'E'){
			lastGoodPosition = stream->tellg();
			temp += stream->get();
			goto FloatLitE;
		}

		return Token(lastGoodType, name, line, currentColumn);
	}

FixedLit: //got here from a "." 
	{
		next = stream->peek();

		if (charIsDigit(next)){
			temp += stream->get();
			goto FixedLitTrailingInt;
		}
		
		goto Reject;
	}

FixedLitTrailingInt:
	{
		next = stream->peek();

		if (next == 'e' || next == 'E'){
			//fixedlit is done 
			name += temp;
			currentColumn += temp.size();
			lastGoodType = Lexeme::LexemeType::MP_FIXED_LIT;
			temp.clear();

			//go for the float lit
			lastGoodPosition = stream->tellg();
			temp += stream->get(); //get the 'e'
			goto FloatLitE;
		}

		name += temp;
		currentColumn += temp.size();

		return Token(Lexeme::LexemeType::MP_FIXED_LIT, name, line, currentColumn);
	}

FloatLitE:
	{
		next = stream->peek();
		
		if (next == '+' || next == '-'){
			temp += stream->get();
			goto FloatLitPlusMinus;
		}

		goto Reject;
	}

FloatLitPlusMinus:
	{
		next = stream->peek();

		if (charIsDigit(next)){
			temp += stream->get();
			goto FloatLitTrailingInt;
		}

		goto Reject;
	}

FloatLitTrailingInt:
	{
		next = stream->peek();

		if (charIsDigit(next)){
			temp += stream->get();
			goto FloatLitTrailingInt;
		}

		name += temp;
		currentColumn += temp.size();
		return Token(Lexeme::LexemeType::MP_FLOAT_LIT, name, line, currentColumn);
	}

Reject:
	{
		if (lastGoodType == Lexeme::LexemeType::MP_INVALID){
			return Token();
		}
		stream->seekg(lastGoodPosition);
		return Token(lastGoodType, name, line, currentColumn);	
	}

}

//string literal FSA stub
Token FiniteStateAutomaton::stringLiteral(istream* stream, int& line, int& currentColumn)
{
	char next;
	string name;

	Lexeme::LexemeType lastGoodType = Lexeme::LexemeType::MP_INVALID;
	int lastGoodPosition = stream->tellg();

//start state
	{
		next = stream->peek();
		if (next == '\''){
			name += stream->get();
			currentColumn++;
			goto OddApostrophes;
		}
		//if you don't see an apostrophe
		goto Reject;
	}

OddApostrophes:  //this is a non-accept state looking for more characters in the string literal
	{
		next = stream->peek();
		if (next == '\''){
			//if you see another apostrophe
			name += stream->get();
			currentColumn++;
			goto EvenApostrophes;
		}
		
		if (next == '\n'){
			//if you encounter EOL in open string you return MP_RUN_STRING error and set stream position back?
			lastGoodType = Lexeme::LexemeType::MP_RUN_STRING;
			goto Reject;
		}

		//otherwise add character to the string literal and keep looking
		name += stream->get();
		currentColumn++;
		goto OddApostrophes;
	}

EvenApostrophes:  //this is a state that checks for more apostrophes, otherwise accepts end of string
	{
		next = stream->peek();
		if (next == '\''){
			//if you see another apostrophe
			name += stream->get();
			currentColumn++;
			goto OddApostrophes;
		}

		//otherwise you have a valid string literal
		return Token(Lexeme::LexemeType::MP_STRING_LIT, name, line, currentColumn);
	}

Reject:
	{
		if (lastGoodType == Lexeme::LexemeType::MP_INVALID){
			return Token();
		}
		stream->seekg(lastGoodPosition);
		return Token(lastGoodType, name, line, currentColumn);
	}


	
}