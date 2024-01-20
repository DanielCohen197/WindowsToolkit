#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "IRegistryItem.hpp"
#include "RegistryValue.hpp"

class RegistryKey;
using RegistryKeyPtr = std::shared_ptr<RegistryKey>;
using RegistryKeyUPtr = std::unique_ptr<RegistryKey>;

class RegistryKey final : public IRegistryItem
{
public:
	RegistryKey(const HKEY main_key, const std::wstring& key_name) :
		m_handle(_s_open_key(main_key, key_name)),
		m_name(key_name)
	{}
	virtual ~RegistryKey()
	{
		try
		{
			if (m_handle != nullptr)
			{
				RegCloseKey(m_handle);
				m_handle = nullptr;

			}
		}
		catch (...) {}
	}

	RegistryKey(const RegistryKey&) = delete;
	RegistryKey& operator=(RegistryKey) = delete;

	RegistryKey(RegistryKey&&) = delete;
	RegistryKey& operator=(RegistryKey&&) = delete;

	[[nodiscard]] std::wstring get_name() override;

	[[nodiscard]] std::vector<RegistryKeyPtr> enumerate_keys() const;
	[[nodiscard]] std::vector<RegistryValuePtr> enumerate_values() const;
	[[nodiscard]] std::vector<IRegistryItemPtr> enumerate_all() const;

private:
	static HKEY _s_open_key(const HKEY main_key, const std::wstring& key_name)
	{
		HKEY registry_key = nullptr;
		const auto status = RegOpenKeyW(main_key, key_name.c_str(), &registry_key);
		if (status != ERROR_SUCCESS)
		{
			throw std::runtime_error("OpenRegKeyW Failed");
		}

		return registry_key;
	}

private:
	HKEY m_handle;
	const std::wstring m_name;
};


inline std::wstring RegistryKey::get_name()
{

	return m_name;
}

inline std::vector<RegistryKeyPtr> RegistryKey::enumerate_keys() const
{
	std::vector<RegistryKeyPtr> keys;

	DWORD sub_key_count;
	DWORD max_sub_key_length;

	if (RegQueryInfoKeyW(
		m_handle,
		nullptr,
		nullptr,
		nullptr,
		&sub_key_count,
		&max_sub_key_length,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	) != ERROR_SUCCESS) {
		throw std::runtime_error("Failed to query registry key information");
	}

	for (DWORD i = 0; i < sub_key_count; ++i)
	{
		std::wstring sub_key_name;
		sub_key_name.resize(max_sub_key_length + 1);
		DWORD sub_key_name_length = sub_key_name.size();
		const auto status = RegEnumKeyExW(
			m_handle,
			i,
			&sub_key_name[0],
			&sub_key_name_length,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);

		if (status != ERROR_SUCCESS)
		{
			throw std::runtime_error("Failed to enumerate registry keys");
		}
		keys.push_back(std::make_shared<RegistryKey>(m_handle, sub_key_name));
	}

	return keys;
}

inline std::vector<RegistryValuePtr> RegistryKey::enumerate_values() const
{
	std::vector<RegistryValuePtr> result;

	DWORD value_count;
	DWORD max_value_length;

	if (RegQueryInfoKeyW(
		m_handle,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		&value_count,
		&max_value_length,
		nullptr,
		nullptr,
		nullptr
	) != ERROR_SUCCESS) {
		throw std::runtime_error("Failed to query registry key information");
	}


	for (DWORD i = 0; i < value_count; ++i)
	{
		std::wstring value_name;
		value_name.resize(max_value_length + 1);
		DWORD value_name_length = value_name.size();

		const auto status = RegEnumValueW(
			m_handle,
			i,
			&value_name[0],
			&value_name_length,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);

		if (status != ERROR_SUCCESS)
		{
			throw std::runtime_error("Failed to enumerate registry values");
		}
		result.push_back(std::make_shared<RegistryValue>(value_name));
	}

	return result;
}

inline std::vector<IRegistryItemPtr> RegistryKey::enumerate_all() const
{
	std::vector<IRegistryItemPtr> registry_items{};

	for (const auto& registry_key : this->enumerate_keys())
	{
		registry_items.push_back(registry_key);
	}

	for (const auto& registry_value : this->enumerate_values())
	{
		registry_items.push_back(registry_value);
	}

	return registry_items;
}
