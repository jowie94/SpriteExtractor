#pragma once

#include <cassert>
#include <memory>
#include <functional>

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
            {
                assert(_fieldPtr);

                if (modelName.empty())
                {
                    _model = ModelManager::GetInstance().Get<Model>();
                }
                else
                {
                    _model = ModelManager::GetInstance().Get<Model>(modelName);
                }

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

        private:
            FieldType Model::* _fieldPtr = nullptr;
            FieldType _newValue;
            FieldType _oldValue;

            std::shared_ptr<Model> _model;
        };
    }
}