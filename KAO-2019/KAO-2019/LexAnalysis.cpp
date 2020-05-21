#include "pch.h"
#include "LexAnalysis.h"

extern Log::LOG logfile;

namespace LexAnalysis
{
	Table lex;
	LT::LexTable lextable = LT::Create(LT_MAXSIZE);
	IT::IdTable idtable = IT::Create(TI_MAXSIZE);
	IT::Entry entryIT;
	const char SP[] = { " .&()[]{}=+-*%/|<>?#" };

	void ClearSpace(unsigned char source[], int size)																										
	{
		bool findLit = false;
		int count = NULL, length = NULL;
		for (int i = 0; i < size; i++)
		{
			if (source[i] == '\n')
				source[i] = '|';
		}
		for (int i = 0; i < size; i++)
		{
			if (source[i] == '\"')
			{
				findLit = !findLit;
				if (length + 1 > 255)	Log::WriteError(logfile, Error::geterror(304));
				length = NULL;
				count++;
			}
			if (findLit)	length++;
			if ((source[i] == ' ' || source[i] == '\t') && !findLit)
			{
				for (int j = 0; j < sizeof(SP) - 1; j++)
				{
					if (source[i + 1] == SP[j] || source[i - 1] == SP[j] || i == 0)
					{
						for (int k = i; k < size; k++)
							source[k] = source[k + 1];
						i--;
						break;
					}
				}
			}
		}
		if (count % 2 != 0)	Log::WriteError(logfile, Error::geterror(300));
	}
	unsigned char** GetWord(unsigned char source[], int size)											
	{
		unsigned char** word = new unsigned char*[LT_MAXSIZE];
		for (int i = 0; i < LT_MAXSIZE; i++)
			word[i] = new unsigned char[256]{ NULL };

		bool findSP, findLit = false;
		int j = 0;

		for (int i = 0, k = 0; i < size - 1; i++, k++)
		{
			findSP = false;
			if (source[i] == '\"')
				findLit = !findLit;
			for (int t = 0; t < sizeof(SP) - 1; t++)
			{
				if (source[i] == SP[t] && !findLit)
				{
					findSP = true;
					if (word[j][0] != NULL) 
					{
						word[j++][k] = '\0';
						k = 0;
					}
					if (SP[t] == ' ') 
					{
						k = -1;
						break;
					}
					word[j][k++] = SP[t];
					word[j++][k] = '\0';
					k = -1;
					break;
				}
			}
			if (!findSP)
				word[j][k] = source[i];
		}
		word[j] = NULL;
		for (int i = 0; i < j; i++)
			if (!strcmp((char*)word[i], ""))
				return NULL;
		return word;
	}
	void Addiddatatype(IT::IDDATATYPE iddatatype, int line)
	{
		LT::Entry entryLT = AddLex(entryLT, LEX_TYPE, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
		LT::Add(lextable, entryLT);
		entryIT.iddatatype = iddatatype;
	}

	Table LexAnalysis(In::IN in, Log::LOG log)
	{
		int iLex = 0, iID = 0, countLit = 1, position = 0, line = 0;
		// индекс лексемы | индекс идентификатора | счетчик литералов 
		unsigned char emptystr[] = ""; //значение строкового литерала
		unsigned char* prefix = new unsigned char[ID_MAXSIZE] { "" }, *bufprefix = new unsigned char[ID_MAXSIZE] { "" }, *oldprefix = new unsigned char[ID_MAXSIZE] { "" };								
		//ОВ			текущий префикс			|		буфер для префиксa		|		предыдущий префикс
		unsigned char* L = new unsigned char[2]{ "L" }, *bufL = new unsigned char[TI_STR_MAXSIZE], *nameLiteral = new unsigned char[10]{ "" };
		// имя литерала начинает с 'L'			|		буфер для литерала		|		имя литерала
		char* charclit = new char[10]{ "" }; // строковая запись счетчика
		bool func = false, libFunc = false, param = false;
		int go = 0, idFunc = -1, idParam = -1;
		int open = 0, close = 0;
		ClearSpace(in.text, in.size);
		unsigned char** word = new unsigned char*[LT_MAXSIZE];
		for (int i = 0; i < LT_MAXSIZE; i++)
			word[i] = new unsigned char[WORD_MAXSIZE] {NULL};
		do {
			word = GetWord(in.text, in.size);
		} while (word == NULL);
	
		for (int i = 0; word[i] != NULL; i++, iLex++)
		{
			bool dbID = false;
			
			FST::FST fstVar(word[i], GRAPH_VAR);
			if (FST::execute(fstVar))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_VAR, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstTypeInteger(word[i], GRAPH_INT);
			if (FST::execute(fstTypeInteger))
			{
				Addiddatatype(IT::INT, line);
				continue;
			}

			FST::FST fstTypeBool(word[i], GRAPH_BOOL);
			if (FST::execute(fstTypeBool))
			{
				Addiddatatype(IT::BOOL, line);
				continue;
			}

			FST::FST fstTypeStr(word[i], GRAPH_STR);
			if (FST::execute(fstTypeStr))
			{
				Addiddatatype(IT::STR, line);
				_mbscpy(entryIT.value.vstr.str, emptystr);
				continue;
			}

			FST::FST fstFunction(word[i], GRAPH_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				entryIT.value.params.count = 0;
				entryIT.value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
				idFunc = iID;
				func = true;
				continue;
			}

			FST::FST fstRandom(word[i], GRAPH_RANDOM);
			if (FST::execute(fstRandom))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_RANDOM, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT = {};
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = iLex;
				entryIT.idtype = IT::S;
				_mbscpy(entryIT.visibility, emptystr);
				entryIT.iddatatype = RANDOM_TYPE;
				entryIT.value.params.count = RANDOM_PARAMS_COUNT;
				entryIT.value.params.types = new IT::IDDATATYPE[RANDOM_PARAMS_COUNT];
				for (int k = 0; k < RANDOM_PARAMS_COUNT; k++)
					entryIT.value.params.types[k] = IT::RANDOM_PARAMS[k];
				IT::Add(idtable, entryIT);
				entryIT = {};
				libFunc = true;
				continue;
			}

			FST::FST fstStrlen(word[i], GRAPH_STRLEN);
			if (FST::execute(fstStrlen))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_STRLEN, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT = {};
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = iLex;
				entryIT.idtype = IT::S;
				_mbscpy(entryIT.visibility, emptystr);
				entryIT.iddatatype = STRLEN_TYPE;
				entryIT.value.params.count = STRLEN_PARAMS_COUNT;
				entryIT.value.params.types = new IT::IDDATATYPE[STRLEN_PARAMS_COUNT];
				for (int k = 0; k < STRLEN_PARAMS_COUNT; k++)
					entryIT.value.params.types[k] = IT::STRLEN_PARAMS[k];
				IT::Add(idtable, entryIT);
				entryIT = {};
				libFunc = true;
				continue;
			}

			FST::FST fstGoNewStr(word[i], GRAPH_GONEWSTR);
			if (FST::execute(fstGoNewStr))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_GONEWSTR, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstReturn(word[i], GRAPH_RETURN);
			if (FST::execute(fstReturn))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_RETURN, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstOut(word[i], GRAPH_OUT);
			if (FST::execute(fstOut))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_OUT, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstCheck(word[i], GRAPH_CHECK);
			if (FST::execute(fstCheck))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_CHECK, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstOtherwise(word[i], GRAPH_OTHERWISE);
			if (FST::execute(fstOtherwise))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_OTHERWISE, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstGo(word[i], GRAPH_GO);
			if (FST::execute(fstGo))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_GO, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				_mbscpy(oldprefix, prefix);
				_mbscpy(prefix, word[i]);
				_mbscpy(entryIT.visibility, emptystr);
				go++;
				continue;
			}

