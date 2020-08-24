#include "Cpu.h"

Cpu::Cpu()
{
}
Cpu::Cpu(const Cpu&)
{
}
Cpu::~Cpu()
{
}

void Cpu::Init()
{
	PDH_STATUS err;

	m_canReadcpu = true;

	err = PdhOpenQuery(NULL, 0, &m_QueryHandle);
	if (err != ERROR_SUCCESS)
		m_canReadcpu = false;
	vector<unsigned char> buffer;
	unsigned long size = 32768;
	buffer.resize(size);
	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, TEXT("Counter 09"), 0, 0, &buffer[0], &size) == ERROR_MORE_DATA)
	{
		size += 8192;
		buffer.resize(size);
	}
	map<string, string> perfNames;
	const char* buf = reinterpret_cast<const char*>(&buffer[0]);


	int i = 0;
	while (i < buffer.size() && buf[i])
	{
		string index(&buf[i]);
		i += static_cast<int>(index.size()) + 1;
		if (i >= buffer.size())
		{
			break;
		}
		string name(&buf[i]);
		i += static_cast<int>(name.size()) + 1;
		perfNames.insert(make_pair(name, index));
	}
	
	//
	// Get the localized version of "Processor" and "%Processor Time"
	//
	string proc = getLocalizedPerfName(perfNames, "Processor");
	string proctime = getLocalizedPerfName(perfNames, "% Processor Time");

	//
	// Add the counter
	//
	const string name = '\\' + proc + "(_Total)\\" + proctime;
		err = PdhAddCounter(m_QueryHandle,(LPCWSTR)name.c_str(), 0, &m_CountHandler);
	if (err != ERROR_SUCCESS)
			m_canReadcpu = false;
	
	m_lastsampleTime = GetTickCount();

	return;
}


/*string getLocalizedPerfName(const map<string, string>& perfNames, const string& name)
{
	unsigned long idx;
	map<string, string>::const_iterator p = perfNames.find(name);
	if (p == perfNames.end())
	{
		return "";
	}
	istringstream is(p->second);
	is >> idx;

	vector<char> localized;
	unsigned long size = 256;
	localized.resize(size);
	while (PdhLookupPerfNameByIndex(0, idx, (LPWSTR)localized[0], &size) == PDH_MORE_DATA)
	{
		size += 256;
		localized.resize(size);
	}
	return string(&localized[0]);
}*/

void Cpu::ShutDown()
{
	if (m_canReadcpu)
		PdhCloseQuery(m_QueryHandle);
	return;
}

void Cpu::Frame()
{
	PDH_FMT_COUNTERVALUE value;
	if(m_canReadcpu)
	{
		if((m_lastsampleTime + 1000)<GetTickCount())
		{
			m_lastsampleTime = GetTickCount();
			PdhCollectQueryData(m_QueryHandle);
			PdhGetFormattedCounterValue(m_CountHandler, PDH_FMT_LONG, NULL, &value);
			m_CpuUsage = value.longValue;
		}
	}
	return;
}

int Cpu::GetCpuPercentage()
{
	int usage;
	if (m_canReadcpu)
		usage = (int)m_CpuUsage;
	else
	{
		usage = -1;
	}
	

	return usage;
}

string Cpu::getLocalizedPerfName(const map<string, string>& perfNames, const string& name)
{
	unsigned long idx;
	map<string, string>::const_iterator p = perfNames.find(name);
	if (p == perfNames.end())
	{
		return "";
	}
	istringstream is(p->second);
	is >> idx;

	vector<char> localized;
	unsigned long size = 256;
	localized.resize(size);
	while (PdhLookupPerfNameByIndex(0, idx, (LPWSTR)localized[0], &size) == PDH_MORE_DATA)
	{
		size += 256;
		localized.resize(size);
	}
	return string(&localized[0]);
}
