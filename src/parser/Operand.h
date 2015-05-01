#ifndef OPERAND_H
#define OPERAND_H

#include "Resources.h"
#include "Token.h"

#include <string>

class Operand
{

protected:
	Operand(LexemeResources::DataType _dataType);
	
public:
	Operand();
	virtual ~Operand(){;}
	virtual bool onTopOfStack();
	virtual std::string getName();
	std::string address();
	LexemeResources::DataType type();
	void setType(LexemeResources::DataType);


protected:

	LexemeResources::DataType _dataType;
	std::string _address;
};


//This type of operand can be used to hold operands which
//should be on the stack ( the only case i can think of 
//where we will need these is for the the operands which
//are returned as the result of a previous operation
class StackOperand : public Operand
{
public:
	StackOperand(LexemeResources::DataType type);

	bool onTopOfStack();

};


//These operands will correspond to 
//variables or literal types
class LexemeOperand : public Operand
{
public:
	LexemeOperand(const Lexeme lexeme, LexemeResources::DataType type);

	std::string getName();

	Lexeme getLexeme() const;

protected:
	const Lexeme _lexeme; 

};


class CommandOperand : public Operand
{
public:
	CommandOperand(std::string command, LexemeResources::DataType type = LexemeResources::UnknownData);

	std::string getCommand() const;
protected:

	const std::string _command;
};

#endif // OPERAND_H
