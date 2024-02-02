#pragma once
#include <stdexcept>
#include <Windows.h>

class WinEventHook final
{
public:
	explicit WinEventHook(WINEVENTPROC callback) :
		m_handle(_s_create_hook(callback))
	{}
	virtual ~WinEventHook()
	{
		try
		{
			UnhookWinEvent(m_handle);
		}
		catch (...)
		{}
	};

	WinEventHook(const WinEventHook&) = delete;
	WinEventHook& operator=(WinEventHook) = delete;

	WinEventHook(WinEventHook&&) = delete;
	WinEventHook& operator=(WinEventHook&&) = delete;

private:
	static HWINEVENTHOOK _s_create_hook(WINEVENTPROC callback);

	const HWINEVENTHOOK m_handle;
};

inline HWINEVENTHOOK WinEventHook::_s_create_hook(WINEVENTPROC callback)
{
	const auto handle = SetWinEventHook(
		EVENT_OBJECT_CREATE,
		EVENT_OBJECT_CREATE,
		nullptr,
		callback,
		0,
		0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	);

	if (!handle)
	{
		throw std::runtime_error("Failed to create WinEventHook");
	}

	return handle;
}
