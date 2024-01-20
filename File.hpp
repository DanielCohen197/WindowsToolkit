#pragma once
#include <filesystem>

#include "FileSystemItem.hpp"

class File final : public FileSystemItem
{
public:
	explicit File(const std::filesystem::path& path) :
		FileSystemItem(path)
	{}
};

using FilePtr = std::shared_ptr<File>;
using FileUPtr = std::unique_ptr<File>;
