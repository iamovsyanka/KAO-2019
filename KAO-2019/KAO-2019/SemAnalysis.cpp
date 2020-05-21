#include "pch.h"
#include "SemAnalysis.h"

namespace Semantics
{
	void Analyze(LexAnalysis::Table lex, Log::LOG log)
	{
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_STRLEN:
			case LEX_RANDOM:
			case LEX_ID:
			{
				if ((lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F || lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::S)
					&& lex.lextable.table[i - 2].lexema != LEX_FUNCTION)
				{
					IT::Entry e = lex.idtable.table[lex.lextable.table[i].idxTI];
					int countParm = 0;
					for (int j = i + 2; lex.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (lex.lextable.table[j].lexema == LEX_ID || lex.lextable.table[j].lexema == LEX_LITERAL)
						{
							countParm++;
							if (countParm > e.value.params.count)
								Log::WriteError(log, Error::geterrorin(316, lex.lextable.table[j].line, -1));
							if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != e.value.params.types[countParm - 1])
								Log::WriteError(log, Error::geterrorin(315, lex.lextable.table[j].line, -1));
						}

					}
					if (countParm < e.value.params.count)
						Log::WriteError(log, Error::geterrorin(317, lex.lextable.table[i].line, -1));
				}
				break;
			}

			case LEX_EQUAL:
			{
				IT::IDDATATYPE lefttype = lex.idtable.table[lex.lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				for (int k = i + 1; lex.lextable.table[k].lexema != LEX_POINT; k++)
				{
					if (lex.lextable.table[k].lexema == LEX_ID || lex.lextable.table[k].lexema == LEX_LITERAL ||
						lex.lextable.table[k].lexema == LEX_RANDOM || lex.lextable.table[k].lexema == LEX_STRLEN)
					{
						if (!ignore)
						{
							IT::IDDATATYPE righttype = lex.idtable.table[lex.lextable.table[k].idxTI].iddatatype;
							if (lefttype != righttype)
								Log::WriteError(log, Error::geterrorin(320, lex.lextable.table[i].line, -1));
						}

						if (lex.lextable.table[k + 1].lexema == LEX_LEFTTHESIS)
						{
							ignore = true;
							continue;
						}

						if (ignore && lex.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
						{
							ignore = false;
							continue;
						}
					}

					if (lefttype == IT::IDDATATYPE::STR || lefttype == IT::IDDATATYPE::BOOL)
					{
						char l = lex.lextable.table[k].lexema;
						if (l == LEX_OPERATOR)
							Log::WriteError(log, Error::geterrorin(321, lex.lextable.table[i].line, -1));
					}
				}
				break;
			}

			case LEX_FUNCTION:
			{
				if (lex.lextable.table[i + 2].lexema == LEX_ID)
				{
					IT::Entry e = lex.idtable.table[lex.lextable.table[i + 2].idxTI];
					i++;
					while (i <= lex.lextable.size && (lex.lextable.table[i].lexema != LEX_FUNCTION || lex.lextable.table[i].lexema != LEX_GO))
					{
						if (lex.lextable.table[i].lexema == LEX_RETURN && (lex.lextable.table[i + 2].lexema == LEX_ID || lex.lextable.table[i + 2].lexema == LEX_LITERAL))
						{
							int next = lex.lextable.table[i + 2].idxTI;
							if (next != TI_NULLIDX)
							{
								if (lex.idtable.table[next].iddatatype != e.iddatatype)
									Log::WriteError(log, Error::geterrorin(313, lex.lextable.table[i].line, -1));
							}
							break;
						}
						i++;
					}
				}
				break;
			}

			case LEX_LOGOPERATOR:
			{
				bool flag = true;
				if (lex.lextable.table[i - 1].lexema == LEX_ID || lex.lextable.table[i - 1].lexema == LEX_LITERAL)
				{
					if (lex.idtable.table[lex.lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
						flag = false;
				}
				if (lex.lextable.table[i + 1].lexema == LEX_ID || lex.lextable.table[i + 1].lexema == LEX_LITERAL)
				{
					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
						flag = false;
				}
				if (!flag)
				{
					Log::WriteError(log, Error::geterrorin(322, lex.lextable.table[i].line, -1));
				}
				break;
			}
			case LEX_CHECK:
			{
				if (lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && lex.lextable.table[i + 3].lexema == LEX_RIGHTTHESIS)
					if ((lex.lextable.table[i + 2].lexema == LEX_ID || lex.lextable.table[i + 2].lexema == LEX_LITERAL) && lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype != IT::BOOL)
						Log::WriteError(log, Error::geterrorin(301, lex.lextable.table[i].line, -1));
			}
			}
		}
	}
}