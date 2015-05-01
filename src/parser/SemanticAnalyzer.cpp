#include "SemanticAnalyzer.h"

//use this to turn on and off comments in the 
//written uMahine code
#define ENABLE_DEBUGGING true

#include <assert.h>

using namespace std;
using namespace LexemeResources;



SemanticAnalyser::SemanticAnalyser()
{
	_currentTable = NULL;
	_outFile.open("compiledUCode.txt");
	_labelCounter = 0;
}

SemanticAnalyser::~SemanticAnalyser()
{
    _outFile.flush();
	_outFile.close();
}

bool SemanticAnalyser::createTable(LexemeOperand operand)
{
	LexemeOperand* lexOp = dynamic_cast<LexemeOperand*>(&operand);
	if (lexOp){
		return createTable(lexOp->getLexeme(), lexOp->type());
	}
	return false;
}

bool SemanticAnalyser::createTable(const Lexeme lexeme, DataType type)
{
	//make sure that the table has been initialized
	if (!_currentTable){
		_currentTable = new SymbolTable(lexeme, type, 0);
		return true;
	}

	bool found = false;
	const Symbol match = _currentTable->lookup(lexeme.getValue(), found);

	if (found)
	{//there is already an entry with this name
		symbolCollisionError(Token(lexeme, -1, -1));
		return false;
	}

	int nextLabel = getNextLabelVal();
	writeCommand("L" + to_string(nextLabel) + ":");
	_currentTable = _currentTable->createTable(lexeme, type);
	_currentTable->setLabel(nextLabel);
	return true;
}

void SemanticAnalyser::closeTable(bool deleteEntry)
{

	SymbolTable* parent = _currentTable->closeTable(_currentTable->label());
	if (parent){
		_currentTable = parent;
		return;
	}
	else{
		//close the table if it is the last one
		delete _currentTable;
	}
}


void SemanticAnalyser::setTableAsFunction()
{
	_currentTable->setFun(true);
}

void SemanticAnalyser::setTableAsProcedure()
{
	_currentTable->setProd(true);
}

//bool SemanticAnalyser::insertSymbol(const Token token)
//{
//	DataType type = UnknownData;
//
//	switch (token.getLexeme().getType())
//	{
//	case MP_INTEGER:
//		type = IntData;
//		break;
//	case MP_FLOAT:
//		type = FloatData;
//		break;
//	case MP_STRING:
//		type = StringData;
//	default:
//		type = UnknownData;
//	}
//
//	//i think we may need to set the type manually for functions/procedures....
//	//i am not sure how we will get the reurn type
//
//	return insertSymbol(token, type);
//}
//
////bool SemanticAnalyser::insertSymbol(const Token token, DataType type)
//{
//	if (!_currentTable){
//		assert(false);
//		return false;//this should not happend;
//	}
//
//	bool found;
//	const Symbol foundSymbol = _currentTable->lookup(token.getLexeme().getValue(), found);
//
//	if (found){
//		symbolCollisionError(token);
//		return false;
//	}
//
//	_currentTable->insert(token.getLexeme(), type);
//	return true;
//}

bool SemanticAnalyser::insertSymbol(const Lexeme lex, DataType type)
{
	if (!_currentTable){
		assert(false);
		return false;//this should not happend;
	}

	bool found;
	const Symbol foundSymbol = _currentTable->lookup(lex.getValue(), found);

	if (found){
		symbolCollisionError(Token(lex, -1, -1));
		return false;
	}

	_currentTable->insert(lex, type);
	return true;
}

bool SemanticAnalyser::insertArgument(const Lexeme lex, const int offset, const DataType type)
{
	if (!_currentTable){
		assert(false);
		return false;//this should not happend;
	}

	bool found;
	const Symbol foundSymbol = _currentTable->lookUpAtLevel(lex.getValue(), found);


	if (found){
		symbolCollisionError(Token(lex, -1, -1));
		return false;
	}

	_currentTable->insertArgument(lex, offset, type);
	return true;

}

