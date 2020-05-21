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

	struct Entry																			// ������ ������� ������
	{
		unsigned char lexema;																// �������
		int line;																			// ����� ������ � �������� ������
		int idxTI;																			// ������ � ������� ��������������� ��� LT_TI_NULLIDX
		int priority;																		// ���������
		unsigned char sType;
		OPER operation;
	};

	struct LexTable																			// ��������� ������� ������
	{
		int maxsize;																		// ������� ������� ������ < LT_MAXSIZE
		int size;																			// ������� ������ ������� ������ < maxsize
		Entry* table;																		// ������ ����� ������� ������
	};

	LexTable Create(																		// ������� ������� ������
		int size																			// ������� ������� ������ < LT_MAXSIZE
	);

	void Add(																				// �������� ������ � ������� ������
		LexTable& lextable,																	// ��������� ������� ������
		Entry entry																			// ������ ������� ������
	);

	Entry GetEntry(																			// �������� ������ ������� ������
		LexTable& lextable,																	// ��������� ������� ������
		int n																				// ����� ���������� ������
	);

	void Delete(LexTable& lextable);														

	Entry AddLex(Entry &entry, unsigned char lexema, int indx, int line, unsigned char sType, int priority = -1, OPER operation = NOT);

	void WriteLexTable(LexTable& lextable);
};