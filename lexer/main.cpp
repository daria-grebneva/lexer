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
std::vector<char> DELIMITERS;
std::vector<char> OPERATIONS;
const enum TOKEN_STATUS : int {
	ID = 1,
	NUMBER = 2,
	OPERATION = 3,
	DELIMITER = 4,
	KEYWORD = 5,
	COMMENTS = 6,
	ERROR = 7,
	START = 8
};

const enum COMMENT_STATE : int {
	SINGLE_LINE = 1,
	MULTILINE = 2,
	NO_COMMENT = 3
};


} // namespace Lexer

void initializeKeywords()
{
	Lexer::KEYWORDS.push_back("if");
	Lexer::KEYWORDS.push_back("then");
	Lexer::KEYWORDS.push_back("else");
	Lexer::KEYWORDS.push_back("while");
	Lexer::KEYWORDS.push_back("do");
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
}

void initializeTokens()
{
	initializeKeywords();
	initializeOperations();
	initializeDelimiters();
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
	return isalpha(symbol);
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

void changeTokenState(char curr, int& state)
{
	if (checkInDelimiteryMap(curr))
	{
		Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::DELIMITER, string(1, curr) });
		state = Lexer::TOKEN_STATUS::DELIMITER;
	}
	if (checkInOperationMap(curr))
	{
		if (curr != '/')
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

void processPrevLexem(string& lexem, int& state)
{
	if (empty(lexem))
	{
		return;
	}
	if (lexem[0] == '/')
	{
		lexem.erase(0, 1);
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
		for (int i = 0; i < lexem.size(); ++i)
		{
			if (!isDigit(lexem[i]))
			{
				state = Lexer::TOKEN_STATUS::ERROR;
			}
		}

		if (state != Lexer::TOKEN_STATUS::ERROR)
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::NUMBER, lexem });
		}
		else
		{
			Lexer::lexerTable.push_back({ Lexer::TOKEN_STATUS::ERROR, lexem });
			state = Lexer::TOKEN_STATUS::START;
		}
		
	}
	state = Lexer::TOKEN_STATUS::START;
	lexem = "";
}

void processString(fstream& fin)
{
	int commentState = Lexer::COMMENT_STATE::NO_COMMENT;
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

		for (int i = 0; i < str.size(); ++i)
		{
			char curr = str[i];
			changeCommentState(curr, prev, commentState, lexema);

			if (commentState != Lexer::COMMENT_STATE::MULTILINE && commentState != Lexer::COMMENT_STATE::SINGLE_LINE)
			{
				changeTokenState(curr, tokenState);
				if (tokenState == Lexer::TOKEN_STATUS::DELIMITER)
				{
					processPrevLexem(lexema, tokenState);
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

		for (int i = 0; i < Lexer::lexerTable.size(); ++i)
		{
			switch (Lexer::lexerTable[i].type)
			{
			case Lexer::TOKEN_STATUS::ID:
				cout << "id " << Lexer::lexerTable[i].lexema << endl;
				break;
			case Lexer::TOKEN_STATUS::NUMBER:
				cout << "number " << Lexer::lexerTable[i].lexema << endl;
					break;
			case Lexer::TOKEN_STATUS::OPERATION:
				cout << "operation " << Lexer::lexerTable[i].lexema << endl;
					break;
			case Lexer::TOKEN_STATUS::DELIMITER:
				cout << "delimiter " << Lexer::lexerTable[i].lexema << endl;
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
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