bool SemanticAnalyser::insertSymbol(SemanticRecord& record)
{
	while (record.size() > 0 )
	{
		if (record.showNextOperandAs<LexemeOperand>()){
			LexemeOperand* nextOp = record.showNextOperandAs<LexemeOperand>();
			assert(nextOp);
			insertSymbol(nextOp->getLexeme(), nextOp->type());
		}
		else{
			assert(false);
			return false;
		}
		delete record.getNextOperandPointer();
	}
	return true;
}

//bool SemanticAnalyser::insertArgument(SemanticRecord& record)
//{
//	for (int i = 0; i < record.size(); i++)
//	{
//		if (record.showNextOperandAs<LexemeOperand>()){
//			LexemeOperand* nextOp = record.showNextOperandAs<LexemeOperand>();
//			assert(nextOp);
//			insertSymbol(nextOp->getLexeme(), nextOp->type());
//		}
//		else{
//			assert(false);
//			return false;
//		}
//		delete record.getNextOperandPointer();
//	}
//	return true;
//}

const Symbol SemanticAnalyser::lookupSymbol(string name, bool& found)
{
	if (!_currentTable){
		assert(false);
		found = false;
		return Symbol();
	}

	return _currentTable->lookup(name, found);
}

string SemanticAnalyser::lookupSymbolAddress(string name, bool& found, DataType& outType)
{
	Symbol matchingSymbol = lookupSymbol(name, found);
	outType = matchingSymbol.dataType();

	if (!found){
		return string();
	}

	char address[64] = { 0 };
	sprintf(address, "%d(D%d)", matchingSymbol.offset(), matchingSymbol.level());
	string addressString(address);

	return addressString;
}

string SemanticAnalyser::stringLitToVal(string value)
{
	return "#\"" + value + "\"";
}

string SemanticAnalyser::intLitToVal(string value)
{
	return "#" + value;
}

string SemanticAnalyser::floatLitToVal(string value)
{
	return "#" + value;
}

LexemeResources::DataType SemanticAnalyser::checkType(LexemeOperand* lex)
{
	if(!lex){
		return UnknownData;
	}
	
	MachineVal tempVal;
	
	tempVal = generateMachineValue(lex->getLexeme());
	
	return tempVal.type;
}

MachineVal SemanticAnalyser::generateMachineValue(Lexeme lex)
{
	//figure out if we need to look up the value
	//of if the value is a literal and we
	//can pass in the value directly 

	DataType type;
	string machineVal;
	bool found = true;
	switch (lex.getType())
	{
	case MP_IDENTIFIER:
	case MP_INTEGER:
	case MP_STRING:
	case MP_FLOAT:
	case MP_BOOLEAN:
		machineVal = lookupSymbolAddress(lex.getValue(), found, type);
		break;
	case MP_STRING_LIT:
		type = StringData;
		machineVal = stringLitToVal(lex.getValue());
		break;
	case MP_INTEGER_LIT:
		type = IntData;
		machineVal = intLitToVal(lex.getValue());
		break;
	case MP_FLOAT_LIT:
	case MP_FIXED_LIT:
		type = FloatData;
		machineVal = floatLitToVal(lex.getValue());
		break;
    case MP_TRUE:
        type = IntData;//there isnt a bool type in uMachine
        machineVal = "#1";
        break;
    case MP_FALSE:
        type = IntData;
        machineVal = "#0";
        break;
	case MP_TO:
		type = IntData;
		machineVal = "#1";
		break;
	case MP_DOWNTO:
		type = IntData;
		machineVal = "#-1";
		break;
	default:
		assert(false);
		//this shouldnt happen
		//but i may have missed some
	}

	if (!found){
		missingObject(lex);
	}

	return MachineVal(machineVal, type);
}

void SemanticAnalyser::missingObject(const Lexeme lex)
{
	string err = "The corresponding symbol was not found for" + lex.getValue() + "\n";
	_errList.push_back(err);
}

void SemanticAnalyser::symbolCollisionError(const Token token)
{
	string err = "This variable has already been used: " + token.getLexeme().getValue() + " \nline:" + to_string(token.getLineNumber()) + " \ncol:" + to_string(token.getColumnNumber()) + "!!.\n";
	_errList.push_back(err);
	assert(false);
}

