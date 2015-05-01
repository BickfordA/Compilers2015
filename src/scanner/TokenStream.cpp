#include "TokenStream.h"

using namespace LexemeResources;
using namespace std;

bool TokenStream::moveAhead()
{
	if (_toRead.empty())
		return false;
	_toRead.pop_front();
	return true;
}

bool TokenStream::nextToken(Token& next) const
{
	if (_toRead.empty())
		return false;

	next = _toRead.front();
	return true;
}

bool TokenStream::secondNextToken(Token& next) const
{
	if (_toRead.size() < 2){
		return false;
	}
	
	list<Token>::const_iterator t = _toRead.begin();
	t++;
	next = *t;

	return true;
}

LexemeType TokenStream::nextTokenType() const
{
	if (_toRead.empty())
		return MP_ERROR;
	return _toRead.front().getType();
}

Lexeme TokenStream::currentLexeme()
{
	if (_toRead.empty())
	{
		return Lexeme();
	}
	return _toRead.front().getLexeme();
}

void TokenStream::addToken(Token newToken)
{
	_toRead.push_back(newToken);
}