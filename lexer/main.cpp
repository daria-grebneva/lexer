#include "pch.h"

using namespace std;

std::string INPUT_PROGRAM_TEXT = "program.txt";
std::string OUTPUT_FILE = "out.txt";

namespace Lexer
{
struct Lexema
{
	int type;
	string lexema;
};

vector<Lexema> lexerTable;
std::vector<std::string> KEYWORDS;
std::vector<std::string> TYPE;
std::vector<char> DELIMITERS;
std::vector<char> OPERATIONS;
const enum TOKEN_STATUS : int {
	ID = 1,
	INT = 2,
	REAL = 3,
	FLOAT = 4,
	OPERATION = 5,
	DELIMITER = 6,
	KEYWORD = 7,
	COMMENTS = 8,
	ERROR = 9,
	START = 10,
	STRING = 11,
	CHAR = 12
};

const enum COMMENT_STATE : int {
	SINGLE_LINE = 1,
	MULTILINE = 2,
	NO_COMMENT = 3
};

const enum STRING_STATE : int {
	IN_STRING = 1,
	NO_STRING = 2,
	IN_CHAR = 3,
	STRING_ERROR = 4
};

const enum NUMBER_STATUS : int {
	DIGIT = 1,
	LETTER = 2,
	DOT = 3,
	SPACE = 4,
	EPSELENT = 5,
	SIGN = 6,
	FINAL = 7,
	BEGIN = 8
};
std::vector<char> BINARY_NUMBER_SYSTEM;
std::vector<char> OCTAL_NUMBER_SYSTEM;
std::vector<char> HEXADECIMAL_NUMBER_SYSTEM;
std::vector<char> DECIMAL_NUMBER_SYSTEM;
} // namespace Lexer

void initializeType()
{
	Lexer::TYPE.push_back("int");
	Lexer::TYPE.push_back("string");
	Lexer::TYPE.push_back("float");
	Lexer::TYPE.push_back("real");
}

void initializeKeywords()
{
	Lexer::KEYWORDS.push_back("if");
	Lexer::KEYWORDS.push_back("then");
	Lexer::KEYWORDS.push_back("else");
	Lexer::KEYWORDS.push_back("while");
	Lexer::KEYWORDS.push_back("do");
	Lexer::KEYWORDS.push_back("for");
	Lexer::KEYWORDS.push_back("var");
	Lexer::KEYWORDS.push_back("int");
	Lexer::KEYWORDS.push_back("string");
	Lexer::KEYWORDS.push_back("float");
}
void initializeOperations()
{
	Lexer::OPERATIONS.push_back('+');
	Lexer::OPERATIONS.push_back('-');
	Lexer::OPERATIONS.push_back('/');
	Lexer::OPERATIONS.push_back('=');
	Lexer::OPERATIONS.push_back('*');
}
void initializeDelimiters()
{
	Lexer::DELIMITERS.push_back('{');
	Lexer::DELIMITERS.push_back('}');
	Lexer::DELIMITERS.push_back(';');
	Lexer::DELIMITERS.push_back(',');
	Lexer::DELIMITERS.push_back('(');
	Lexer::DELIMITERS.push_back(')');
	Lexer::DELIMITERS.push_back(':');
}

void initializeBinaryNumberSystem()
{
	Lexer::BINARY_NUMBER_SYSTEM.push_back('1');
	Lexer::BINARY_NUMBER_SYSTEM.push_back('0');
}

void initializeOctalNumberSystem()
{
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('0');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('1');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('2');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('3');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('4');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('5');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('6');
	Lexer::OCTAL_NUMBER_SYSTEM.push_back('7');
}

void initializeDecimalNumberSystem()
{
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('0');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('1');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('2');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('3');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('4');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('5');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('6');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('7');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('8');
	Lexer::DECIMAL_NUMBER_SYSTEM.push_back('9');
}

void initializeHexadecimalNumberSystem()
{
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('0');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('1');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('2');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('3');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('4');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('5');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('6');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('7');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('8');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('9');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('A');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('B');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('C');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('D');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('E');
	Lexer::HEXADECIMAL_NUMBER_SYSTEM.push_back('F');
}

void initializeTokens()
{
	initializeKeywords();
	initializeOperations();
	initializeDelimiters();
	initializeBinaryNumberSystem();
	initializeOctalNumberSystem();
	initializeDecimalNumberSystem();
	initializeHexadecimalNumberSystem();
	initializeType();
}

bool isBinary(char symbol)
{
	return (std::find(Lexer::BINARY_NUMBER_SYSTEM.begin(), Lexer::BINARY_NUMBER_SYSTEM.end(), symbol) != Lexer::BINARY_NUMBER_SYSTEM.end());
}