void SemanticAnalyser::printCurrentTable()
{
	if (!_currentTable)
		return;
	_currentTable->printTable();
}

void SemanticAnalyser::programHeading(int& beginLabel)
{
	beginLabel = getNextLabelVal();
	writeCommand("BR L" + to_string(beginLabel) + "");
}

void SemanticAnalyser::programTail()
{
	//deallocate and Halt!!!
	_outFile << "MOV D0 SP" << " \n";
	_outFile << "HLT" << " \n";
}

void SemanticAnalyser::ifStatementBegin(int& nextLabel)
{
	//if not true then branch to the next 
	//label, which should be the else or
	//the end of the if statement
	if (ENABLE_DEBUGGING)
		_outFile << "\n; Start 'if' statement \n";
	nextLabel = getNextLabelVal();
	_outFile << "BRFS L" << nextLabel  << " \n";

}

void SemanticAnalyser::ifStatementElse(int currentLabel, int&nextLabel)
{
	//end of true section jump to the end 
	nextLabel = getNextLabelVal();
	_outFile << "BR L" << nextLabel << "\n";


	//the if was false start here 
	_outFile << "L" << currentLabel << ":\n";

}

void SemanticAnalyser::ifStatementEnd(int currentLabel)
{
	_outFile << "L" << currentLabel << ":\n";
	if (ENABLE_DEBUGGING)
		_outFile << "; end 'if' statement \n\n";

}

void SemanticAnalyser::whileStatementPrecondition(int& repeatLabel)
{
	if (ENABLE_DEBUGGING)
		_outFile << "\n; Start 'while' statement \n";
	repeatLabel = getNextLabelVal();
	_outFile << "L" << repeatLabel << ":\n";
}

void SemanticAnalyser::whileStatementPostcondition(int& exitLabel)
{
	//if false exit
	exitLabel = getNextLabelVal();
	_outFile << "BRFS L" << exitLabel << " \n";
	if (ENABLE_DEBUGGING)
		_outFile << "; end 'while' condition \n";

	//exitLabel = getNextLabelVal();
	//_outFile << "L" << exitLabel << ":\n";
}

void SemanticAnalyser::whileStatementPostbody(int repeatLabel, int exitLabel)
{
	_outFile << "BR L" << repeatLabel << " \n";
	_outFile << "L" << exitLabel << ":\n";

	if (ENABLE_DEBUGGING)
		_outFile << "; end 'while' loop \n\n";
}

void SemanticAnalyser::writeList(SemanticRecord& writeSymbols, bool writeLn)
{
	while (writeSymbols.size()){
		Operand * nextOp = writeSymbols.getNextOperandPointer();
		push(nextOp);
		delete nextOp;
		_outFile << "WRTS " << " \n";
	}
	//TODO: this could be optimized to be inlcuded as the last command in the loop
	if (writeLn){
		_outFile << "WRTLN" << " \n";
	}
}

void SemanticAnalyser::repeatBegin(int& beginLabel)
{
	beginLabel = getNextLabelVal();
	_outFile << "L" << beginLabel << ":\n";
}

void SemanticAnalyser::repeatExit(int repeatLabel)
{
	//repeat condition will leave false on the stack if it fails
	//and we will continue

	_outFile << "BRFS L" << repeatLabel << "\n";
}

