#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "Token.h"
#include "TokenStream.h"
#include "Resources.h"
#include "SemanticRecord.h"
#include "SemanticAnalyzer.h"
#include "SemanticRecord.h"
#include "Lexeme.h"

#include <string>
#include <list>
#include <iostream>

struct ParseException : public std::runtime_error
{
	ParseException(std::string const& message)
		: std::runtime_error(message)
	{
	}
};

class Grammar
{

public:

	std::string getError();

	void setTokenStream(TokenStream* tokens);

	void printLog();
	SemanticAnalyser* _semanticAnalyser;

private:
	//TODO: need to decide if we will use this
	//SemanticOrganizer* _semanticOrganizer;


	bool match();
	void logRule(int rule);

	//dont need to pass any other info as we can get it from the 
	//current token, just give what the expected token should have been
	void error(std::string expectedTokenNames);
	void error(LexemeResources::TypeList espectedTypes);

	std::list<std::string> _errStrings;

	//the list of current tokens
	TokenStream* _currentTokens;

	//Syntactic Sugar... mmmhhhmm
	LexemeResources::LexemeType nextTokenType() const;
	Token  nextToken() const;

	//a list of production rules logged while parsing
	std::list<int> _ruleLog;

	//access to the current Token's lexeme for semantic records
	Lexeme currentLexeme();


public:
	bool procedureAssignmentAmbiguity();
	 
	 bool factor(SemanticRecord& record);
	 bool forStatement(); 
	 bool multiplyingOperator(SemanticRecord& record); 
	 /*need to keep track of when not to push (when passing var)*/
	 bool actualParameter(SemanticRecord& params, LexemeResources::DataType type);
	 bool actualParameterTail(SemanticRecord& params, std::list<LexemeResources::DataType>& argumentTypes);
	 bool addingOperator(SemanticRecord& record); 
	 bool assignmentStatement();
	 bool block(int begin); 
	 bool booleanExpression(SemanticRecord& record); 
	 bool compoundStatement(); 
	 bool controlVariable(SemanticRecord& record);
	 bool emptyStatement(); 
	 bool expression(SemanticRecord& record); 
	 bool factorTail(SemanticRecord& record); 
	 bool finalValue(SemanticRecord& record); 
	 bool formalParameterSection(SemanticRecord& record);
	 bool formalParameterSectionTail(SemanticRecord& record); 
	 bool functionDeclaration();  // close function SymbolTable  
	 bool functionHeading();   // create function SymbolTable, add function parameters symbols
	 bool functionIdentifier(SemanticRecord& record); 
	 bool identifierList(SemanticRecord& record); 
	 bool identifierTail(SemanticRecord& record); 
	 bool ifStatement();
	 bool initialValue(SemanticRecord& record);  
	 bool optionalActualParameterList(SemanticRecord& parameters, std::list<LexemeResources::DataType>& argumentTypes);  
	 bool optionalElsePart(); 
	 bool optionalFormalParameterList(SemanticRecord& record); 
	 bool optionalRelationalPart(SemanticRecord& record); 
	 bool optionalSign(bool&); 
	 bool ordinalExpression(SemanticRecord& record); 
	 bool procedureAndFunctionDeclarationPart(); 
	 bool procedureDeclaration();  //Close procedure SymbolTable
	 bool procedureHeading();   //Create procedure SymbolTable
	 bool procedureIdentifier(SemanticRecord& record); 
	 bool procedureStatement();
	 bool program();  //Close program SymbolTable 
	 bool programHeading();  //Create program SymbolTable
	 bool programIdentifier(SemanticRecord& record); 
	 bool readParameter(); 
	 bool readParameterTail(); 
	 bool readStatement();
	 bool relationalOperator(SemanticRecord& record); 
	 bool repeatStatement();
	 bool simpleExpression(SemanticRecord& record); 
	 bool statement(SemanticRecord& record); 
	 bool statementPart(); 
	 bool statementSequence(); 
	 bool statementTail();
	 bool stepValue(SemanticRecord& record); 
	 bool systemGoal(); 
	 bool term(SemanticRecord& record); 
	 bool termTail(SemanticRecord& record); 
	 bool type(SemanticRecord& record); 
	 bool valueParameterSection(SemanticRecord& record); 
	 bool variableDeclaration();   // add symbols to current symbol tables
	 bool variableDeclarationPart(); 
	 bool variableDeclarationTail(); 
	 bool variableIdentifier(SemanticRecord& record); 
	 bool variableParameterSection(SemanticRecord& record); 
	 bool whileStatement(); 
	 bool writeParameter(SemanticRecord& record); 
	 bool writeParameterTail(SemanticRecord& record); 
	 bool writeStatement();

};

#endif