bool isOctal(char symbol)
{
	return (std::find(Lexer::OCTAL_NUMBER_SYSTEM.begin(), Lexer::OCTAL_NUMBER_SYSTEM.end(), symbol) != Lexer::OCTAL_NUMBER_SYSTEM.end());
}

bool isDecimal(char symbol)
{
	return (std::find(Lexer::DECIMAL_NUMBER_SYSTEM.begin(), Lexer::DECIMAL_NUMBER_SYSTEM.end(), symbol) != Lexer::DECIMAL_NUMBER_SYSTEM.end());
}

bool isHexadecimal(char symbol)
{
	return (std::find(Lexer::HEXADECIMAL_NUMBER_SYSTEM.begin(), Lexer::HEXADECIMAL_NUMBER_SYSTEM.end(), symbol) != Lexer::HEXADECIMAL_NUMBER_SYSTEM.end());
}

bool isSpace(char symbol)
{
	return (symbol == ' ');
}

bool isDigit(char symbol)
{
	return isdigit(symbol);
}

bool isLetter(char symbol)
{
	return (isalpha(symbol) || (symbol == '_'));
}

string getBuffer(fstream& fin)
{
	string str = "";
	getline(fin, str);

	return str;
}

void changeCommentState(char curr, char prev, int& state, string& lexema)
{
	if ((prev == '/') && state != Lexer::COMMENT_STATE::MULTILINE && state != Lexer::COMMENT_STATE::SINGLE_LINE)
	{
		if (curr == '*')
		{
			state = Lexer::COMMENT_STATE::MULTILINE;
			if (!empty(lexema))
			{
				lexema.pop_back();
			}
		}
		else if (curr == '/')
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::COMMENTS, "//" });
			state = Lexer::COMMENT_STATE::SINGLE_LINE;
			if (!empty(lexema))
			{
				lexema.pop_back();
			}
		}
		else
		{
			if (Lexer::lexerTable.back().type != Lexer::TOKEN_STATUS::COMMENTS)
			{
				Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::OPERATION, string(1, prev) });
				state = Lexer::TOKEN_STATUS::OPERATION;
			}
		}
	}
	else if ((prev == '*') && (curr == '/') && (state == Lexer::COMMENT_STATE::MULTILINE))
	{
		Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::COMMENTS, "/* */" });
		state = Lexer::COMMENT_STATE::NO_COMMENT;
	}
}

void changeStringState(char curr, string& wasChar, int& state)
{
	if (curr == '"')
	{
		if (state == Lexer::STRING_STATE::IN_STRING)
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::STRING, "\"\"" });
		}

		state = (state == Lexer::STRING_STATE::NO_STRING) ? Lexer::STRING_STATE::IN_STRING : Lexer::STRING_STATE::NO_STRING;
	}
	if (curr == '\'')
	{
		if (state == Lexer::STRING_STATE::IN_CHAR && (wasChar.size() == 1))
		{
			string lex = "\'" + wasChar + "\'";
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::CHAR, lex });
			wasChar = "";
		}
		else if (wasChar.size() > 1)
		{
			string lex = "\'" + wasChar + "\'";
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::ERROR, lex });
		}

		state = (state == Lexer::STRING_STATE::NO_STRING) ? Lexer::STRING_STATE::IN_CHAR : Lexer::STRING_STATE::NO_STRING;
	}
	if (state == Lexer::STRING_STATE::IN_CHAR && (curr != '\''))
	{
		wasChar += curr;
	}
}

bool checkInKeywordMap(string item)
{
	return (std::find(Lexer::KEYWORDS.begin(), Lexer::KEYWORDS.end(), item) != Lexer::KEYWORDS.end());
}

bool checkInDelimiteryMap(char item)
{
	return (std::find(Lexer::DELIMITERS.begin(), Lexer::DELIMITERS.end(), item) != Lexer::DELIMITERS.end());
}

bool checkInOperationMap(char item)
{
	return (std::find(Lexer::OPERATIONS.begin(), Lexer::OPERATIONS.end(), item) != Lexer::OPERATIONS.end());
}

char checkDelimitery(char curr, int& state)
{
	if (checkInDelimiteryMap(curr))
	{
		//Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::DELIMITER, string(1, curr) });
		state = Lexer::TOKEN_STATUS::DELIMITER;
		return curr;
	}

	return ' ';
}

void checkOperation(char curr, char prev, int& state)
{
	if (checkInOperationMap(curr))
	{
		if ((curr != '/') && (prev != 'e'))
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::OPERATION, string(1, curr) });
			state = Lexer::TOKEN_STATUS::OPERATION;
		}
	}
}

