#pragma once

#include "Parm.h"
#include "LT.h"
#include "IT.h"
#include "LexAnalysis.h"

#pragma region Templates
	#define HEAD *(log.stream) << ".586\n\
	.model flat, stdcall\n\
	includelib libucrt.lib\n\
	includelib kernel32.lib\n\
	includelib ../Debug/StaticLib.lib\n\
	ExitProcess PROTO :DWORD\n\n";

	#define PROC 	*(log.stream) << "EXTRN Random: proc\n\
	EXTRN Strlen: proc\n\
	EXTRN Line: proc\n\
	EXTRN OutInt: proc\n\
	EXTRN OutStr: proc\n";

	#define STACK	*(log.stream) << "\n.stack 4096\n\n";
	#define CONST	*(log.stream) << "\n.const\n";
	#define DATA	*(log.stream) << "\n.data\n";
	#define CODE	*(log.stream) << "\n.code\n\n";
	#define ZERO 		*log.stream << "\tdivisionByZero db 'ERROR: DIVIDE BY ZERO', 0" << endl;\
						*log.stream << "\tbelowZeroNum db 'ERROR: NUM IS BELOVER THAN ZERO', 0" << endl;
	#define PUSH *(log.stream) << "\tpush ";

#pragma endregion

namespace CodeGeneration
{
	void Generation(LexAnalysis::Table table, Log::LOG log);
	void AddConstSegm(LexAnalysis::Table table, Log::LOG log);
	void AddDataSegm(LexAnalysis::Table table, Log::LOG log);
	void AddCodeSegm(LexAnalysis::Table table, Log::LOG log);
}

