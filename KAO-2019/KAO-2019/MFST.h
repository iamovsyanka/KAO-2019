#pragma once

#include "GRB.h"
#include "LexAnalysis.h"
#include "Log.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define MFST_TRACE_START_LOG(LOG)	*logfile.stream << std::setfill('-') << std::endl << std::setw(25) << '-' << " ТРАССИРОВКА СИНТАКСИЧЕСКОГО АНАЛИЗА " << std::setw(27) << '-' << std::endl << std::setw(89) << '-' << std::endl \
									<< " Шаг" << " | " << std::setfill(' ') \
									<< std::setw(25) << std::left << "Правило" << " | " \
									<< std::setw(30) << std::left << "Входная лента" << " | " \
									<< std::setw(20) << std::left << "Стек" \
									<< std::endl << std::setfill('-') << std::setw(89) << '-' << std::endl;

#define MFST_TRACE1_LOG(LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << ++FST_TRACE_n << " | " \
									<< std::setfill(' ') << std::setw(25) << std::left << rule.getCRule(rbuf, nrulechain) << " | " \
									<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) << " | " \
									<< std::setw(20) << std::left << getCSt(sbuf) \
									<< std::endl;

#define MFST_TRACE2_LOG(LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << FST_TRACE_n << " | " \
									<< std::setw(28) << std::setfill(' ') << std::right << " | " \
									<< std::setw(30) << std::setfill(' ') << std::left << getCLenta(lbuf, lenta_position) << " | "\
									<< std::setw(20) << std::setfill(' ') << std::left << getCSt(sbuf) \
									<< std::endl;

#define MFST_TRACE3_LOG(LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << ++FST_TRACE_n << " | " \
									<< std::setw(28) << std::setfill(' ') << std::right << " | " \
									<< std::setw(30) << std::setfill(' ') << std::left << getCLenta(lbuf, lenta_position) << " | "\
									<< std::setw(20) << std::setfill(' ') << std::left << getCSt(sbuf) \
									<< std::endl;

#define MFST_TRACE4_LOG(c, LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << ++FST_TRACE_n << " | " << std::setfill(' ') << std::setw(25) << std::left << c << " | " << std::setw(31) << ' '  << "|" << std::endl; 
#define MFST_TRACE5_LOG(c, LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << FST_TRACE_n << " | " << std::setfill(' ') << std::setw(25) << std::left << c << " | " << std::setw(31)<< ' ' << "|" << std::endl;  
#define MFST_TRACE6_LOG(c, k, LOG)	*log.stream << std::setfill('0') << std::setw(4) << std::right << FST_TRACE_n << " | " << std::setfill(' ') << std::left << c << std::setfill('0') << std::setw(4) << std::right << k << std::setfill(' ') <<  std::setw(10) << ' ' << " | " << std::setw(32) << "|" << std::endl; 
#define MFST_TRACE7_LOG(LOG)		*log.stream << std::setfill('0') << std::setw(4) << std::right << state.lenta_position << " | " \
									<< std::setw(20) << std::setfill(' ') << std::left << rule.getCRule(rbuf, state.nrulechain) \
									<< std::endl;
#define MFST_TRACE7					std::cout << std::setfill('0') << std::setw(4) << std::right << state.lenta_position << " | " \
									<< std::setw(20) << std::setfill(' ') << std::left << rule.getCRule(rbuf, state.nrulechain) \
									<< std::endl;


typedef std::stack<short> MFSTSTACK;													// стек автомата

namespace MFST
{
	struct MfstState																	// состояние автомата (для сохранения)
	{
		short lenta_position;															// позиция на ленте
		short nrule;																	// номер текущего правила
		short nrulechain;																// номер текущей цепочки, текущего правила
		MFSTSTACK st;																	// стек автомата
		MfstState();
		MfstState(
			short pposition,															// позиция на ленте
			MFSTSTACK pst,																// стек автомата
			short pnrulechain															// номер текущей цепочки, текущего правила
		);
		MfstState(
			short pposition,															// позиция на ленте
			MFSTSTACK pst,																// стек автомата
			short pnrule,																// номер текущего правила
			short pnrulechain															// номер текущей цепочки, текущего правила
		);
	};

	struct Mfst																			// магазинный автомат
	{
		enum RC_STEP {																	// код возвтара функции step
			NS_OK,																		// найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,																	// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,																// не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,																	// неизвестный нетерминальный символ грамматики
			TS_OK,																		// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,																		// тек. символ ленты != вершине стека, восстановлено состояние
			LENTA_END,																	// текущая позиция ленты >= lenta_size
			SURPRISE																	// неожиданный код возврата (ошибка в step)
		};

		struct MfstDiagnosis															// диагностика
		{
			short lenta_position;														// позиция на ленте
			RC_STEP rc_step;															// код завершения шага
			short nrule;																// номер правила
			short nrule_chain;															// номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(
				short plenta_position,													// позиция на ленте
				RC_STEP prt_step,														// код завершения шага
				short pnrule,															// номер правила
				short pnrule_chain														// номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER];													// последние самые глубокие сообщения

		GRBALPHABET* lenta;																// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;															// текущая позиция на ленте
		short nrule;																	// номер текущего правила
		short nrulechain;																// номер текущей цепочки, текущего правила
		short lenta_size;																// размер ленты
		GRB::Greibach grebach;															// грамматика Грейбах
		LexAnalysis::Table lex;																	// результат работы лексического анализатора
		MFSTSTACK st;																	// стек автомата
		std::stack<MfstState> storestate;												// стек для сохранения состояний 
		Mfst();
		Mfst(
			LexAnalysis::Table plex,															// результат работы лексического анализатора
			GRB::Greibach pgrebach														// грамматика Грейбах
		);
		char* getCSt(char* buf);														// получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);							// лента: n символов с pos
		Error::ERROR getDiagnosis(short n);												// получить n-ю строку диагностики или 0х00
		bool savestate(Log::LOG log);													// сохранить состояние автомата
		bool reststate(Log::LOG log);													// восстановить состояние автомата
		bool push_chain(																// поместить цепочку правила в стек
			GRB::Rule::Chain chain														// цепочка правила
		);
		RC_STEP step(Log::LOG log);														// выполнить шаг автомата
		bool start(Log::LOG log);														// запустить автомат
		bool savediagnosis(
			RC_STEP pprc_step															// код завершения шага
		);
		void printrules(Log::LOG log, bool b);											// вывести последовательность правил

		struct Deducation																// вывод
		{
			short size;																	// количество шагов в выводе
			short* nrules;																// номера правил грамматики
			short* nrulechains;															// номера цепочек правик грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;

		bool savededucation();															// сохранить дерево вывода
	};
}