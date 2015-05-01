#ifndef TOKEN_STREAM
#define TOKEN_STREAM

#include "Token.h"
#include "Resources.h"

#include <list>

class TokenStream
{
public:

	bool moveAhead();
	
	
	bool nextToken(Token& nextToken) const;
	bool secondNextToken(Token& nextToken) const;
	LexemeResources::LexemeType nextTokenType() const;
	
	void addToken(Token newToken);

	Lexeme currentLexeme();  //used in generating semantic records

protected:

	std::list<Token> _toRead;
};


#endif // TOKEN_STREAM