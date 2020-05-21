#include "pch.h"
#include "In.h"

namespace In
{
	IN getin(wchar_t infile[])																//считывание файла In
	{
		IN in;
		std::ifstream file;
		unsigned char letter;
		int i = 0, position = -1;
		in.size = 0;
		in.ignor = 0;
		in.lines = 0;
		in.text = new unsigned char[IN_MAX_LEN_TEXT];
		int code[256] = IN_CODE_TABLE;
		for (int k = 0; k < 256; k++)
			in.code[k] = code[k];
		file.open(infile);
		if (file.fail())
			throw ERROR_THROW(102);
		while (true) {
			letter = file.get();
			if (file.eof())
				break;
			in.size++;
			position++;
			if (in.code[(int)letter] == in.F) {
				throw ERROR_THROW_IN(200, in.lines, position);
			}
			else if (in.code[(int)letter] == in.T) {
				in.text[i] = letter;
				i++;
			}
			else if (in.code[(int)letter] == in.I) {
				in.ignor++;
				in.size--;
			}
			else {
				in.text[i] = in.code[(int)letter];
				i++;
			}
			if (letter == IN_CODE_ENDL) {
				in.lines++;
				position = -1;
			}
		}
		in.text[i] = '\0';
		return in;
	}
}