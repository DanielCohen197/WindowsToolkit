#pragma once

class IRegistryItem
{
public:
	[[nodiscard]] virtual std::wstring get_name() = 0;
};

using IRegistryItemPtr = std::shared_ptr<IRegistryItem>;
