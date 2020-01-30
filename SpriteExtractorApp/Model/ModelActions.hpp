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
		template<typename Model, typename GetModelPolicy = ::Model::Policy::DefaultPolicy<Model>>
		class EditModel : public ICommand
		{
		public:
			EditModel(GetModelPolicy getModelPolicy = GetModelPolicy())
			: _getModelPolicy(getModelPolicy)
			{}
			
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
			GetModelPolicy _getModelPolicy;
		};
    	
        template<typename Model, typename FieldType, typename GetModelPolicy = ::Model::Policy::DefaultPolicy<Model>>
        class EditModelField : public EditModel<Model, GetModelPolicy>
        {
        public:
            EditModelField(FieldType Model::* fieldPtr, FieldType newValue, GetModelPolicy getModelPolicy = GetModelPolicy())
			: EditModel<Model, GetModelPolicy>(getModelPolicy)
            , _fieldPtr(fieldPtr)
            , _newValue(newValue)
            {
                assert(_fieldPtr);

                _oldValue = (*this->GetModel()).*fieldPtr;
            }

            ~EditModelField() override = default;

            void redo() override
            {
                (*this->GetModel()).*_fieldPtr = _newValue;
            }

            void undo() override
            {
                (*this->GetModel()).*_fieldPtr = _oldValue;
            }
        	
        private:
            FieldType Model::* _fieldPtr = nullptr;
            FieldType _newValue;
            FieldType _oldValue;
        };

        template<typename Container, typename Model, typename FieldType>
        class EditMapModel : public EditModelField<Model, FieldType, ::Model::Policy::MapPolicy<Container, Model>>
        {
        public:
            EditMapModel(FieldType Model::* fieldPtr, FieldType newValue, const std::string& key, const std::string& modelName = "")
            : EditModelField<Model, FieldType, ::Model::Policy::MapPolicy<Container, Model>>(fieldPtr, newValue, ::Model::Policy::MapPolicy<Container, Model>(key, modelName))
            {}
        };

		template<typename Model, typename ElementType, typename GetModelPolicy = ::Model::Policy::DefaultPolicy<Model>>
    	class InsertElement : public EditModel<Model, GetModelPolicy>
		{
			using FieldType = std::vector<ElementType>;
		public:
			InsertElement(FieldType Model::* fieldPtr, ElementType element, GetModelPolicy getModelPolicy = GetModelPolicy())
			: EditModel<Model, GetModelPolicy>(getModelPolicy)
			, _fieldPtr(fieldPtr)
			, _element(element)
			{
			}

			void redo() override
			{
				((*this->GetModel()).*_fieldPtr).emplace_back(_element);
			}

			void undo() override
			{
				std::shared_ptr<Model> model = this->GetModel();

				FieldType& vector = (*this->GetModel()).*_fieldPtr;
				auto it = std::find(vector.begin(), vector.end(), _element);

				if (it != vector.end())
				{
					vector.erase(it);
				}
			}
			
		private:
			FieldType Model::* _fieldPtr = nullptr;
			ElementType _element;
		};
    }
}