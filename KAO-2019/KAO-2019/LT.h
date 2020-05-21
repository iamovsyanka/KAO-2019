#pragma once
#include "Error.h"

#define LEXEMA_FIXSIZE		1																
#define LT_MAXSIZE			4096															
#define LT_TI_NULLIDX		0xfffffff														

#define LEX_TYPE			't'															
#define LEX_VAR				'v'																
#define LEX_ID				'i'																
#define LEX_LITERAL			'l'																
#define LEX_FUNCTION		'f'															
#define LEX_RETURN			'r'																
#define LEX_OUT				'o'																
#define LEX_GO				'g'																
#define LEX_POINT			'.'																
#define LEX_AND				'&'																
#define LEX_LEFTBRACE		'{'																
#define LEX_BRACELET		'}'																
#define LEX_LEFTTHESIS		'['																
#define LEX_RIGHTTHESIS		']'																
#define LEX_LEFTBRACKET		'('																
#define LEX_RIGHTBRACKET	')'																
#define LEX_OPERATOR		's'																
#define LEX_EQUAL			'='																
#define LEX_LOGOPERATOR		'b'																
#define LEX_CHECK			'c'																
#define LEX_OTHERWISE		'u'																
#define LEX_RANDOM			'!'																
#define LEX_STRLEN			'~'																
#define LEX_GONEWSTR		'n'																
#define LEX_STYPE_NULL		' '

namespace LT																				
{
	enum OPER { NOT = -1, PLUS = 1, MINUS, MUL, DIR, OST, MORE, LESS, EQU, NOEQU };

	struct Entry																			// строка таблицы лексем
	{
		unsigned char lexema;																// лексема
		int line;																			// номер строки в исходном тексте
		int idxTI;																			// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;																		// приоритет
		unsigned char sType;
		OPER operation;
	};

	struct LexTable																			// экземпл€р таблицы лексем
	{
		int maxsize;																		// емкость таблицы лексем < LT_MAXSIZE
		int size;																			// текущий размер таблицы лексем < maxsize
		Entry* table;																		// массив строк таблицы лексем
	};

	LexTable Create(																		// создать таблицу лексем
		int size																			// емкость таблицы лексем < LT_MAXSIZE
	);

	void Add(																				// добавить строку в таблицу лексем
		LexTable& lextable,																	// экземпл€р таблицы лексем
		Entry entry																			// строка таблицы лексем
	);

	Entry GetEntry(																			// получить строку таблицы лексем
		LexTable& lextable,																	// экземпл€р таблицы лексем
		int n																				// номер получаемой строки
	);

	void Delete(LexTable& lextable);														

	Entry AddLex(Entry &entry, unsigned char lexema, int indx, int line, unsigned char sType, int priority = -1, OPER operation = NOT);

	void WriteLexTable(LexTable& lextable);
};