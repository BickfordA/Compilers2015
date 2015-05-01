#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "Symbol.h"

#include "Resources.h"

#include <string>
#include <unordered_map>
#include <list>
#include <iostream>

class SymbolTable : public Symbol
{

public:
	SymbolTable(Lexeme lexeme,  LexemeResources::DataType type, int level, SymbolTable* parent = NULL);

	//create a new table and return 
	// a pointer to the child table
	SymbolTable* createTable(Lexeme lexeme, LexemeResources::DataType type);

	//close the current table and return
	// a pointer to the parent table
	SymbolTable* closeTable(int label);

	void insert(const Lexeme lex, const LexemeResources::DataType type);
	void insertArgument(const Lexeme lex,const int offset, const LexemeResources::DataType type);

	const Symbol lookUpAtLevel(const std::string name, bool& outFound);
	const Symbol lookup(const std::string name, bool& outFound);

	void printTable();

	int allocSize();

private:
	//this should only be called when closing a table 
	void insert(Symbol tableSymbol);

	int _currentOffset;

	SymbolTable * _parentTable;
	SymbolTable * _activeChildTable;

	std::unordered_map<std::string, Symbol> _symbolLookup;
};


#endif //SYMBOL_TABLE