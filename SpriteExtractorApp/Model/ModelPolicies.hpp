#pragma once

#include <string>

#include "ModelManager.hpp"

namespace Model
{
    namespace Policy
    {
        template<typename Model>
        struct DefaultPolicy
        {
            DefaultPolicy()
            {}

            DefaultPolicy(const std::string& modelName)
            : ModelName(modelName)
            {}

            std::shared_ptr<Model> operator()()
            {
                if (ModelName.empty())
                {
                    return ModelManager::GetInstance().Get<Model>();
                }
                else
                {
                    return ModelManager::GetInstance().Get<Model>(ModelName);
                }
            }

            std::string ModelName;
        };

        template<typename Container, typename Model>
        struct MapPolicy
        {
            MapPolicy()
            {}

            MapPolicy(const std::string& key, const std::string& modelName = "")
            : Key(key)
            , ModelName(modelName)
            {
                assert(!key.empty());
            }

            std::shared_ptr<Model> operator()()
            {
                std::shared_ptr<Container> model;
                if (ModelName.empty())
                {
                    model = ModelManager::GetInstance().Get<Container>();
                }
                else
                {
                    model = ModelManager::GetInstance().Get<Container>(ModelName);
                }

                return model.get()->operator[](Key);
            }

            std::string Key;
            std::string ModelName;
        };
    }
}