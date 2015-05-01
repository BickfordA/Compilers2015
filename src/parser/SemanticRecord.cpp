#include "SemanticRecord.h"
#include "SemanticAnalyzer.h"
#include <assert.h>

using namespace LexemeResources;
using namespace std;


SemanticRecord::~SemanticRecord()
{
	while (_identifiers.size()){
		delete _identifiers.front();
		_identifiers.pop_front();
	}
}

void SemanticRecord::addOperands(SemanticRecord& other)
{
	while (other.size()){
		addOperand(other.getNextOperandPointer());
	}
}

void SemanticRecord::addOperand(Lexeme id, DataType type)
{
	LexemeOperand * addOP = new LexemeOperand(id, type);
	_identifiers.push_back(addOP);
}

void SemanticRecord::addOperand(CommandOperand operand)
{
	CommandOperand* cmdOp = new CommandOperand(operand);
	_identifiers.push_back(cmdOp);
}

void SemanticRecord::addOperand(StackOperand operand)
{
	StackOperand* stkOp = new StackOperand(operand);
	_identifiers.push_back(stkOp);

}

void SemanticRecord::addOperand(Operand* operand)
{
	_identifiers.push_back(operand);
}

Operand* SemanticRecord::getNextOperandPointer()
{
	Operand* nextOp = NULL;

	if (!_identifiers.empty()){
		nextOp = _identifiers.front();
		_identifiers.pop_front();
	}
	if (!nextOp){
		throw SemanticAnaylserException("Pushing invalid symbol.");
	}
	return nextOp;
}

Operand SemanticRecord::getNextOperand()
{
	Operand* nextOp = NULL;

	if (!_identifiers.empty()){
		nextOp = _identifiers.front();
		_identifiers.pop_front();
	}
	else{
		throw SemanticAnaylserException("Getting next operand, when there isnt one");
	}

	return *nextOp;
}

LexemeOperand SemanticRecord::getNextOperandAsLexeme()
{
	LexemeOperand* nextOp = NULL;

	if (!_identifiers.empty()){
		nextOp = dynamic_cast<LexemeOperand*>( _identifiers.front());
		_identifiers.pop_front();
	}
	else{
		throw SemanticAnaylserException("Getting next operand, when there isnt one");
	}

	if (nextOp)
		return *nextOp;

	return LexemeOperand(Lexeme(), UnknownData);
}

CommandOperand SemanticRecord::getNextOperandAsCommand()
{
	CommandOperand* nextOp = NULL;

	if (!_identifiers.empty()){
		nextOp = dynamic_cast<CommandOperand*>(_identifiers.front());
		_identifiers.pop_front();
	}
	assert(nextOp);

	if (nextOp)
		return *nextOp;
	
	return CommandOperand("", UnknownData);
}


Operand SemanticRecord::showNextOperand()
{
	Operand* nextOp;

	if (!_identifiers.empty()){
		nextOp = _identifiers.front();
	}

	// we still own this
	return Operand(*nextOp);
}

int SemanticRecord::size() const
{
	return _identifiers.size();
}


void SemanticRecord::setType(DataType type)
{ 
	//need to iterate through the list and reset all of the types

	std::list<Operand*>::const_iterator iterator;
	for (iterator = _identifiers.begin(); iterator != _identifiers.end(); ++iterator) {
		Operand* myOP = *iterator;
		myOP->setType(type);
	}
}

void SemanticRecord::convertTypesToAddresses()
{
	for (Operand* op: _identifiers){
		op->setType(dataToAddressType(op->type()));
	}
}

DataType SemanticRecord::dataToAddressType(DataType type)
{
	switch (type)
	{
	case IntData:
		return AddressInt;
	case FloatData:
		return AddressFloat;
	case StringData:
		return AddressString;
	case BoolData:
		return AddressBool;
	default:
		return type;
	}
}