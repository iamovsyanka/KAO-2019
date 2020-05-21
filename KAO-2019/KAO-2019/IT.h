#pragma once
#include "pch.h"
#include "Error.h"
#include "LT.h"

#define ID_MAXSIZE		10																	// максимальное количество символов в идентификаторе													// максимальное число символов в идентификаторе + область видимости
#define TI_MAXSIZE		4096																// максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000															// значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT	0x00																// значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff															// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255																	// максимальна€ длина строки													
#define MAX_PARAMS_COUNT 3
#define RANDOM_PARAMS_COUNT 1
#define STRLEN_PARAMS_COUNT 1
#define RANDOM_TYPE IT::IDDATATYPE::INT
#define STRLEN_TYPE IT::IDDATATYPE::INT

namespace IT																				// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, DEC, UNKNOWN };						// типы данных идентификаторов: integer, string, без типа(процедура), bool, неопределенный
	enum IDTYPE {
		V = 1, F = 2, P = 3, L = 4, S = 5													// типы идентификаторов: переменна€, функци€, параметр, литерал, библ. функци€	
	};

	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::INT };
	static const IDDATATYPE STRLEN_PARAMS[] = { IT::IDDATATYPE::STR };

	struct Entry																			
	{
		int			idxfirstLE;																
		unsigned char id[ID_MAXSIZE];														
		IDDATATYPE	iddatatype = UNKNOWN;													
		IDTYPE		idtype;																	
		unsigned char visibility[ID_MAXSIZE];												

		struct
		{
			int vint;																		
			double vdec;
			struct
			{
				int len;																	
				unsigned char str[TI_STR_MAXSIZE - 1];										
			} vstr;																			

			struct
			{
				int count;																	
				IDDATATYPE *types;															
			} params;
		} value = { LT_TI_NULLIDX };														
	};

	struct IdTable																			
	{
		int maxsize;																		
		int size;																		
		Entry* table;																		
	};

	IdTable Create(																			// создать таблицу идентификаторов
		int size																			// емкость таблицы идентификаторов < TI_MAXSIZE
	);

	void Add(																				// добавить строку в таблицу идентификаторов
		IdTable& idtable,																	// экземпл€р таблицы идентификаторов
		Entry entry																			// строка таблицы идентификаторов
	);

	Entry GetEntry(IdTable& idtable, int n);

	int IsId(IdTable& idtable,	unsigned char id[ID_MAXSIZE]);

	void Delete(IdTable& idtable);															

	void WriteTable(IdTable& idtable);
};