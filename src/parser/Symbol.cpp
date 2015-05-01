#include "Symbol.h"

using namespace LexemeResources;
using namespace std;


Symbol::Symbol()
	: _dataType(UnknownData), _level(-1), _offset(-1), _size(-1), _isFun(false), _isProc(false)
{
}

Symbol::Symbol(Lexeme lex, LexemeResources::DataType dataType, int level, int offset, int size, list<DataType> arguments)
	: _lexeme(lex), _dataType(dataType), _level(level), _offset(offset), _size(size), _argumentTypes(arguments), _isFun(false), _isProc(false)
{
}

Lexeme Symbol::lexeme() const
{
	return _lexeme;
}

DataType Symbol::dataType() const
{
	return _dataType;
}

int Symbol::level() const
{
	return _level;
}

int Symbol::offset() const
{
	return _offset;
}

int Symbol::size() const
{
	return _size;
}

void Symbol::setOffset(int offset)
{
	_offset = offset;
}

list<DataType> Symbol::argumentTypes() const
{
	return _argumentTypes;
}

int Symbol::label() const
{
	return _label;
}

void Symbol::setLabel(int label)
{
	_label = label;
}

void Symbol::setArgumentTypes(list<DataType> newArguments)
{ 
	_argumentTypes = newArguments;
}

void Symbol::setFun(bool isFun)
{
	_isFun = isFun;
}

bool Symbol::function()
{
	return _isFun;
}

void Symbol::setProd(bool isProd)
{
	_isProc = isProd;
}

bool Symbol::funProd()
{
	return _isFun || _isProc;
}

