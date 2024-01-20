#pragma once
#include <memory>

class RegistryValue final : public IRegistryItem
{
public:
	explicit RegistryValue(const std::wstring& name) :
		m_name(name)
	{}
	virtual ~RegistryValue() = default;

	RegistryValue(const RegistryValue&) = delete;
	RegistryValue& operator=(RegistryValue) = delete;

	RegistryValue(RegistryValue&&) = delete;
	RegistryValue& operator=(RegistryValue&&) = delete;

	[[nodiscard]] std::wstring get_name() override { return m_name; }

private:
	const std::wstring m_name;
};

using RegistryValuePtr = std::shared_ptr<RegistryValue>;
