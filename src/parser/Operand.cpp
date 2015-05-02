#include "Operand.h"

#include <assert.h>

using namespace std;
using namespace LexemeResources;

Operand::Operand(DataType type)
	: _dataType(type)
{
}

Operand::Operand()
	: _dataType(UnknownData)
{
}

bool Operand::onTopOfStack()
{ 
	return false;
}

string Operand::getName()
{
	assert(false);
	return string();
}

DataType Operand::type()
{
	return _dataType;
}

void Operand::setType(LexemeResources::DataType myType)
{
	_dataType = myType;
}

string Operand::address()
{
	return _address;
}

//-------------
// StackOperand
//-------------
StackOperand::StackOperand(DataType type)
	:Operand(type)
{
}

bool StackOperand::onTopOfStack()
{
	//this better be the case
	//this is why we have this class
	return true; 
}

void StackOperand::setName(string name)
{
	_name = name;
}

string StackOperand::getName()
{
	return _name;
}

// --------------
// LexemeOperand
// --------------
LexemeOperand::LexemeOperand(const Lexeme lexeme, DataType type)
	:_lexeme(lexeme), Operand(type)
{
}

string LexemeOperand::getName()
{
	return _lexeme.getValue();
}

Lexeme LexemeOperand::getLexeme() const
{
	return _lexeme;
}

// --------------
// CommandOperand
// --------------

CommandOperand::CommandOperand(string command, DataType type)
	:Operand(type), _command(command)
{
}

string CommandOperand::getCommand() const
{
	return _command;
}
