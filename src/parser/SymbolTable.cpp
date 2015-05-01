#include "SymbolTable.h"

#include "Symbol.h"

#include "Resources.h"


using namespace std;
using namespace LexemeResources;


SymbolTable::SymbolTable(Lexeme lexeme, LexemeResources::DataType type, int level, SymbolTable* parent)
	:Symbol(lexeme, type, level)
{
	_currentOffset = 0;
	_parentTable = parent;
	_activeChildTable = NULL;
	_size = 0;
}

SymbolTable* SymbolTable::createTable(Lexeme lexeme, LexemeResources::DataType type)
{
	int childLevel = _level + 1;
	SymbolTable* childTable = new SymbolTable(lexeme, type, childLevel, this);
	
	return childTable;
}

SymbolTable* SymbolTable::closeTable(int label)
{
	///size the size of the symbol table so this 
	//symbol will have the correct size
	//when it is refered to

	unordered_map<string, Symbol>::const_iterator symbolIt = _symbolLookup.begin();
	while (symbolIt != _symbolLookup.end()){
		_size += symbolIt->second.size();
		symbolIt++;
	}

	//clear the current entries in the table
	_symbolLookup.clear();

	if (_parentTable){
		Symbol newSymbol(_lexeme, _dataType, _level, 0, _size);
		newSymbol.setLabel(label);
		newSymbol.setArgumentTypes(this->argumentTypes());
		newSymbol.setProd(this->funProd());
		newSymbol.setFun(this->function());
		//add itself to the parent table
		_parentTable->insert(newSymbol);
	}


	return _parentTable;
}

void SymbolTable::insertArgument(const Lexeme lex, const int offset, const DataType type)
{ 
	_argumentTypes.push_back(type);

	int size = 1;
	Symbol newSymbol(lex, type, _level, offset, size);

	//increment size but not offset
	_size += size;

	_symbolLookup[lex.getValue()] = newSymbol;
}

void SymbolTable::insert(const Lexeme lex,const DataType type)
{
	//size is always 1 right?!?
	int size = 1;
	Symbol newSymbol(lex, type, _level, _currentOffset, size);
	_currentOffset += size;
	_size += size;

	_symbolLookup[lex.getValue()] =  newSymbol;
}

void SymbolTable::insert(Symbol tableSymbol)
{
	tableSymbol.setOffset(_currentOffset);
	_currentOffset += tableSymbol.size();
	_symbolLookup[tableSymbol.lexeme().getValue()] = tableSymbol;
	_size = _currentOffset;
}

const Symbol SymbolTable::lookUpAtLevel(const string name, bool& outFound)
{
	outFound = true;

	try{
		return _symbolLookup.at(name);
	}
	catch (out_of_range){
		outFound = false;

		return Symbol();
	}
}

const Symbol SymbolTable::lookup(const string name, bool& outFound)
{
	outFound = true;
	
	if (name == _lexeme.getValue()){
		Symbol thisSym(*this);
		if (this->function())
			thisSym.setOffset(-11);
		thisSym.setFun(this->function());
		thisSym.setProd(this->funProd());
		return thisSym;
	}
	
	try{
		return _symbolLookup.at(name);
	}
	catch(out_of_range){
		outFound = false;
		if (_parentTable){
			//if it wasnt found at this nesting level
			//check the parent table to see if 
			//it is defined there
			return _parentTable->lookup(name, outFound);
		}
		else{
			return Symbol();
		}
	}
}

void SymbolTable::printTable()
{
	//if (_symbolLookup.empty())
		//return;
	std::cout
		<< "\nCurrent Table Name: " << lexeme().getValue()
		<< "\nCurrent Table Level: " << level()
		<< "\nCurrent Table Size : " << size();
		//<< "\nCurrent Table Offset: " << offset()
		//<< "\n";

	std::cout << "\nSymbol Name:\tDataType: \tOffset: \tSize: \n";

	for (auto& x : _symbolLookup)
		std::cout << x.first << "\t\t" << x.second.dataType() << "\t\t" << x.second.offset() << "\t\t" << x.second.size() << std::endl;
}

int SymbolTable::allocSize()
{ 
	unordered_map<string, Symbol>::const_iterator sym = _symbolLookup.cbegin();
	int allocSize = 0; //need one for each non argument type
	while (sym != _symbolLookup.cend()){
		if (!DataIsAddress(sym->second.dataType())){
			allocSize++;
		}
		sym++;
	}
	return allocSize;
}