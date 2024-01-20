#pragma once
#include <filesystem>
#include <memory>
#include <string>

class FileSystemItem
{
public:
	virtual ~FileSystemItem() = default;

	FileSystemItem(const FileSystemItem&) = delete;
	FileSystemItem& operator=(FileSystemItem) = delete;

	FileSystemItem(FileSystemItem&&) = delete;
	FileSystemItem& operator=(FileSystemItem&&) = delete;

	[[nodiscard]] std::wstring get_name() const;

	const std::filesystem::path m_path;

protected:
	explicit FileSystemItem(const std::filesystem::path& path) :
		m_path(path)
	{}
};

inline std::wstring FileSystemItem::get_name() const
{
	return m_path;
}

using FileSystemItemPtr = std::shared_ptr<FileSystemItem>;
using FileSystemItemUPtr = std::unique_ptr<FileSystemItem>;
