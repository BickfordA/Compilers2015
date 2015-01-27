#include "FiniteStateAutomaton.h"

FiniteStateAutomaton::FiniteStateAutomaton(istream* fileStream)
	:_filePointer(fileStream)
{}

bool FiniteStateAutomaton::charIsWhiteSpace(char c)
{
	if (c == ' ') return true;
	if (c == '\t') return true;
	if (c == '\n') return true;

	return false;
}

bool FiniteStateAutomaton::charIsDigit(char c)
{
	if (c == '1') return true;
	if (c == '2') return true;
	if (c == '3') return true;
	if (c == '4') return true;
	if (c == '5') return true;
	if (c == '6') return true;
	if (c == '7') return true;
	if (c == '8') return true;
	if (c == '9') return true;
	if (c == '0') return true;
	
	return false;
}

bool FiniteStateAutomaton::charIsUpperAlphabet(char c)
{
	if (c == 'A') return true;
	if (c == 'B') return true;
	if (c == 'C') return true;
	if (c == 'D') return true;
	if (c == 'E') return true;
	if (c == 'F') return true;
	if (c == 'G') return true;
	if (c == 'H') return true;
	if (c == 'I') return true;
	if (c == 'J') return true;
	if (c == 'K') return true;
	if (c == 'L') return true;
	if (c == 'M') return true;
	if (c == 'N') return true;
	if (c == 'O') return true;
	if (c == 'P') return true;
	if (c == 'Q') return true;
	if (c == 'R') return true;
	if (c == 'S') return true;
	if (c == 'T') return true;
	if (c == 'U') return true;
	if (c == 'V') return true;
	if (c == 'W') return true;
	if (c == 'X') return true;
	if (c == 'Y') return true;
	if (c == 'Z') return true;

	return false;
}

bool FiniteStateAutomaton::charIsLowerAlphabet(char c)
{
	if (c == 'a') return true;
	if (c == 'b') return true;
	if (c == 'c') return true;
	if (c == 'd') return true;
	if (c == 'e') return true;
	if (c == 'f') return true;
	if (c == 'g') return true;
	if (c == 'h') return true;
	if (c == 'i') return true;
	if (c == 'j') return true;
	if (c == 'k') return true;
	if (c == 'l') return true;
	if (c == 'm') return true;
	if (c == 'n') return true;
	if (c == 'o') return true;
	if (c == 'p') return true;
	if (c == 'q') return true;
	if (c == 'r') return true;
	if (c == 's') return true;
	if (c == 't') return true;
	if (c == 'u') return true;
	if (c == 'v') return true;
	if (c == 'w') return true;
	if (c == 'x') return true;
	if (c == 'y') return true;
	if (c == 'z') return true;

	return false;
}