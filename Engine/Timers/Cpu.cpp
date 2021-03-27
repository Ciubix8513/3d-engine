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
	std::vector<unsigned char> buffer;
	unsigned long size = 32768;
	buffer.resize(size);
	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, TEXT("Counter 09"), 0, 0, &buffer[0], &size) == ERROR_MORE_DATA)
	{
		size += 8192;
		buffer.resize(size);
	}
	map<std::string, std::string> perfNames;
	const char* buf = reinterpret_cast<const char*>(&buffer[0]);


	int i = 0;
	while (i < buffer.size() && buf[i])
	{
		std::string index(&buf[i]);
		i += static_cast<int>(index.size()) + 1;
		if (i >= buffer.size())
		{
			break;
		}
		std::string name(&buf[i]);
		i += static_cast<int>(name.size()) + 1;
		perfNames.insert(make_pair(name, index));
	}
	
	//
	// Get the localized version of "Processor" and "%Processor Time"
	//
	std::string proc = getLocalizedPerfName(perfNames, "Processor");
	std::string proctime = getLocalizedPerfName(perfNames, "% Processor Time");

	//
	// Add the counter
	//
	const std::string name = '\\' + proc + "(_Total)\\" + proctime;
		err = PdhAddCounter(m_QueryHandle,(LPCWSTR)name.c_str(), 0, &m_CountHandler);
	if (err != ERROR_SUCCESS)
			m_canReadcpu = false;
	
	m_lastsampleTime = GetTickCount64();

	return;
}


/*std::string getLocalizedPerfName(const map<std::string, std::string>& perfNames, const std::string& name)
{
	unsigned long idx;
	map<std::string, std::string>::const_iterator p = perfNames.find(name);
	if (p == perfNames.end())
	{
		return "";
	}
	istd::stringstream is(p->second);
	is >> idx;

	std::vector<char> localized;
	unsigned long size = 256;
	localized.resize(size);
	while (PdhLookupPerfNameByIndex(0, idx, (LPWSTR)localized[0], &size) == PDH_MORE_DATA)
	{
		size += 256;
		localized.resize(size);
	}
	return std::string(&localized[0]);
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
		if((m_lastsampleTime + 1000)<GetTickCount64())
		{
			m_lastsampleTime = GetTickCount64();
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

std::string Cpu::getLocalizedPerfName(const map<std::string, std::string>& perfNames, const std::string& name)
{
	unsigned long idx;
	map<std::string, std::string>::const_iterator p = perfNames.find(name);
	if (p == perfNames.end())
	{
		return "";
	}
	istringstream is(p->second);
	is >> idx;

	std::vector<char> localized;
	unsigned long size = 256;
	localized.resize(size);
	while (PdhLookupPerfNameByIndex(0, idx, (LPWSTR)localized[0], &size) == PDH_MORE_DATA)
	{
		size += 256;
		localized.resize(size);
	}
	return std::string(&localized[0]);
}
