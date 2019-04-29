#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>

class ModelManager
{
public:
    static ModelManager& GetInstance();

    template<typename T>
    std::shared_ptr<T> Get(const std::string& name)
    {
        auto model = Get(name);
        return std::static_pointer_cast<T>(model);
    }

	template<typename T>
	std::shared_ptr<T> Get()
	{
		return Get<T>(typeid(T).name());
	}

	template<typename T>
	std::shared_ptr<T> Create(const std::string& name)
	{
		auto model = std::make_shared<T>();
		Add(name, model);
		return model;
	}

	template<typename T>
	std::shared_ptr<T> Create()
	{
		return Create<T>(typeid(T).name());
	}

	template<typename T>
	void Add(const std::string& name, std::shared_ptr<T> model)
    {
		_models.emplace(name, model);
    }

	template<typename T>
	void Remove()
    {
		Remove(typeid(T).name());
    }

	void Remove(const std::string& name);

private:
    using ModelPtr = std::shared_ptr<void>;

    ModelManager();

    ModelPtr Get(const std::string& name);

    std::unordered_map<std::string, ModelPtr> _models;
};