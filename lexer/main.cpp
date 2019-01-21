#include "pch.h"
#include "consts.h"

using namespace std;

int getAuthomateState()
{
	return TOKEN_STATUS::ERROR;
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

void processString(fstream& fin)
{
	string str = getBuffer(fin);
	while (!empty(str))
	{
		string lexema = "";
		for (int i = 0; i < str.size(); ++i)
		{
			char s = str[i];
					   
			while (!isSpace(s))
			{
				lexema += s;
			}

			if (isSpace(s))
			{
				//work with lexema

				lexema = "";
			}
		}
		str = getBuffer(fin);
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

		processString(fin);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