bool checkSingleLineEnd(int commentState)
{
	return (commentState == Lexer::COMMENT_STATE::SINGLE_LINE);
}

void processDigit(string lexem, bool (*f)(char), int start)
{
	int state = Lexer::NUMBER_STATUS::BEGIN;
	int token = Lexer::TOKEN_STATUS::ERROR;
	char prev = ' ';
	for (int i = start; i < lexem.size(); ++i)
	{
		if (f(lexem[start]) && (state == Lexer::NUMBER_STATUS::BEGIN))
		{
			state = Lexer::NUMBER_STATUS::DIGIT;
		}
		if (state == Lexer::NUMBER_STATUS::DIGIT && (i == lexem.size() - 1) && f(lexem[i]))
		{
			state = Lexer::NUMBER_STATUS::FINAL;
			token = Lexer::TOKEN_STATUS::INT;
		}
		if (state == Lexer::NUMBER_STATUS::DIGIT && (lexem[i] == '.'))
		{
			state = Lexer::NUMBER_STATUS::DOT;
		}
		if (state == Lexer::NUMBER_STATUS::DIGIT && (lexem[i] == 'e'))
		{
			state = Lexer::NUMBER_STATUS::EPSELENT;
		}
		if (state == Lexer::NUMBER_STATUS::DOT && (i == lexem.size() - 1) && (f(prev) || (prev == '.')) && f(lexem[i]))
		{
			state = Lexer::NUMBER_STATUS::FINAL;
			token = Lexer::TOKEN_STATUS::REAL;
		}
		if (state == Lexer::NUMBER_STATUS::DOT && (lexem[i] == 'e'))
		{
			state = Lexer::NUMBER_STATUS::EPSELENT;
		}
		if (state == Lexer::NUMBER_STATUS::EPSELENT && (lexem[i] == '+' || lexem[i] == '-'))
		{
			state = Lexer::NUMBER_STATUS::SIGN;
		}
		if (state == Lexer::NUMBER_STATUS::SIGN && (i == lexem.size() - 1) && isDigit(lexem[i]))
		{
			state = Lexer::NUMBER_STATUS::FINAL;
			token = Lexer::TOKEN_STATUS::FLOAT;
		}
		prev = lexem[i];
	}
	token = (state == Lexer::NUMBER_STATUS::FINAL) ? token : Lexer::TOKEN_STATUS::ERROR;
	Lexer::lexerTable.push_back({ token, lexem });
}

void processPrevLexem(string& lexem, int& state)
{
	if (lexem[0] == '/' || lexem[0] == '"' || lexem[0] == '\'')
	{
		lexem.erase(0, 1);
	}
	if (empty(lexem))
	{
		return;
	}
	if (isLetter(lexem[0]))
	{
		if (checkInKeywordMap(lexem))
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::KEYWORD, lexem });
		}
		else
		{
			for (int i = 0; i < lexem.size(); ++i)
			{
				if (!isLetter(lexem[i]) && !isDigit(lexem[i]))
				{
					state = Lexer::TOKEN_STATUS::ERROR;
				}
			}
			if (state != Lexer::TOKEN_STATUS::ERROR)
			{
				Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::ID, lexem });
			}
			else
			{
				Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::ERROR, lexem });
				state = Lexer::TOKEN_STATUS::START;
			}
		}
	}
	else
	{
		state = Lexer::TOKEN_STATUS::START;
		if (lexem.size() > 2)
		{
			string numberSystem = lexem.substr(0, 2);

			if (numberSystem == "0b")
			{
				processDigit(lexem, isBinary, 2);
			}
			else if (numberSystem == "0o")
			{
				processDigit(lexem, isOctal, 2);
			}
			else if (numberSystem == "0h")
			{
				processDigit(lexem, isHexadecimal, 2);
			}
			else
			{
				processDigit(lexem, isDecimal, 0);
			}
		}
		else
		{
			processDigit(lexem, isDecimal, 0);
		}
	}
	state = Lexer::TOKEN_STATUS::START;
	lexem = "";
}