void SemanticAnalyser::forBegin(int& beginCondition,int& exitLoop, SemanticRecord& controlVars)
{
	//we need to allocate the space for the variables
	
	LexemeOperand lexOp1 = controlVars.getNextOperandAsLexeme();
	MachineVal initVal = generateMachineValue(lexOp1.getLexeme());

	//we dont use this for anything because this is on top of the stack
	Operand lexOp2 = controlVars.getNextOperand();
	//MachineVal assignedVal = generateMachineValue(lexOp2.getLexeme());

	LexemeOperand lexOp3 = controlVars.getNextOperandAsLexeme();
	MachineVal stepVal = generateMachineValue(lexOp3.getLexeme());
	//this should be a -1 or 1 depending on if
	//we are going up or down. 

	//this is on the stack as well so no neet to use it
	Operand lexOp4 = controlVars.getNextOperand();
	//MachineVal finalVal = generateMachineValue(lexOp4.getLexeme());

	controlVars.addOperand(new LexemeOperand( lexOp1));

	if (ENABLE_DEBUGGING)
		_outFile << "\n;         Begin 'for' loop\n";

	//int level = _currentTable->level() ;
	//
	//the intial value is on the stack and will be located at -1 offset current level
	_outFile << "PUSH " << initVal.value << " \n"; //for current value

	//set SP for offset into control vars
	//_outFile << "MOV SP D" << to_string(level) << " \n";


	//_outFile << "PUSH " << initVal.value << " \n";

	//change the loop variable to its assignment
	//_outFile << "PUSH " << "-3(D" << level << ") \n";
	_outFile << "PUSH " << "-3(SP) \n"; //one 
	_outFile << "POP " << initVal.value << " \n"; //zero

	//////////////////////////////////////
	////////////initialized
	//////////////////////////////////////

	//skip loop increment the first time
	int skipIncrement = getNextLabelVal();
	_outFile << "BR L" << skipIncrement << "\n";

	//begin the the condition check
	beginCondition = getNextLabelVal();
	_outFile << "L" << beginCondition << ": \n";

	//update the loop variable
	_outFile << "PUSH " << stepVal.value << " \n"; //one 
	_outFile << "PUSH " << initVal.value << " \n"; //one two
	_outFile << "ADDS " << " \n"; //one
	_outFile << "POP " << "-3(SP) \n";// -3  // zero
	_outFile << "PUSH " << "-3(SP) \n";//one
	_outFile << "POP " << initVal.value << " \n";//zero
	//loop variable updated

	_outFile << "L" << skipIncrement << ": \n";

	//check against limit
	_outFile << "PUSH " << "-3(SP) \n"; //one
	_outFile << "PUSH " << "-3(SP) \n"; //two
	//limit checked

	string compare = stepVal.value == "#-1" ? "CMPGES" : "CMPLES";
	_outFile << compare << "\n";

	exitLoop = getNextLabelVal();
	_outFile << "BRFS L" << to_string(exitLoop) << " \n";


	//if the step is negative we continue if greater than the condition
	//otherwise we want to continue if the value is less than the condition

}

void SemanticAnalyser::forEndBody(int loopAgain, int exitLoop, SemanticRecord& intialRecord)
{
	//int level = _currentTable->level();
	//revert to control variable
	LexemeOperand lexOp1 = intialRecord.getNextOperandAsLexeme();
	MachineVal initVal = generateMachineValue(lexOp1.getLexeme());

	_outFile << "MOV  -3(SP) " << initVal.value << " \n";


	_outFile << "BR L" << to_string(loopAgain) << " \n";
	_outFile << "L" << to_string(exitLoop) << ": \n";

	//revert to value before loop
	_outFile << "POP " << initVal.value << " \n";

	//this will pop off the intial assigment value
	//_outFile << "PUSH D" << to_string(level) << " \n";
	//_outFile << "PUSH #-2" << " \n";
	//_outFile << "ADDS" << " \n";
	_outFile << "POP 1(SP)" << " \n";
	_outFile << "POP 1(SP)" << " \n";
	//we are now three down to where we should be
	//BOOYAH!!!

	if (ENABLE_DEBUGGING)
		_outFile << "\n;         End 'for' loop\n";

}

list<DataType> SemanticAnalyser::arguments(LexemeOperand lexop)
{
	bool ok = false;
	const Symbol funSymbol = lookupSymbol(lexop.getName(), ok);

	list<DataType> argTypes = funSymbol.argumentTypes();
	return argTypes;
}

