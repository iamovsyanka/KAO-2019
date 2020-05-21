#pragma once

#include "GRB.h"
#include "LexAnalysis.h"
#include "Log.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define MFST_TRACE_START_LOG(LOG)	*logfile.stream << std::setfill('-') << std::endl << std::setw(25) << '-' << " ����������� ��������������� ������� " << std::setw(27) << '-' << std::endl << std::setw(89) << '-' << std::endl \
									<< " ���" << " | " << std::setfill(' ') \
									<< std::setw(25) << std::left << "�������" << " | " \
									<< std::setw(30) << std::left << "������� �����" << " | " \
									<< std::setw(20) << std::left << "����" \
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


typedef std::stack<short> MFSTSTACK;													// ���� ��������

namespace MFST
{
	struct MfstState																	// ��������� �������� (��� ����������)
	{
		short lenta_position;															// ������� �� �����
		short nrule;																	// ����� �������� �������
		short nrulechain;																// ����� ������� �������, �������� �������
		MFSTSTACK st;																	// ���� ��������
		MfstState();
		MfstState(
			short pposition,															// ������� �� �����
			MFSTSTACK pst,																// ���� ��������
			short pnrulechain															// ����� ������� �������, �������� �������
		);
		MfstState(
			short pposition,															// ������� �� �����
			MFSTSTACK pst,																// ���� ��������
			short pnrule,																// ����� �������� �������
			short pnrulechain															// ����� ������� �������, �������� �������
		);
	};

	struct Mfst																			// ���������� �������
	{
		enum RC_STEP {																	// ��� �������� ������� step
			NS_OK,																		// ������� ������� � �������, ������� �������� � ����
			NS_NORULE,																	// �� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,																// �� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,																	// ����������� �������������� ������ ����������
			TS_OK,																		// ���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,																		// ���. ������ ����� != ������� �����, ������������� ���������
			LENTA_END,																	// ������� ������� ����� >= lenta_size
			SURPRISE																	// ����������� ��� �������� (������ � step)
		};

		struct MfstDiagnosis															// �����������
		{
			short lenta_position;														// ������� �� �����
			RC_STEP rc_step;															// ��� ���������� ����
			short nrule;																// ����� �������
			short nrule_chain;															// ����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(
				short plenta_position,													// ������� �� �����
				RC_STEP prt_step,														// ��� ���������� ����
				short pnrule,															// ����� �������
				short pnrule_chain														// ����� ������� �������
			);
		} diagnosis[MFST_DIAGN_NUMBER];													// ��������� ����� �������� ���������

		GRBALPHABET* lenta;																// ���������������� (TS/NS) ����� (�� LEX)
		short lenta_position;															// ������� ������� �� �����
		short nrule;																	// ����� �������� �������
		short nrulechain;																// ����� ������� �������, �������� �������
		short lenta_size;																// ������ �����
		GRB::Greibach grebach;															// ���������� �������
		LexAnalysis::Table lex;																	// ��������� ������ ������������ �����������
		MFSTSTACK st;																	// ���� ��������
		std::stack<MfstState> storestate;												// ���� ��� ���������� ��������� 
		Mfst();
		Mfst(
			LexAnalysis::Table plex,															// ��������� ������ ������������ �����������
			GRB::Greibach pgrebach														// ���������� �������
		);
		char* getCSt(char* buf);														// �������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);							// �����: n �������� � pos
		Error::ERROR getDiagnosis(short n);												// �������� n-� ������ ����������� ��� 0�00
		bool savestate(Log::LOG log);													// ��������� ��������� ��������
		bool reststate(Log::LOG log);													// ������������ ��������� ��������
		bool push_chain(																// ��������� ������� ������� � ����
			GRB::Rule::Chain chain														// ������� �������
		);
		RC_STEP step(Log::LOG log);														// ��������� ��� ��������
		bool start(Log::LOG log);														// ��������� �������
		bool savediagnosis(
			RC_STEP pprc_step															// ��� ���������� ����
		);
		void printrules(Log::LOG log, bool b);											// ������� ������������������ ������

		struct Deducation																// �����
		{
			short size;																	// ���������� ����� � ������
			short* nrules;																// ������ ������ ����������
			short* nrulechains;															// ������ ������� ������ ���������� (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;

		bool savededucation();															// ��������� ������ ������
	};
}