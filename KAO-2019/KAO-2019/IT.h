#pragma once
#include "pch.h"
#include "Error.h"
#include "LT.h"

#define ID_MAXSIZE		10																	// ������������ ���������� �������� � ��������������													// ������������ ����� �������� � �������������� + ������� ���������
#define TI_MAXSIZE		4096																// ������������ ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000															// �������� �� ��������� ��� ���� integer
#define TI_STR_DEFAULT	0x00																// �������� �� ��������� ��� ���� string
#define TI_NULLIDX		0xffffffff															// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255																	// ������������ ����� ������													
#define MAX_PARAMS_COUNT 3
#define RANDOM_PARAMS_COUNT 1
#define STRLEN_PARAMS_COUNT 1
#define RANDOM_TYPE IT::IDDATATYPE::INT
#define STRLEN_TYPE IT::IDDATATYPE::INT

namespace IT																				// ������� ���������������
{
	enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, DEC, UNKNOWN };						// ���� ������ ���������������: integer, string, ��� ����(���������), bool, ��������������
	enum IDTYPE {
		V = 1, F = 2, P = 3, L = 4, S = 5													// ���� ���������������: ����������, �������, ��������, �������, ����. �������	
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

	IdTable Create(																			// ������� ������� ���������������
		int size																			// ������� ������� ��������������� < TI_MAXSIZE
	);

	void Add(																				// �������� ������ � ������� ���������������
		IdTable& idtable,																	// ��������� ������� ���������������
		Entry entry																			// ������ ������� ���������������
	);

	Entry GetEntry(IdTable& idtable, int n);

	int IsId(IdTable& idtable,	unsigned char id[ID_MAXSIZE]);

	void Delete(IdTable& idtable);															

	void WriteTable(IdTable& idtable);
};