void processString(fstream& fin)
{
	int commentState = Lexer::COMMENT_STATE::NO_COMMENT;
	int stringState = Lexer::STRING_STATE::NO_STRING;
	int tokenState = Lexer::TOKEN_STATUS::START;
	string str = getBuffer(fin);
	string lexema = "";
	while (!empty(str))
	{
		if (!empty(lexema))
		{
			processPrevLexem(lexema, tokenState);
		}
		char prev = ' ';

		//проверка на конец строки однострочного комментария
		commentState = checkSingleLineEnd(commentState) ? Lexer::COMMENT_STATE::NO_COMMENT : commentState;
		stringState = Lexer::STRING_STATE::NO_STRING;
		string wasChar = "";

		for (int i = 0; i < str.size(); ++i)
		{
			char curr = str[i];
			changeCommentState(curr, prev, commentState, lexema);
			changeStringState(curr, wasChar, stringState);

			if (commentState != Lexer::COMMENT_STATE::MULTILINE && commentState != Lexer::COMMENT_STATE::SINGLE_LINE && stringState == Lexer::STRING_STATE::NO_STRING)
			{
				char delimiter = checkDelimitery(curr, tokenState);
				checkOperation(curr, prev, tokenState);
				if (tokenState == Lexer::TOKEN_STATUS::DELIMITER)
				{
					processPrevLexem(lexema, tokenState);
					if (delimiter != ' ')
					{
						Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::DELIMITER, string(1, curr) });
					}
					tokenState = Lexer::TOKEN_STATUS::START;
				}
				else if (tokenState == Lexer::TOKEN_STATUS::OPERATION)
				{
					processPrevLexem(lexema, tokenState);
					tokenState = Lexer::TOKEN_STATUS::START;
				}
				else if (isSpace(curr))
				{
					processPrevLexem(lexema, tokenState);
				}
				else
				{
					lexema += curr;
				}
			}

			prev = curr;
		}
		str = getBuffer(fin);
	}
	if (!empty(lexema))
	{
		processPrevLexem(lexema, tokenState);
	}
}

void printLexems()
{
	for (int i = 0; i < Lexer::lexerTable.size(); ++i)
	{
		switch (Lexer::lexerTable[i].type)
		{
		case Lexer::TOKEN_STATUS::ID:
			cout << "id " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::INT:
			cout << "int " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::REAL:
			cout << "real " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::FLOAT:
			cout << "float " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::OPERATION:
			cout << "operation " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::DELIMITER:
			cout << "delimiter " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::STRING:
			cout << "string " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::CHAR:
			cout << "char " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::KEYWORD:
			cout << "keyword " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::COMMENTS:
			cout << "comment " << Lexer::lexerTable[i].lexema << endl;
			break;
		case Lexer::TOKEN_STATUS::ERROR:
			cout << "error " << Lexer::lexerTable[i].lexema << endl;
			break;
		default:
			break;
		}
	}
}

void checkIds(bool& isOk, int& i)
{
	//cout << "LEXEM: " << Lexer::lexerTable[i].lexema << endl;
	if (Lexer::lexerTable[i].type == Lexer::ID)
	{
		//cout << "ID: " << Lexer::lexerTable[i].lexema << endl;
		if (Lexer::lexerTable[++i].lexema == ",")
		{
			//cout << "TYPE: " << Lexer::lexerTable[i].lexema << endl;
			++i;
			if (Lexer::lexerTable[i].lexema == ";")
			{
				isOk = false;
			}
			return checkIds(isOk, i);
		}
	}
	if (Lexer::lexerTable[i].lexema != ";")
	{
		isOk = false;
	}
	else
	{
		++i;
	}
}

void checkRightTypeAndVar(bool& isOk, int& i)
{
	//cout << "Lexer " << Lexer::lexerTable[i].lexema << endl;
	if (std::find(Lexer::TYPE.begin(), Lexer::TYPE.end(), Lexer::lexerTable[i].lexema) != Lexer::TYPE.end())
	{
		++i;
		if (Lexer::lexerTable[i].lexema != ":")
		{
			isOk = false;
		}
		else
		{
			++i;
			if (Lexer::lexerTable[i].lexema != ";")
			{
				checkIds(isOk, i);
			}
			else
			{
				isOk = false;
			}
		}
	}
	else if (Lexer::lexerTable[i].lexema == ";")
	{
		isOk = false;
	}
	else
	{
		isOk = false;
	}
}

bool checkTokensOrder()
{

	bool isOk = (Lexer::lexerTable[0].lexema == "var");

	int i = 1;
	while (isOk && (i != Lexer::lexerTable.size()))
	{
		checkRightTypeAndVar(isOk, i);
	}

	return isOk;
}

int main()
{
	try
	{
		std::fstream fin(INPUT_PROGRAM_TEXT, std::istream::in);
		std::fstream fout(OUTPUT_FILE, std::ostream::out);
		if (!fin.is_open())
		{
			throw std::invalid_argument("wrong filename of input program");
		}
		initializeTokens();
		processString(fin);
		//printLexems();
		bool isOk = checkTokensOrder();
		cout << (isOk ? "ok" : "not ok") << endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
