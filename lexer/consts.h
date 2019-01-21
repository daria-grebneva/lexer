#pragma once
#include "pch.h"

std::string INPUT_PROGRAM_TEXT = "program.txt";
std::string OUTPUT_FILE = "out.txt";

const enum TOKEN_STATUS : int
{
	ID = 1,
	NUMBER = 2,
	SYMBOLS = 3,
	DELIMITERS = 4,
	KEYWORDS = 5,
	COMMENTS = 6,
	ERROR = 7
};

const enum CHAR_STATUS : int {
	SPACE = 1,
	DIGIT = 2,
	LETTER = 3,
	SYMBOL = 4
};

const std::vector<std::string> KEYWORDS = {
	"if", 
	"then",
	"else",
	"while",
	"do"
};

const std::vector<std::string> DELIMITERS = {
	";",
	"{",
	"}"
};

const std::vector<std::string> COMMENTS = {
	"//",
	"/*",
	"*/"
};

const std::vector<std::string> OPERATION = {
	"+",
	"-",
	":",
	"/"
};