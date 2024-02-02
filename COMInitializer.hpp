#pragma once

#include <stdexcept>
#include <Windows.h>

// Singleton class

class COMInitializer final
{
public:
	COMInitializer()
	{
		if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		{
			throw std::runtime_error("Failed to initialize COM");
		}
	}
	virtual ~COMInitializer()
	{
		try
		{
			CoUninitialize();
		}
		catch (...) {}
	};

	COMInitializer(const COMInitializer&) = delete;
	COMInitializer& operator=(COMInitializer) = delete;

	COMInitializer(COMInitializer&&) = delete;
	COMInitializer& operator=(COMInitializer&&) = delete;
};
