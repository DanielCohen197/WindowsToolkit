#pragma once

#include <functional>

// TODO Add enable ifs (?) to make sure the type 
template <typename T, typename U>
class ResourceIterator final
{
public:
	explicit ResourceIterator(std::unique_ptr<T>&& main_resource) :
		m_main_resource(std::move(main_resource))
	{}
	virtual ~ResourceIterator() = default;

	ResourceIterator(const ResourceIterator&) = delete;
	ResourceIterator& operator=(ResourceIterator) = delete;

	ResourceIterator(ResourceIterator&&) = delete;
	ResourceIterator& operator=(ResourceIterator&&) = delete;

	void iterate_with_callback(std::function<void(const std::shared_ptr<U>&)> callback)
	{
		for (const auto& resource : m_main_resource->enumerate_all())
		{
			callback(resource);

			if (dynamic_cast<T*>(resource.get()))
			{
				std::unique_ptr<T> sub_resource = std::make_unique<T>(resource->get_name());
				ResourceIterator<T, U> sub_iterator(std::move(sub_resource));
				sub_iterator.iterate_with_callback(callback);
			}
		}
	}

private:
	std::unique_ptr<T> m_main_resource;
};
