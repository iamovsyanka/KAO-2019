#include "pch.h"
#include "Log.h"

using namespace std;

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open())	throw ERROR_THROW(103);
		wcscpy_s(log.logfile, logfile);
		return log;
	}

	void WriteLine(LOG log, char* c, ...)
	{
		char** pointer = &c;
		int len = 0;
		while (*pointer != "")
		{
			len += strlen(*pointer);
			pointer++;
		}
		char *string = new char[len + 1];
		string[0] = 0;
		pointer = &c;
		while (*pointer != "")
		{
			strcat(string, *pointer);
			pointer++;
		}
		log.stream->write(string, len);
		delete[] string;
	}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t** pointer = &c;
		int len = 0;
		while (*pointer != L"")
		{
			len += wcslen(*pointer);
			pointer++;
		}
		wchar_t *wstring = new wchar_t[len + 1];
		wstring[0] = 0;
		pointer = &c;
		while (*pointer != L"")
		{
			wcscat(wstring, *pointer);
			pointer++;
		}
		char* string = new char[len + 1];
		wcstombs(string, wstring, len + 1);
		log.stream->write(string, len);
		delete[] string, wstring;
	}

	void WriteLog(LOG log)
	{
		time_t curTime;
		tm timeStruct;
		time(&curTime);
		char strTime[30];
		localtime_s(&timeStruct, &curTime);
		strftime(strTime, 1024, "%d.%m.%Y %H:%M:%S", &timeStruct);
		*log.stream << "---- �������� -------" << setfill('-') << setw(49) << '-' << strTime << endl;
		cout << "---- �������� -------" << setfill('-') << setw(49) << '-' << strTime << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		*log.stream << "\n---- ��������� ------" << setfill('-') << setw(68) << '-' << endl;
		cout << "\n---- ��������� ------" << setfill('-') << setw(68) << '-' << endl;

		char temp[PARM_MAX_SIZE + 1];
		wcstombs(temp, PARM_LOG, wcslen(PARM_LOG) + 1);
		*log.stream << temp << " ";
		cout << temp << " ";
		wcstombs(temp, parm.log, wcslen(parm.log) + 1);
		*log.stream << temp << endl;
		cout << temp << endl;

		wcstombs(temp, PARM_OUT, wcslen(PARM_OUT) + 1);
		*log.stream << temp << " ";
		cout << temp << " ";
		wcstombs(temp, parm.out, wcslen(parm.out) + 1);
		*log.stream << temp << endl;
		cout << temp << endl;

		wcstombs(temp, PARM_IN, wcslen(PARM_IN) + 1);
		*log.stream << temp << "  ";
		cout << temp << "  ";
		wcstombs(temp, parm.in, wcslen(parm.in) + 1);
		*log.stream << temp << endl;
		cout << temp << endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- �������� ������ ------" << setfill('-') << setw(62) << '-' << endl;
		*log.stream << "���������� ��������: " << in.size << endl;
		*log.stream << "���������������    : " << in.ignor << endl;
		*log.stream << "���������� �����   : " << in.lines << endl;

		cout << "\n---- �������� ������ ------" << setfill('-') << setw(62) << '-' << endl;
		cout << "���������� ��������: " << in.size << endl;
		cout << "���������������    : " << in.ignor << endl;
		cout << "���������� �����   : " << in.lines << endl;
	}

	void WriteError(LOG log, Error::ERROR error)											
	{
		if (log.stream != NULL)
		{
			if (error.inext.line == -1)	
				*log.stream << "Error" << error.id << "  " << error.message << endl;
			else if (error.inext.position == NULL || error.inext.position == -1)
				*log.stream << "Error" << error.id << "  " << error.message << endl << " ������: " << error.inext.line + 1 << endl;
			else
				*log.stream << "Error" << error.id << "  " << error.message << endl << " ������: " << error.inext.line + 1 << " | ������� � ������: " << error.inext.position << endl;
			*log.stream << endl;
		}

		if (error.inext.line == -1)
			cout << "Error" << error.id << "  " << error.message << endl;
		else if (error.inext.position == NULL || error.inext.position == -1)
			cout << "Error" << error.id << "  " << error.message << endl << " ������: " << error.inext.line + 1 << endl;
		else
			cout << "Error" << error.id << "  " << error.message << endl << " ������: " << error.inext.line + 1 << " | ������� � ������: " << error.inext.position << endl;

		cout << endl;
		system("pause");
		exit(-1);
	}

	void WriteIDTableLog(IT::IdTable& idtable, LOG log)
	{
		int numberNoneId = 0;
		*log.stream << endl << setfill('~') << setw(31) << '~' << " ������� ��������������� " << setfill('~') << setw(33) << '~' << endl;
		*log.stream << setfill('-') << setw(89) << '-' << endl;
		*log.stream << "�" << " | " << "�������������" << " | " << "��� ������" << " | " << "��� ��������������" << " | " << "������ � ��" << " | " << "��������/���������" << endl;
		*log.stream << setw(89) << '-' << endl;
		for (int i = 0; i < idtable.size; i++)
		{
			*log.stream << right << i << " | ";
			*log.stream << setfill(' ') << setw(13) << left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case IT::INT:
				*log.stream << setw(10) << left;
				*log.stream << "integer" << " | ";
				break;
			case IT::STR:
				*log.stream << setw(10) << left;
				*log.stream << "string" << " | ";
				break;
			case IT::BOOL:
				*log.stream << setw(10) << left;
				*log.stream << "bool" << " | ";
				break;
			}
			switch (idtable.table[i].idtype)
			{
			case IT::V: *log.stream << setw(18) << left << "����������" << " | "; break;
			case IT::F: *log.stream << setw(18) << left << "�������" << " | "; break;
			case IT::P: *log.stream << setw(18) << left << "��������" << " | "; break;
			case IT::L: *log.stream << setw(18) << left << "�������" << " | ";
				numberNoneId++;
				break;
			case IT::S: *log.stream << setw(18) << left << "����. �������" << " | ";
				numberNoneId++;
				break;
			}
			*log.stream << setw(11) << left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == IT::INT && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << setw(18) << left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::BOOL && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << setw(18) << left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::STR && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].idtype == IT::F || idtable.table[i].idtype == IT::S)
			{
				for (int k = 0; k < idtable.table[i].value.params.count; k++)
				{
					*log.stream << left << k+1 << ":";
					switch (idtable.table[i].value.params.types[k])
					{
					case IT::INT:*log.stream << left << "integer "; break;
					case IT::STR:*log.stream << left << "string "; break;
					case IT::BOOL:*log.stream << left << "bool "; break;
					}
				}
				if (idtable.table[i].value.params.count == NULL)
					*log.stream << "-";
			}
			else
				*log.stream << "-";
			*log.stream << endl;
		}
		*log.stream << setfill('-') << setw(89) << '-' << endl;
	}

	void WriteLineLexTableLog(LT::LexTable& lextable, LOG log)
	{
		*log.stream << endl << setfill('~') << setw(31) << '~' << " ������� ������ " << setfill('~') << setw(42) << '~' << endl;
		*log.stream << setfill('-') << setw(89) << '-' << endl;
		*log.stream << "   �" << " | " << "�������" << setfill(' ') << setw(20) << ' ' << left << " | " << "������" << setw(21) << ' ' << left << " | " << "������ � ��" << endl;
		*log.stream << setfill('-') << setw(89) << '-' << endl;
		for (int i = 0; i < lextable.size; i++)
		{
			*log.stream << setfill('0') << setw(4) << right << i << " | " << setfill(' ') << setw(24) << left << lextable.table[i].lexema << "    | " << setw(24) << left
				<< lextable.table[i].line + 1 << "    | ";
			if (lextable.table[i].idxTI == LT_TI_NULLIDX)	*log.stream << "-" << endl;
			else											*log.stream << setw(23) << lextable.table[i].idxTI << endl;
		}
	}

	void WriteLexTableLog(LT::LexTable& lextable, LOG log)
	{
		*log.stream << endl << setfill('~') << setw(31) << '~' << " ������� ������ " << setfill('~') << setw(42) << '~' << endl;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].line != lextable.table[i - 1].line)
			{
				(*log.stream) << endl;
				(*log.stream) << lextable.table[i].line + 1 << " ";
			}
			(*log.stream) << lextable.table[i].lexema;
		}
		(*log.stream) << "\n";
	}

	void WriteTraceLog(LOG log)
	{
		cout << endl << setw(25) << '-' << " ����������� ��������������� ������� " << setw(27) << '-' << endl << setw(89) << '-' << endl \
			<< " ���" << " | " << setfill(' ') \
			<< setw(25) << left << "�������" << " | " \
			<< setw(30) << left << "������� �����" << " | " \
			<< setw(20) << left << "����" \
			<< endl << setfill('-') << setw(89) << '-' << endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
		log.stream = NULL;
	}
}