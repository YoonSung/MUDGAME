#pragma once
class CLogSaver
{
public:
	CLogSaver(void);
	~CLogSaver(void);
	void AddLog(char* logString);
	void PrintEntireLog();
public:
	std::string m_logs;
};

