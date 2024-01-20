#include <filesystem>
#include <iostream>

#include "Directory.hpp"
#include "RegistryKey.hpp"
#include "ResourceIterator.hpp"

void print_name(const FileSystemItemPtr& dir)
{
	std::wcout << dir->get_name() << std::endl;
}



int main()
{
	const std::wstring key_name = LR"(SOFTWARE\Microsoft\Unified Store)";
	const RegistryKey base_key(HKEY_CURRENT_USER, key_name);

	for (const auto& registry_item : base_key.enumerate_all())
	{
		std::wcout << registry_item->get_name() << std::endl;
	}

	auto dir = std::make_unique<Directory>(L"C:\\temp\\garden");

	ResourceIterator<Directory, FileSystemItem> dir_iterator(std::move(dir));

	dir_iterator.iterate_with_callback(print_name);
	return 0;
}
 