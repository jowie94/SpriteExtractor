#pragma once

#include <cassert>
#include <memory>

#include "ModelManager.hpp"
#include "ModelPolicies.hpp"
#include "CommandQueue/CommandQueue.fwd.hpp"

namespace Commands
{
    namespace Model
    {
        template<typename Model, typename FieldType, typename GetModelPolicy = ::Model::Policy::DefaultPolicy<Model>>
        class EditModel : public ICommand
        {
        public:
            EditModel(FieldType Model::* fieldPtr, FieldType newValue, GetModelPolicy getModelPolicy = GetModelPolicy())
            : _fieldPtr(fieldPtr)
            , _newValue(newValue)
            , _getModelPolicy(getModelPolicy)
            {
                assert(_fieldPtr);

                _oldValue = (*GetModel()).*fieldPtr;
            }

            ~EditModel() override = default;

            void redo() override
            {
                (*GetModel()).*_fieldPtr = _newValue;
            }

            void undo() override
            {
                (*GetModel()).*_fieldPtr = _oldValue;
            }

        protected:
            std::shared_ptr<Model> GetModel()
            {
                if (!_model)
                {
                    _model = _getModelPolicy();
                }

                return _model;
            }

        private:
            std::shared_ptr<Model> _model;
            FieldType Model::* _fieldPtr = nullptr;
            FieldType _newValue;
            FieldType _oldValue;
            GetModelPolicy _getModelPolicy;
        };

        template<typename Container, typename Model, typename FieldType>
        class EditMapModel : public EditModel<Model, FieldType, ::Model::Policy::MapPolicy<Container, Model>>
        {
        public:
            EditMapModel(FieldType Model::* fieldPtr, FieldType newValue, const std::string& key, const std::string& modelName = "")
            : EditModel(fieldPtr, newValue, ::Model::Policy::MapPolicy<Container, Model>(key, modelName))
            {}
        };
    }
}