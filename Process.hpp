#pragma once

class Process
{
public:
	Process() :
	m_process_handle()

	virtual ~Process() = default;

	Process(const Process&) = delete;
	Process& operator=(Process) = delete;

	Process(Process&&) = delete;
	Process& operator=(Process&&) = delete;

private:
	
};