			FST::FST fstTrue(word[i], GRAPH_TRUE);
			FST::FST fstFalse(word[i], GRAPH_FALSE);
			if (FST::execute(fstTrue) || FST::execute(fstFalse))
			{
				int value;
				if (!strcmp((char*)word[i], "true"))
					value = 1;																		
				else
					value = 0;
				for (int k = 0; k < idtable.size; k++)												
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::BOOL)
					{
						LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, k, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						dbID = true;
						break;
					}
				}

				if (dbID)	continue;
				LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::BOOL;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = iLex;
				_itoa_s(countLit++, charclit, sizeof(char) * 10, 10);				
				_mbscpy(bufL, L);													
				word[i] = _mbscat(bufL, (unsigned char*)charclit);					
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}
			FST::FST fstIdentif(word[i], GRAPH_ID);
			if (FST::execute(fstIdentif))
			{
				if (func)																		
				{
					int idx = IT::IsId(idtable, word[i]);										
					if (idx != TI_NULLIDX)	Log::WriteError(log, Error::geterrorin(308, line, -1));
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]);											
					if (idx != TI_NULLIDX)															
					{
						LT::Entry entryLT = AddLex(entryLT, LEX_ID, idx, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						func = false;
						continue;
					}
					_mbscpy(bufprefix, prefix);
					word[i] = _mbscat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);												
					if (idx != TI_NULLIDX)															
					{
						LT::Entry entryLT = AddLex(entryLT, LEX_ID, idx, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						if (lextable.table[lextable.size - 3].lexema == LEX_VAR)
						{
							if (idtable.table[idx].idtype == IT::P)	Log::WriteError(log, Error::geterrorin(309, line, -1));
							else									Log::WriteError(log, Error::geterrorin(310, line, -1));
						}
						continue;
					}
				}

				LT::Entry entryLT = AddLex(entryLT, LEX_ID, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);

				if (param) 
				{																		
					entryIT.idtype = IT::P;
					_mbscpy(entryIT.visibility, prefix);
					idParam++;
					idtable.table[idFunc].value.params.count++;
					if (idtable.table[idFunc].value.params.count > 3)
						Log::WriteError(log, Error::geterrorin(314, line, -1));
					idtable.table[idFunc].value.params.types[idParam] = entryIT.iddatatype;
				}

				else if (!func) 
				{																
					entryIT.idtype = IT::V;
					_mbscpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT || entryIT.iddatatype == IT::BOOL)
						entryIT.value.vint = TI_INT_DEFAULT;									
					if (entryIT.iddatatype == IT::STR) 
					{
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));			
					}

					if (lextable.table[lextable.size - 2].lexema == LEX_TYPE && lextable.table[lextable.size - 3].lexema != LEX_VAR)
						Log::WriteError(log, Error::geterrorin(305, line, position));
					if (lextable.table[lextable.size - 2].lexema == LEX_VAR && lextable.table[lextable.size - 2].lexema != LEX_TYPE)
						Log::WriteError(log, Error::geterrorin(307, line, position));
				}
				else 
				{																				
					_mbscpy(oldprefix, prefix);
					_mbscpy(prefix, word[i]);
					_mbscpy(entryIT.visibility, emptystr);
				}

				entryIT.idxfirstLE = iLex;
				_mbscpy(entryIT.id, word[i]);

				if (entryIT.idtype == IT::F && entryIT.iddatatype == IT::UNKNOWN)
					Log::WriteError(log, Error::geterrorin(311, line, -1));
				if (entryIT.idtype == IT::V && entryIT.iddatatype == IT::UNKNOWN)
					Log::WriteError(log, Error::geterrorin(306, line, -1));

				IT::Add(idtable, entryIT);															
				entryIT = {};
				func = false;
				continue;
			}

			FST::FST fstLiteralInt(word[i], GRAPH_INT_LITERAL);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)word[i]);
				if (value > 4294967295 || value < 0) Log::WriteError(log, Error::geterrorin(319, line, -1));
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT)
					{
						LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, k, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						dbID = true;
						break;
					}
				}

				if (dbID)	continue;

				LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = iLex;
				_itoa_s(countLit++, charclit, sizeof(char) * 10, 10);		
				_mbscpy(bufL, L);																	
				word[i] = _mbscat(bufL, (unsigned char*)charclit);									
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}

			FST::FST fstLiteralInt16(word[i], GRAPH_INT16);
			if (FST::execute(fstLiteralInt16))
			{
				int value = strtol((char*)word[i], NULL, 16);
				
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT)
					{
						LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, k, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						dbID = true;
						break;
					}
				}

				if (dbID)	continue;

				LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = iLex;
				_itoa_s(countLit++, charclit, sizeof(char) * 10, 10);
				_mbscpy(bufL, L);
				word[i] = _mbscat(bufL, (unsigned char*)charclit);
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}

			FST::FST fstLiteralString(word[i], GRAPH_STRING_LITERAL);
			if (FST::execute(fstLiteralString))
			{
				int length = _mbslen(word[i]);														
				for (int k = 0; k < length; k++)													
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;
				if (_mbslen(word[i]) == 0)	Log::WriteError(log, Error::geterrorin(318, line, position));
				for (int k = 0; k < idtable.size; k++)												
				{
					if (!(_mbscmp(idtable.table[k].value.vstr.str, word[i])))
					{
						dbID = true;
						LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, k, line, LEX_STYPE_NULL);
						LT::Add(lextable, entryLT);
						break;
					}
				}

				if (dbID)	continue;

				LT::Entry entryLT = AddLex(entryLT, LEX_LITERAL, iID++, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.value.vstr.str, word[i]);											
				entryIT.value.vstr.len = length - 2;												
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = iLex;
				_itoa_s(countLit++, charclit, sizeof(char) * 10, 10);									
				_mbscpy(bufL, L);																	
				nameLiteral = _mbscat(bufL, (unsigned char*)charclit);								
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}

			FST::FST fstOperator(word[i], GRAPH_SIGN);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT;
				switch (word[i][0])																	
				{
				case '+':
					entryLT = AddLex(entryLT, LEX_OPERATOR, LT_TI_NULLIDX, line, '+', 2, LT::PLUS);
					break;
				case '-':
					entryLT = AddLex(entryLT, LEX_OPERATOR, LT_TI_NULLIDX, line, '-', 2, LT::MINUS);
					break;
				case '/':
					entryLT = AddLex(entryLT, LEX_OPERATOR, LT_TI_NULLIDX, line, '/', 4, LT::DIR);
					break;
				case '*':
					entryLT = AddLex(entryLT, LEX_OPERATOR, LT_TI_NULLIDX, line, '*', 4, LT::MUL);
					break;
				case '%':
					entryLT = AddLex(entryLT, LEX_OPERATOR, LT_TI_NULLIDX, line, '%', 4, LT::OST);
					break;
				}
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstLogOperator(word[i], GRAPH_LOG_OPERATOR);
			if (FST::execute(fstLogOperator))
			{
				LT::Entry entryLT;
				switch (word[i][0])
				{
				case '>':
					entryLT = AddLex(entryLT, LEX_LOGOPERATOR, LT_TI_NULLIDX, line, LEX_STYPE_NULL,  -1, LT::MORE);
					break;
				case '<':
					entryLT = AddLex(entryLT, LEX_LOGOPERATOR, LT_TI_NULLIDX, line, LEX_STYPE_NULL, -1, LT::LESS);
					break;
				case '?':
					entryLT = AddLex(entryLT, LEX_LOGOPERATOR, LT_TI_NULLIDX, line, LEX_STYPE_NULL, -1, LT::EQU);
					break;
				case '#':
					entryLT = AddLex(entryLT, LEX_LOGOPERATOR, LT_TI_NULLIDX, line, LEX_STYPE_NULL, -1, LT::NOEQU);
					break;
				}
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstPoint(word[i], GRAPH_POINT);
			if (FST::execute(fstPoint))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_POINT, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstAnd(word[i], GRAPH_AND);
			if (FST::execute(fstAnd))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_AND, LT_TI_NULLIDX, line, LEX_STYPE_NULL, 1);
				LT::Add(lextable, entryLT);
				continue;
			}
			
			FST::FST fstLeftBrace(word[i], GRAPH_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				open++;
				continue;
			}

			FST::FST fstRightBrace(word[i], GRAPH_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				close++;
				continue;
			}

			FST::FST fstLeftThesis(word[i], GRAPH_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line, LEX_STYPE_NULL, 0);
				LT::Add(lextable, entryLT);
				if (iID> 0 && idtable.table[iID- 1].idtype == IT::F)
					param = true;
				continue;
			}

			FST::FST fstRightThesis(word[i], GRAPH_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line, LEX_STYPE_NULL, 0);
				if (param && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)		
					_mbscpy(prefix, oldprefix);															
				idParam = -1;
				param = false;
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstLeftBracket(word[i], GRAPH_LEFTBRACKET);
			if (FST::execute(fstLeftBracket))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_LEFTBRACKET, LT_TI_NULLIDX, line, LEX_STYPE_NULL, 0);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstRightBracket(word[i], GRAPH_RIGHTBRACKET);
			if (FST::execute(fstRightBracket))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_RIGHTBRACKET, LT_TI_NULLIDX, line, LEX_STYPE_NULL, 0);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstEqual(word[i], GRAPH_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = AddLex(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line, LEX_STYPE_NULL);
				LT::Add(lextable, entryLT);
				continue;
			}

			position += _mbslen(word[i]);

			if (word[i][0] == '|') {
				line++;
				position = 0;
				iLex--;
				continue;
			}

			throw ERROR_THROW_IN(205, line, position);
		}

		if (go == NULL)		Log::WriteError(log, Error::geterror(302));
		if (go > 1)			Log::WriteError(log, Error::geterror(303));
		if (open > close)	Log::WriteError(log, Error::geterror(612));
		if (close > open)	Log::WriteError(log, Error::geterror(613));

		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}
