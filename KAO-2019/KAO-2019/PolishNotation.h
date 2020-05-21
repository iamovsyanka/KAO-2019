#pragma once
#include "pch.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Polish
{
	bool GoPolish(LexAnalysis::Table& lex);
	bool isPolish(int i, LexAnalysis::Table& lex);
}
