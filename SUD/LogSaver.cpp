#include "stdafx.h"
#include "LogSaver.h"


void CLogSaver::AddLog(char* logString)
{
	int logNum = m_logs.length();

	if ( logNum >= 10 )
	{
		m_logs.erase(0,0);

	}
	m_logs.append(logString);
}

void CLogSaver::PrintEntireLog()
{
	m_logs.append("==========================================");
	for ( int i = 0 ; i < m_logs.length() ; ++i )
	{
		//have to do;
		;
	}
}

CLogSaver::CLogSaver(void)
{
	m_logs.append("==========================================");
}


CLogSaver::~CLogSaver(void)
{
}
