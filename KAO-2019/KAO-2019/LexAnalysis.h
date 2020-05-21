#pragma once
#include "In.h"
#include "Error.h"
#include "Log.h"
#include "IT.h"
#include "LT.h"
#include "FST.h"
#include "Graphs.h"

#define WORD_MAXSIZE 255

namespace LexAnalysis
{
	struct Table
	{
		IT::IdTable idtable;;
		LT::LexTable lextable;
	};
	Table LexAnalysis(In::IN in, Log::LOG log);
	void ClearSpace(unsigned char source[], int size);
	unsigned char** GetWord(unsigned char source[], int size);
	void Addiddatatype(IT::IDDATATYPE iddatatype, int line);
}