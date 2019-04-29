#include "ModelManager.hpp"

#include <cassert>

ModelManager& ModelManager::GetInstance()
{
	static ModelManager instance;
	return instance;
}

void ModelManager::Remove(const std::string& name)
{
	_models.erase(name);
}

ModelManager::ModelManager()
{
}

ModelManager::ModelPtr ModelManager::Get(const std::string& name)
{
    auto it = _models.find(name);

    if (it != _models.end())
    {
        return it->second;
    }

    assert(!"Model doesn't exist");
    return nullptr;
}
