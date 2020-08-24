#pragma once
#ifndef _CPU_H_
#define _CPU_H_
#pragma region Includes
#pragma comment(lib,"pdh.lib")
#include <Pdh.h>
#include <string>
#include <vector>
#include <iostream>
#include <PdhMsg.h>
#include <map>
#include <sstream>

using namespace std;
#pragma endregion

class Cpu
{
#pragma region Public Funcs
public:
	Cpu();
	Cpu(const Cpu&);
	~Cpu();

	void Init();
	void ShutDown();
	void Frame();
	int GetCpuPercentage();
private:
	string getLocalizedPerfName(const map<string, string>& perfNames, const string& name);

#pragma endregion
#pragma region Private Vars
private:
	bool m_canReadcpu;
	HQUERY m_QueryHandle;
	HCOUNTER m_CountHandler;
	unsigned long m_lastsampleTime;
	long m_CpuUsage;
#pragma endregion


};
#endif 
