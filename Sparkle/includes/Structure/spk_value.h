#pragma once

#include <vector>
#include <functional>

namespace spk
{
	template<typename TType>
	class Value
	{
	public:
		class Default
		{
		private:
			TType _value;
			std::vector<Value*> _subscribedValues;

		public:
			Default() :
				_value({})
			{

			}
			Default(const TType& p_value) :
				_value(p_value)
			{

			}

			Default& operator=(const TType& p_val) {
				_value = p_val;
				for (jgl::Size_t i = 0; i < _subscribedValues.size(); i++)
				{
					_subscribedValues[i]->_notifyDefaultValueEdition();
				}

				return (*this);
			}

			TType& operator()() { return (_value); }
			const TType& operator()() const { return (_value); }

			operator TType() const { return (_value); }

			void subscribe(Value* p_value)
			{
				_subscribedValues.push_back(p_value);
			}

			void unsubscribe(Value* p_value)
			{
				auto tmp = std::find(_subscribedValues.begin(), _subscribedValues.end(), p_value);

				if (tmp != _subscribedValues.end())
				{
					_subscribedValues.erase(tmp);
				}
			}
		};

	private:
		jgl::Bool _isDefaultValue = false;
		Default* _defaultValue;
		TType _value;
		std::vector<std::function<void()>> _callbacks;

		void _executeCallback()
		{
			for (jgl::Size_t i = 0; i < _callbacks.size(); i++)
			{
				if (_callbacks[i] != nullptr)
					_callbacks[i]();
			}
		}

		void _notifyDefaultValueEdition()
		{
			_value = *_defaultValue;
			if (_isDefaultValue == true)
				_executeCallback();
		}

	public:
		Value() :
			_value(),
			_defaultValue(nullptr),
			_isDefaultValue(false)
		{

		}

		Value(const TType& p_value) :
			_value(p_value),
			_defaultValue(nullptr),
			_isDefaultValue(false)
		{

		}

		void setDefaultValue(Default* p_defaultValue)
		{
			if (_defaultValue != nullptr)
				_defaultValue->unsubscribe(this);

			_defaultValue = p_defaultValue;
			resetDefaultValue();

			if (_defaultValue != nullptr)
				_defaultValue->subscribe(this);
		}

		void resetDefaultValue()
		{
			if (_defaultValue == nullptr)
				return;
			_value = *_defaultValue;
			_isDefaultValue = true;
			_executeCallback();
		}

		Value& operator=(const TType& p_val) {
			_value = p_val;
			_isDefaultValue = false;
			_executeCallback();
			return (*this);
		}

		operator TType() const { return (_value); }

		TType& value() { return (_value); }
		const TType& value() const { return (_value); }

		TType& operator()() { return _value; }
		const TType& operator()() const { return _value; }

		template<typename Func, typename... Args>
		void addCallback(Func&& p_func, Args&&... p_args)
		{
			_callbacks.push_back(std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...));
		}

		void cleanCallbacks()
		{
			_callbacks.clear();
		}
	};
}