#include "pch.h"
#include "CodeGeneration.h"

using namespace std;

namespace CodeGeneration
{
	void Generation(LexAnalysis::Table table, Log::LOG log)
	{
		HEAD
		PROC
		STACK	
		CONST
		ZERO
		AddConstSegm(table, log);
		DATA
		AddDataSegm(table, log);
		CODE
		AddCodeSegm(table, log);
	}

	void AddConstSegm(LexAnalysis::Table table, Log::LOG log)
	{
		for (int i = 0; i < table.idtable.size; i++)
		{
			if (table.idtable.table[i].idtype == IT::L)
			{
				*(log.stream) << "\t" << table.idtable.table[i].id;
				if (table.idtable.table[i].iddatatype == IT::STR)
					*(log.stream) << " BYTE \"" << table.idtable.table[i].value.vstr.str << "\", 0";
				if (table.idtable.table[i].iddatatype == IT::INT || table.idtable.table[i].iddatatype == IT::BOOL)
					*(log.stream) << " SDWORD " << table.idtable.table[i].value.vint;
				*(log.stream) << '\n';
			}
		}
	}
	void AddDataSegm(LexAnalysis::Table table, Log::LOG log)
	{
		for (int i = 0; i < table.lextable.size; i++)
		{
			if (table.lextable.table[i].lexema == LEX_VAR)
			{
				if (table.idtable.table[table.lextable.table[i + 2].idxTI].idtype == IT::V)
				{
					*(log.stream) << "\t" << table.idtable.table[table.lextable.table[i + 2].idxTI].id;
					if (table.idtable.table[table.lextable.table[i + 2].idxTI].iddatatype == IT::STR)
					{
						*(log.stream) << " DWORD ?\n";
					}
					if (table.idtable.table[table.lextable.table[i + 2].idxTI].iddatatype == IT::INT || table.idtable.table[table.lextable.table[i + 2].idxTI].iddatatype == IT::BOOL)
					{
						*(log.stream) << " SDWORD 0\n";
					}
				}
			}
		}
	}	
	void AddCodeSegm(LexAnalysis::Table table, Log::LOG log)
	{
		stack<unsigned char*> stk;
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0,
			countParm = 0;
		string strret = string(),
			func_name = string();
		bool flag_func = false,
			flag_ret = false,
			flag_go = false,
			flag_if = false,
			flag_then = false,
			flag_else = false,
			flag_strlen = false,
			flag_random = false,
			flag_callfunc = false,
			flag_condition = false,
			flag_dir = false;

		for (int i = 0; i < table.lextable.size; i++)
		{
			switch (table.lextable.table[i].lexema)
			{
			case LEX_FUNCTION:
			{
				while (table.lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					if (table.lextable.table[i].lexema == LEX_ID && table.idtable.table[table.lextable.table[i].idxTI].idtype == IT::F)
					{
						*(log.stream) << (func_name = string((char*)table.idtable.table[table.lextable.table[i].idxTI].id)) << " PROC ";
					}
					if (table.lextable.table[i].lexema == LEX_ID && table.idtable.table[table.lextable.table[i].idxTI].idtype == IT::P)
					{
						*(log.stream) << table.idtable.table[table.lextable.table[i].idxTI].id << " : ";
						if (table.idtable.table[table.lextable.table[i].idxTI].iddatatype == IT::BOOL)
						{
							*(log.stream) << "SDWORD";
						}
						else
						{
							*(log.stream) << "DWORD";
						}
					}
					if (table.lextable.table[i].lexema == LEX_AND)
					{
						*(log.stream) << ", ";
					}
					i++;
				}
				flag_func = true;
				*(log.stream) << "\n";
				break;
			}
			case LEX_GO:
			{
				flag_go = true;
				*(log.stream) << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				while (table.lextable.table[i].lexema != LEX_POINT)
				{
					switch (table.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_LITERAL:
					{
						if (table.idtable.table[table.lextable.table[i].idxTI].idtype == IT::F)
						{
							flag_callfunc = true;
						}
						if (table.idtable.table[table.lextable.table[i].idxTI].iddatatype == IT::INT || table.idtable.table[table.lextable.table[i].idxTI].iddatatype == IT::BOOL)
						{
							*(log.stream) << "\tpush " << table.idtable.table[table.lextable.table[i].idxTI].id << "\n";
							stk.push(table.idtable.table[table.lextable.table[i].idxTI].id);
							break;
						}
						if (table.idtable.table[table.lextable.table[i].idxTI].iddatatype == IT::STR)
						{
							char* s;
							if (table.idtable.table[table.lextable.table[i].idxTI].idtype == IT::L)
							{
								*(log.stream) << "\tpush offset " << table.idtable.table[table.lextable.table[i].idxTI].id << "\n";
								s = new char[8]{ "offset " };
							}
							else
							{
								*(log.stream) << "\tpush " << table.idtable.table[table.lextable.table[i].idxTI].id << "\n";
								s = new char[1]{ "" };
							}
							size_t len1 = strlen((char*)s);
							size_t len2 = strlen((char*)table.idtable.table[table.lextable.table[i].idxTI].id);
							char* result = (char*)malloc(len1 + len2 + 1);
							memcpy(result, s, len1);
							memcpy(result + len1, (char*)table.idtable.table[table.lextable.table[i].idxTI].id, len2 + 1);
							stk.push((unsigned char*)result);
							break;
						}
					}
					case LEX_OPERATOR:
					{
						switch (table.lextable.table[i].operation)
						{
							case LT::MUL:
							{
								*(log.stream) << "\tpop eax\n\tpop ebx\n";
								*(log.stream) << "\tmul ebx\n\tpush eax\n";
								break;
							}
							case LT::PLUS:
							{
								*(log.stream) << "\tpop eax\n\tpop ebx\n";
								*(log.stream) << "\tadd eax, ebx\n\tpush eax\n";
								break;
							}
							case LT::MINUS:
							{
								*(log.stream) << "\tpop ebx\n\tpop eax\n";
								*(log.stream) << "\tsub eax, ebx\n\tpush eax\n";
								break;
							}
							case LT::DIR:
							{
								flag_dir=true;
								*(log.stream) << "\tpop ebx" << endl;
								*(log.stream) << "\tmov edx, 0 " << endl;
								*(log.stream) << "\tpop eax" << endl;
								*(log.stream) << "\tcmp ebx, 0" << endl;
								*(log.stream) << "\tje finish" << endl;
								*(log.stream) << "\tidiv ebx" << endl;
								*(log.stream) << "\tpush eax" << endl;
								break;
							}
							case LT::OST: 
							{
								*(log.stream) << "\tpop ebx" << endl;
								*(log.stream) << "\tmov edx, 0 " << endl;
								*(log.stream) << "\tpop eax" << endl;
								*(log.stream) << "\tidiv ebx" << endl;
								*(log.stream) << "\tpush edx" << endl;
								*(log.stream) << "\tmov eax, edx" << endl;
							}
						}
						break;
					}
					case LEX_STRLEN:
					{
						flag_strlen = true;
						break;
					}
					case LEX_RANDOM:
					{
						flag_random = true;
						break;
					}
					case '@':
					{
						countParm = (char)table.lextable.table[i + 1].lexema - '0';
						for (int j = 1; j <= countParm; j++)
							*(log.stream) << "\tpop edx\n";
						for (int j = 1; j <= countParm; j++)
						{
							*(log.stream) << "\tpush " << stk.top() << "\n";
							stk.pop();
						}
						if (flag_callfunc && !flag_random && !flag_strlen)
						{
							*(log.stream) << "\tcall " << table.idtable.table[table.lextable.table[i - countParm - 1].idxTI].id << "\n\tpush eax\n";
							flag_callfunc = false;
						}

						if (flag_strlen)
						{
							*(log.stream) << "\tcall Strlen;" << "\n" << "\tpush eax\n";
							flag_strlen = false;
						}
						if (flag_random)
						{
							*(log.stream) << "\tcall Random\n" << "\tpush eax\n";
							flag_random = false;
						}
						break;
					}
					}
					i++;
				}
				*(log.stream) << "\tpop " << table.idtable.table[table.lextable.table[result_position].idxTI].id << "\n";
				break;
			}
			case '@':
			{
				countParm = (char)table.lextable.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++)
				{
					if (table.idtable.table[table.lextable.table[i - j].idxTI].iddatatype == IT::INT || table.idtable.table[table.lextable.table[i - j].idxTI].iddatatype == IT::BOOL)
						*(log.stream) << "\tpush " << table.idtable.table[table.lextable.table[i - j].idxTI].id << "\n";
					else
					{
						if (table.idtable.table[table.lextable.table[i - j].idxTI].idtype == IT::L)
							*(log.stream) << "\tpush offset" << table.idtable.table[table.lextable.table[i - j].idxTI].id << "\n";
						else
							*(log.stream) << "\tpush " << table.idtable.table[table.lextable.table[i - j].idxTI].id << "\n";
					}
				}
				*(log.stream) << "\tcall " << table.idtable.table[table.lextable.table[i - countParm - 1].idxTI].id << "\n";
				break;
			}
			case LEX_RETURN:
			{
				PUSH
				i++;
				if (table.idtable.table[table.lextable.table[i + 1].idxTI].idtype == IT::L)
				{
					*(log.stream) << table.idtable.table[table.lextable.table[i + 1].idxTI].value.vint << "\n";
				}
				else
				{
					*(log.stream) << table.idtable.table[table.lextable.table[i + 1].idxTI].id << "\n";
				}
				if (flag_func)
				{
					*(log.stream) << "\tjmp local" << num_of_ret << "\n";
					flag_ret = true;
				}
				if (flag_go)
				{
					*(log.stream) << "\t\tjmp theend\n";
					flag_ret = true;
				}
				if (flag_dir)
				{
					*(log.stream) << "\tpop eax" << endl;
					*(log.stream) << "\tret" << endl;
					*(log.stream) << "\tcall exitProcess" << endl;
					*(log.stream) << "\tfinish:" << endl;
					*(log.stream) << "\tpush offset divisionByZero" << endl;
					*(log.stream) << "\tcall outstr" << endl;
					*(log.stream) << "\tcall exitProcess" << endl;
				}
				break;
			}
			case LEX_BRACELET:
			{
				if (flag_go && !flag_else && !flag_func)
				{
					if (flag_ret)
					{
						*(log.stream) << "theend:\n";
						flag_ret = false;
					}
					*(log.stream) << "\tfinish:" << endl;
					*(log.stream) << "\tpush offset divisionByZero" << endl;
					*(log.stream) << "\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (flag_func && !flag_then && !flag_else)
				{
					if (flag_ret)
					{
						*(log.stream) << "local" << num_of_ret++ << ":\n";
						*(log.stream) << "\tpop eax\n\tret\n";
						flag_ret = false;
					}
					*(log.stream) << func_name << " ENDP\n\n";
					flag_func = false;
				}
				if (flag_then)
				{
					flag_then = false;
					if (flag_else)
					{
						*(log.stream) << "\tjmp e" << num_of_ends << "\n";
						flag_else = false;
					}
					*(log.stream) << "m" << num_of_points++ << ":\n";
				}
				if (flag_else)
				{
					flag_else = false;
					*(log.stream) << "e" << num_of_ends++ << ":\n";
				}
				break;
			}
			case LEX_CHECK:
			{
				flag_if = true;
				break;
			}
			case LEX_OTHERWISE:
			{
				flag_else = true;
				break;
			}
			case LEX_LEFTTHESIS:
			{
				if (flag_if)
				{
					if (table.idtable.table[table.lextable.table[i + 1].idxTI].iddatatype == IT::BOOL && table.lextable.table[i + 2].lexema == LEX_RIGHTTHESIS)
					{
						*(log.stream) << "\tmov eax, " << table.idtable.table[table.lextable.table[i + 1].idxTI].id << "\n";
						*(log.stream) << "\tcmp eax, 1\n";
						*(log.stream) << "\tjz m" << num_of_points << "\n";
						*(log.stream) << "\tjnz m" << num_of_points + 1 << "\n";
					}
					else
					{
						*(log.stream) << "\tmov eax, " << table.idtable.table[table.lextable.table[i + 1].idxTI].id << "\n";
						*(log.stream) << "\tcmp eax, " << table.idtable.table[table.lextable.table[i + 3].idxTI].id << "\n";
						if (table.lextable.table[i + 2].operation == LT::MORE)
						{
							*(log.stream) << "\tjg m" << num_of_points << "\n";
							*(log.stream) << "\tjl m" << num_of_points + 1 << "\n";
						}
						else if (table.lextable.table[i + 2].operation == LT::LESS)
						{
							*(log.stream) << "\tjl m" << num_of_points << "\n";
							*(log.stream) << "\tjg m" << num_of_points + 1 << "\n";
						}
						else if (table.lextable.table[i + 2].operation == LT::EQU)
						{
							*(log.stream) << "\tjz m" << num_of_points << "\n";
							*(log.stream) << "\tjnz m" << num_of_points + 1 << "\n";
						}
						else if (table.lextable.table[i + 2].operation == LT::NOEQU)
						{
							*(log.stream) << "\tjnz m" << num_of_points << "\n";
							*(log.stream) << "\tjz m" << num_of_points + 1 << "\n";
						}
					}
					*(log.stream) << "\tje m" << num_of_points + 1 << "\n";
					int j = i;
					while (table.lextable.table[j++].lexema != LEX_BRACELET)
					{
						if (table.lextable.table[j + 1].lexema == LEX_OTHERWISE)
						{
							flag_else = true;
							break;
						}
					}
				}
				break;
			}
			case LEX_RIGHTTHESIS:
			{
				if (table.lextable.table[i + 1].lexema == LEX_LEFTBRACE && flag_if)
				{
					flag_then = true;
					*(log.stream) << "m" << num_of_points++ << ":\n";
					flag_if = false;
				}
				break;
			}
			case LEX_OUT:
			{
				if (table.idtable.table[table.lextable.table[i + 2].idxTI].iddatatype == IT::INT || table.idtable.table[table.lextable.table[i + 2].idxTI].iddatatype == IT::BOOL)
					*(log.stream) << "\tpush " << table.idtable.table[table.lextable.table[i + 2].idxTI].id << "\n\tcall OutInt\n";
				else
				{
					if (table.idtable.table[table.lextable.table[i + 2].idxTI].idtype == IT::L)
						*(log.stream) << "\tpush offset ";
					else
						PUSH
					*(log.stream) << table.idtable.table[table.lextable.table[i + 2].idxTI].id << "\n\tcall OutStr\n";
				}
				break;
			}
			case LEX_GONEWSTR:
			{
				*(log.stream) << "\tcall Line\n";
				break;
			}
			}
		}
	}
};