#include "pch.h"
#include "Parm.h"

namespace Parm
{
	PARM getparm(int argc, wchar_t* argv[])
	{
		PARM parm = { L"", L"", L""};
		int len_in = wcslen(PARM_IN);
		int len_out = wcslen(PARM_OUT);
		int len_log = wcslen(PARM_LOG);
		for (int i = 0; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(101)
			else
			{
				if (!_wcsnicmp(argv[i], PARM_IN, len_in))
					wcscpy_s(parm.in, argv[i] + len_in);
				else if (!_wcsnicmp(argv[i], PARM_OUT, len_out))
					wcscpy_s(parm.out, argv[i] + len_out);
				else if (!_wcsnicmp(argv[i], PARM_LOG, len_log))
					wcscpy_s(parm.log, argv[i] + len_log);
			}
		}

		if (*parm.in == NULL)	throw ERROR_THROW(100);
		if (*parm.out == NULL)
		{
			wcscpy_s(parm.out, L"Out");
			wcsncat_s(parm.out, PARM_OUT_DEFAULT_EXT, sizeof(PARM_OUT_DEFAULT_EXT));
		}
		if (*parm.log == NULL)
		{
			wcscpy_s(parm.log, parm.in);
			wcsncat_s(parm.log, PARM_LOG_DEFAULT_EXT, sizeof(PARM_LOG_DEFAULT_EXT));
		}
		return parm;
	}
}