void SemanticAnalyser::prodCall(SemanticRecord& id, SemanticRecord& args)
{
	//look up the function

	LexemeOperand* fun = id.showNextOperandAs<LexemeOperand>();
	assert(fun);
	bool ok = false;
	const Symbol funSymbol = lookupSymbol(fun->getName(), ok);

	list<DataType> argTypes = funSymbol.argumentTypes();
	

	//TODO: check the input types vs the required types

	//while (args.size() > 0){
	//	if (argTypes.size() == 0){
	//		//error out with "Incorrect number of arguments"
	//		assert(false);
	//		break;
	//	}
	//	DataType nextArgType = argTypes.front();
	//	argTypes.pop_front();

	//	if (args.showNextOperandAs<LexemeOperand>()){
	//		LexemeOperand nextArg = args.getNextOperandAsLexeme();
	//		if (DataIsAddress(nextArgType)){
	//			//then we need to push the address of the value 
	//			//on to the stack
	//			bool ok;
	//			DataType outType;
	//			string addr = lookupSymbolAddress(nextArg.getName(), ok, outType);

	//			if (!ok){
	//				assert(false);
	//				//argument not found
	//			}
	//			if (outType != DataAsReferenceType(nextArgType)){
	//				assert(false);
	//				//incorrect argument type
	//			}

	//			Symbol matchingSymbol = lookupSymbol(nextArg.getName(), ok);

	//			writeCommand("PUSH D" + to_string(matchingSymbol.level()));
	//			writeCommand("PUSH #" +to_string( matchingSymbol.offset()));
	//			writeCommand("ADDS");
	//		}
	//		else{
	//			//just push the value its self
	//			push(&nextArg, nextArgType);
	//		}
	//	}
	//	else{
	//		//they should all have lexemes so we can lok them up
	//		assert(false);
	//	}
	//}

	writeCommand("CALL L" + to_string(funSymbol.label()));
}

Operand* SemanticAnalyser::funCall(SemanticRecord& id, SemanticRecord& args)
{
	prodCall(id, args);

	LexemeOperand* fun = id.showNextOperandAs<LexemeOperand>();
	assert(fun);
	bool ok = false;
	const Symbol funSymbol = lookupSymbol(fun->getName(), ok);


	return new StackOperand(funSymbol.dataType());
}

void SemanticAnalyser::insertArguments(SemanticRecord& inputRecord)
{
	int offset = inputRecord.size();
	offset++; //for the program counter
	if (_currentTable->function()){
		//take one more for the return value
		offset++;
	}
	offset = offset + 10; //for register copy 
	offset = offset* (-1);//moving backwards from SP at time of call

	while (inputRecord.size()){
		if (inputRecord.showNextOperandAs<LexemeOperand>()){
			LexemeOperand nextOp = inputRecord.getNextOperandAsLexeme();
			insertArgument(nextOp.getLexeme(), offset, nextOp.type());
			offset++;
		}
	}
}

void SemanticAnalyser::generateActivationRecord(int beginRecord)
{

	//The size of the memeory that we are going to need will be 
	//equal to the number of new variables

	int memAlloc = _currentTable->allocSize();

	writeCommand("L" + to_string(beginRecord) + ":");

	if (_currentTable->funProd()){
		//save all of the registers 
		writeCommand("PUSH D0");
		writeCommand("PUSH D1");
		writeCommand("PUSH D2");
		writeCommand("PUSH D3");
		writeCommand("PUSH D4");
		writeCommand("PUSH D5");
		writeCommand("PUSH D6");
		writeCommand("PUSH D7");
		writeCommand("PUSH D8");
		writeCommand("PUSH D9");
	}

	int level = _currentTable->level();

	writeCommand("MOV SP D" + to_string(level));
	writeCommand("PUSH SP");
	writeCommand("PUSH #" + to_string(memAlloc));
	writeCommand("ADDS");
	writeCommand("POP SP");



}

void SemanticAnalyser::restoreRegisterState()
{
	writeCommand("POP D9");
	writeCommand("POP D8");
	writeCommand("POP D7");
	writeCommand("POP D6");
	writeCommand("POP D5");
	writeCommand("POP D4");
	writeCommand("POP D3");
	writeCommand("POP D2");
	writeCommand("POP D1");
	writeCommand("POP D0");
}

void SemanticAnalyser::functionHeading(int& beginLabel)
{
	beginLabel = getNextLabelVal();
	writeCommand("PUSH #\"Yahoo\"");
	writeCommand("BR L" + to_string(beginLabel));
}

