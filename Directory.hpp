#pragma once
#include <filesystem>

#include "File.hpp"
#include "FileSystemItem.hpp"

class Directory;
using DirectoryPtr = std::shared_ptr<Directory>;
using DirectoryUPtr = std::unique_ptr<Directory>;

// TODO Maybe need to add a boolean to create or to open existing. Same for file class
class Directory final : public FileSystemItem
{
public:
	explicit Directory(const std::filesystem::path& path) :
		FileSystemItem(path)
	{}

	[[nodiscard]] std::vector<DirectoryPtr> enumerate_dirs() const;
	[[nodiscard]] std::vector<FilePtr> enumerate_files() const;
	[[nodiscard]] std::vector<FileSystemItemPtr> enumerate_all() const;
};

inline std::vector<DirectoryPtr> Directory::enumerate_dirs() const
{
	std::vector<DirectoryPtr> directories;

	for (const auto& file_system_item : std::filesystem::directory_iterator(m_path))
	{
		if (std::filesystem::is_directory(file_system_item))
		{
			directories.push_back(std::make_shared<Directory>(file_system_item));
		}
	}

	return directories;
}

inline std::vector<FilePtr> Directory::enumerate_files() const
{
	std::vector<FilePtr> files;

	for (const auto& file_system_item : std::filesystem::directory_iterator(m_path))
	{
		if (!std::filesystem::is_directory(file_system_item))
		{
			files.push_back(std::make_shared<File>(file_system_item));
		}
	}

	return files;
}

inline std::vector<FileSystemItemPtr> Directory::enumerate_all() const
{
	std::vector<FileSystemItemPtr> file_system_items{};

	for (const auto& directory : this->enumerate_dirs())
	{
		file_system_items.push_back(directory);
	}

	for (const auto& file : this->enumerate_files())
	{
		file_system_items.push_back(file);
	}

	return file_system_items;
}

