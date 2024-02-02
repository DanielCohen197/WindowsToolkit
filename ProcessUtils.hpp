#pragma once

#include <iostream>
#include <Windows.h>
#include <string>

namespace ProcessUtils
{
	void notify_process_creation(const std::wstring& process_name)
	{
		SetWinEventHook(
			EVENT_OBJECT_CREATE,
			EVENT_OBJECT_CREATE,
			nullptr,
			[](HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD)
			{
				std::wcout << L"Process " << process_name << L" has been created" << std::endl;
			},
			0,
			0,
			WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS;
		);
	}
}