void SemanticAnalyser::functionEnd()
{
	//put the stack pointer back to its original spot
	int level = _currentTable->level();
	writeCommand("MOV D" + to_string(level) + " SP");

	//the return value is one below the stack pointer
	//and the PC value is one below that

	//int argSize = _currentTable->allocSize(); //- _currentTable->size();
	int argSize = _currentTable->argumentTypes().size();

	//argSize = argSize + 10;


	//int offset = argSize + 2 ;//one for the PC and one for the ReVal
	writeCommand("MOV -12(SP) 0(SP)"); //move PC to safety
	writeCommand("MOV -11(SP) 1(SP)"); //move Ret Val to safety

	writeCommand("MOV 1(SP) " + to_string(-(12 + argSize)) + "(SP)"); //move Ret
	writeCommand("MOV 0(SP) " + to_string(-(11 + argSize)) + "(SP)"); //move PC

	//move the SP into position
	restoreRegisterState();

	//ont top of the stack are space for the arguments
	//and for the return value and the PC

	argSize = _currentTable->argumentTypes().size();

	writeCommand("PUSH SP");
	//we want to be right above the pc which should 2 up!
	writeCommand("PUSH #" + to_string((argSize))); //for offset
	writeCommand("SUBS");
	writeCommand("POP SP");

	writeCommand("RET");
}

void SemanticAnalyser::procedureHeading(int& beginProc)
{
	beginProc = getNextLabelVal();
	writeCommand("BR L" +to_string(beginProc));
}

void SemanticAnalyser::procedureEnd()
{

	//put the stack pointer back to its original spot
	int level = _currentTable->level();
	writeCommand("MOV D" + to_string(level) + " SP");

	//the return value is one below the stack pointer
	//and the PC value is one below that

	//int argSize = _currentTable->allocSize(); //- _currentTable->size();
	int argSize = _currentTable->argumentTypes().size();

	argSize = argSize + 10;

	int offset = argSize + 1;//one for the PC
	writeCommand("MOV -11(SP) 0(SP)"); //move PC to safety
	writeCommand("MOV 0(SP) " + to_string((-offset)) + "(SP)"); //move PC

	//move the SP into position
	restoreRegisterState();
	//argSize = _currentTable->allocSize();// -_currentTable->size();
	argSize =_currentTable->argumentTypes().size();

	writeCommand("PUSH SP");
	writeCommand("PUSH #" + to_string((argSize))); //for offset
	writeCommand("SUBS");
	writeCommand("POP SP");
	writeCommand("RET");

}

void SemanticAnalyser::unaryPrefixCommand(SemanticRecord& infixSymbols)
{
	int size = infixSymbols.size();
	assert(infixSymbols.size() == 2);

	LexemeOperand * first = dynamic_cast<LexemeOperand*>(infixSymbols.getNextOperandPointer());
	assert(first);
	MachineVal firstArg = generateMachineValue(first->getLexeme());

	CommandOperand command = infixSymbols.getNextOperandAsCommand();

	//MachineVal secondArg;
	//LexemeOperand* second = NULL;
	//if (infixSymbols.size() > 0){
	//	second = dynamic_cast<LexemeOperand*>(infixSymbols.getNextOperandPointer());
	//	if (second)
	//		secondArg = generateMachineValue(second->getLexeme());
	//}

	string address = DataIsAddress(firstArg.type) ? "@" : "";

	_outFile << command.getCommand()  << " "<<address << firstArg.value << " \n";

	delete first;
}

StackOperand SemanticAnalyser::infixStackCommand(SemanticRecord& infixSymbols)
{
	int size = infixSymbols.size();
	assert(infixSymbols.size() == 3);
	
	//we need to do some sort of type resolution here if the two operands are not
	//the same type, one approach could be to take the smallest common type
	//or to take the type of the first value (current behavior)

	Operand * first = infixSymbols.getNextOperandPointer();

	CommandOperand command = infixSymbols.getNextOperandAsCommand();

	DataType type = command.type() == UnknownData ? first->type() : command.type();

	push(first, type);

	Operand* second = infixSymbols.getNextOperandPointer();
	push(second, type);

	writeCommand(command.getCommand());

	StackOperand retVal = StackOperand(command.type() == UnknownData ? first->type() : type);
	
	//we own these as soon as the are take from the SemanticRecord
	//delete them now that we are done 
	delete first;
	delete second;

	return retVal;
}

