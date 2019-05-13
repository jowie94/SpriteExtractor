#pragma once

#include <cassert>
#include <memory>

#include "ModelManager.hpp"
#include "CommandQueue/CommandQueue.fwd.hpp"

namespace Commands
{
    namespace Model
    {
        template<typename Model, typename FieldType>
        class EditModel : public ICommand
        {
        public:
            EditModel(FieldType Model::* fieldPtr, FieldType newValue, const std::string& modelName = "")
            : _fieldPtr(fieldPtr)
            , _newValue(newValue)
            , _model(GetModel(modelName))
            {
                assert(_fieldPtr);

                _oldValue = (*_model).*fieldPtr;
            }

            ~EditModel() override = default;

            void redo() override
            {
                (*_model).*_fieldPtr = _newValue;
            }

            void undo() override
            {
                (*_model).*_fieldPtr = _oldValue;
            }

        protected:
            std::shared_ptr<Model> GetModel(const std::string& modelName)
            {
                if (modelName.empty())
                {
                    return ModelManager::GetInstance().Get<Model>();
                }
                else
                {
                    return ModelManager::GetInstance().Get<Model>(modelName);
                }
            }

            std::shared_ptr<Model> _model;

        private:
            FieldType Model::* _fieldPtr = nullptr;
            FieldType _newValue;
            FieldType _oldValue;
        };
    }
}