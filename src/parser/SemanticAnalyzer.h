#ifndef SEMANTICANALYSER_H
#define SEMANTICANALYSER_H

#include "Resources.h"
#include "Token.h"
#include "Symbol.h"
#include "SymbolTable.h"

#include "Operand.h"
#include "SemanticRecord.h"

#include <iostream>
#include <fstream>


struct MachineVal
{
	MachineVal(std::string inValue = "", LexemeResources::DataType inType = LexemeResources::UnknownData)
		:type(inType), value(inValue)
	{
		value = inValue;
		type = inType;
	}

	std::string value; //address or literal value
	LexemeResources::DataType type;
};

//this class will keep track of 
//the current instance of the symbol table
//as well as current semantic actions
class SemanticAnalyser
{

public:
	SemanticAnalyser();
	~SemanticAnalyser();

	bool createTable(LexemeOperand operand);
	bool createTable(const Lexeme token, LexemeResources::DataType type);
	void closeTable(bool deleteEntry= false); 

	bool insertSymbol(const Lexeme lex, LexemeResources::DataType type);
	bool insertArgument(const Lexeme lex, const int offset, const LexemeResources::DataType type);

	bool insertSymbol(SemanticRecord& record);
	//bool insertArgument(SemanticRecord& record);
	

	const Symbol lookupSymbol(std::string name, bool& found);
	std::string lookupSymbolAddress(std::string name, bool& found, LexemeResources::DataType& outType);
	
	std::string stringLitToVal(std::string value);
	std::string intLitToVal(std::string value);
	std::string floatLitToVal(std::string value);
	
	LexemeResources::DataType checkType(LexemeOperand* operand);
	MachineVal generateMachineValue(Lexeme lex);


	void printCurrentTable();
	std::string errorMsg();


	void programHeading(int& beginLabel);
	void programTail();

	//call this after the booleanExpression has
	//been evaluated and is on top of the stack
	void ifStatementBegin(int& nextLabel);
	void ifStatementElse(int currentLabel, int& exitLabel);
	void ifStatementEnd(int lastLabel);

	//while statement
	void whileStatementPrecondition(int& repeatLabel);
	void whileStatementPostcondition(int& endLabel);
	void whileStatementPostbody(int repeatLabel, int exitLoop);

	//repeat statement
	void repeatBegin(int& repeatStart);
	void repeatExit(int repeatStart);

	//for statement
	void forBegin(int& beginCondition, int& exitLoop, SemanticRecord& controlVars);
	void forEndBody(int loopAgain, int exitLoop, SemanticRecord& intialRecord);

	std::list<LexemeResources::DataType> arguments(LexemeOperand lexop);

	Operand* funCall(SemanticRecord& id, SemanticRecord& args);
	void prodCall(SemanticRecord& id, SemanticRecord& args);

	void generateActivationRecord(int beginRecord);

	void insertArguments(SemanticRecord& inputArgs);

	void functionHeading(int& beginLabel);
	void functionEnd();

	void procedureHeading(int& beginLabel);
	void procedureEnd();


	void writeList(SemanticRecord& writeSymbols, bool writeLn = true);

	//The command is in the form
	//[Command] [Argument] 
	void unaryPrefixCommand(SemanticRecord& infixSymbols);
	


	//The command are in the order they are encountered
	//ie A + B 
	// Operand Command Operand
	StackOperand infixStackCommand(SemanticRecord& infixSymbols);

	StackOperand pushAddress(Lexeme lex, LexemeResources::DataType);

	StackOperand push(Lexeme lex, LexemeResources::DataType type = LexemeResources::UnknownData);

	void writeCommand(const std::string command);

private:
	//if we are in a function call dont imediatly push all values to the stack 
	bool _inFunctionCall;

	//these are some helper functions so i can be lazy in creating
	//the functions for writing command
	Operand twoValueCommand(const std::string command, SemanticRecord records);

	//if we have methods which call the uMachine instructions
	//we can be sneeky and add comments which describe the 
	//value that we are working with, this will make 
	//debugging the uMachine code alot easier.
	void push(Operand* val, const LexemeResources::DataType castType = LexemeResources::UnknownData);
	void cast(LexemeResources::DataType valType, LexemeResources::DataType toType);
	

	//file pointer
	std::ofstream _outFile;

	//////////////
	//fun ways to generate errors
	//for missing objects
	void missingObject(const Lexeme lex);
	//for namespace collisions
	void symbolCollisionError(const Token lex);
	std::list<std::string> _errList;
	
	SymbolTable * _currentTable;

	int getNextLabelVal();
	int _labelCounter;
};



#endif //SEMANTICANALYSER_H