StackOperand SemanticAnalyser::pushAddress(Lexeme lex, DataType type)
{
	LexemeOperand nextArg(lex, type);
	//then we need to push the address of the value 
	//on to the stack
	bool ok;
	DataType outType;
	string addr = lookupSymbolAddress(nextArg.getName(), ok, outType);

	if (!ok){
		assert(false);
		//argument not found
	}
	if (DataAsReferenceType(outType) != DataAsReferenceType(type)){
		assert(false);
		//incorrect argument type
	}

	if (DataIsAddress(outType)){
		Symbol matchingSymbol = lookupSymbol(nextArg.getName(), ok);
		writeCommand("PUSH " + to_string(matchingSymbol.offset()) + "(D" + to_string(matchingSymbol.level())+")");
	}
	else{
		Symbol matchingSymbol = lookupSymbol(nextArg.getName(), ok);

		writeCommand("PUSH D" + to_string(matchingSymbol.level()));
		writeCommand("PUSH #" + to_string(matchingSymbol.offset()));
		writeCommand("ADDS");
	}



	return StackOperand(type);
}

StackOperand SemanticAnalyser::push(Lexeme lex, DataType type)
{
	LexemeOperand * lexOp = new LexemeOperand(lex, UnknownData);

	push(lexOp, type);
	StackOperand retVal(lexOp->type());

	delete lexOp;
	return retVal;
}

Operand SemanticAnalyser::twoValueCommand(const string command, SemanticRecord records)
{
	assert(records.size() == 2);

	//we need to do some sort of type resolution here if the two operands are not
	//the same type, one approach could be to take the smallest common type
	//or to take the type of the first value (current behavior)

	Operand * first = records.getNextOperandPointer();
	push(first);
	DataType typeOfOp = first->type();

	Operand* second = records.getNextOperandPointer();
	push(second, typeOfOp);

	writeCommand(command);


	delete first;
	delete second;
	
	return StackOperand(typeOfOp);
}

void SemanticAnalyser::push(Operand* val, DataType castType)
{
	if (!val->onTopOfStack())
	{

		//then this should be on the top of the stack
		//and it doesnt need to be pushed
		LexemeOperand* lexOp = dynamic_cast<LexemeOperand*>(val);
		assert(lexOp);
		MachineVal valAddress = generateMachineValue(lexOp->getLexeme());

		//I dont like this but it is a way to pass the
		//type back up
		val->setType(valAddress.type);

		string addressOperator;
		if (DataIsAddress(lexOp->type())){
			addressOperator = "@";
		}

		if (ENABLE_DEBUGGING && !val->getName().empty()){
			//add comment with the variable name before the push 
			_outFile << ";\t\tPushing: " << val->getName() << " Type:" << to_string(val->type())<< "\n";
		}

		_outFile << "PUSH " << addressOperator << valAddress.value << " \n";

	}
	if (castType != UnknownData && castType != BoolData){
		cast(val->type(), castType);
	}
    _outFile.flush();
}

void SemanticAnalyser::cast(DataType valType, DataType toType)
{
	valType = DataAsReferenceType(valType);
	toType = DataAsReferenceType(toType);
	if (valType == toType){
		return; //already the correct type
	}

	if (valType == IntData && toType == FloatData){
		writeCommand("CASTSF");
		return;
	}

	if (valType == FloatData && toType == IntData){
		writeCommand("CASTSI");
		return;
	}

	assert(false);
	//we should not be casting other type
	return;
}

void SemanticAnalyser::writeCommand(const string command)
{
	_outFile << command << "\n";
	_outFile.flush();
}

int SemanticAnalyser::getNextLabelVal()
{
	return _labelCounter++;
}