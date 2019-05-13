#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>

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

	template<typename T, typename... Args>
	std::shared_ptr<T> CreateWithName(const std::string& name, Args&&... args)
	{
		auto model = std::make_shared<T>(std::forward<Args>(args)...);
		Add(name, model);
		return model;
	}

	template<typename T, typename... Args>
	std::shared_ptr<T> Create(Args&&... args)
	{
		return CreateWithName<T, Args...>(typeid(T).name(), std::forward<Args>(args)...);
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
    ~ModelManager();

    ModelPtr Get(const std::string& name);

    std::unordered_map<std::string, ModelPtr> _models;
};