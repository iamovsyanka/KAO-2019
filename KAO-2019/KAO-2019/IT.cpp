#include "pch.h"
#include "IT.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
			throw ERROR_THROW(203);
		IdTable idTable;
		idTable.maxsize = size;
		idTable.size = NULL;
		idTable.table = new Entry[size];
		return idTable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
			throw ERROR_THROW(204);
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable & idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, unsigned char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp((const char*)idtable.table[i].id, (const char*)id) == 0)
				return i;
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}

	void WriteTable(IdTable& idtable)
	{
		int i, numberNoneId = 0;
		std::cout << std::endl << std::setfill('-') << std::setw(31) << '-' << " ТАБЛИЦА ИДЕНТИФИКАТОРОВ " << std::setfill('-') << std::setw(33) << '-' << std::endl;
		std::cout << std::setfill('-') << std::setw(89) << '-' << std::endl;
		std::cout << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение/Параметры" << std::endl;
		std::cout << std::setw(89) << '-' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			std::cout << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			std::cout << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case IT::INT:
				std::cout << std::setw(10) << std::left;
				std::cout << "int_" << " | ";
				break;
			case IT::STR:
				std::cout << std::setw(10) << std::left;
				std::cout << "str_" << " | ";
				break;
			case IT::BOOL:
				std::cout << std::setw(10) << std::left;
				std::cout << "bool_" << " | ";
				break;
			}
			switch (idtable.table[i].idtype)
			{
			case IT::V: std::cout << std::setw(18) << std::left << "переменная" << " | "; break;
			case IT::F: std::cout << std::setw(18) << std::left << "функция" << " | "; break;
			case IT::P: std::cout << std::setw(18) << std::left << "параметр" << " | "; break;
			case IT::L: std::cout << std::setw(18) << std::left << "литерал" << " | ";
				numberNoneId++;
				break;
			case IT::S: std::cout << std::setw(18) << std::left << "библ. функция" << " | ";
				numberNoneId++;
				break;
			}
			std::cout << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == IT::INT && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::BOOL && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::STR && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				std::cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].idtype == IT::F || idtable.table[i].idtype == IT::S)
			{
				for (int k = 0; k < idtable.table[i].value.params.count; k++)
				{
					std::cout << std::left << "P" << k << ":";
					switch (idtable.table[i].value.params.types[k])
					{
					case IT::INT:std::cout << std::left << "I|"; break;
					case IT::STR:std::cout << std::left << "S|"; break;
					case IT::BOOL:std::cout << std::left << "B|"; break;
					}
				}
				if (idtable.table[i].value.params.count == NULL)
					std::cout << "-";
			}
			else
				std::cout << "-";
			std::cout << std::endl;
		}
		std::cout << std::setfill('-') << std::setw(89) << '-' << std::endl;
		std::cout << "     | Количество идентификаторов: " << i - numberNoneId << std::endl;
		std::cout << std::setw(89) << '-' << std::endl;
	}
}