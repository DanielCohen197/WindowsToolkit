#include <filesystem>
#include <iostream>
#include <windows.h>
#include <evntprov.h>
#include <evntrace.h>
#include <TraceLoggingProvider.h>

#include "COMInitializer.hpp"
#include "Directory.hpp"
#include "RegistryKey.hpp"
#include "ResourceIterator.hpp"
#include "ScreenCapturer.hpp"
#include "WinEventHook.hpp"

void print_name(const FileSystemItemPtr& dir)
{
	std::wcout << dir->get_name() << std::endl;
}

void iterate_registry()
{
	const std::wstring key_name = LR"(SOFTWARE\Microsoft\Unified Store)";
	auto base_key = std::make_unique<RegistryKey>(HKEY_CURRENT_USER, key_name);

	ResourceIterator<RegistryKey, IRegistryItem> registry_iterator(std::move(base_key));
}

void iterate_dir()
{
	auto dir = std::make_unique<Directory>(L"C:\\temp\\garden");

	ResourceIterator<Directory, FileSystemItem> dir_iterator(std::move(dir));

	dir_iterator.iterate_with_callback(print_name);
}

void process_action()
{
	WinEventHook([](HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD)
		{
			std::wcout << L"Process has been created" << std::endl;
		});
}

int main()
{
	ScreenCapturer capturer;

	const auto a = capturer.capture();
	Sleep(4000);
	const auto b = capturer.capture();


    return 0;
}