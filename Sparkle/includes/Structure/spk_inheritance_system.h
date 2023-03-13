#pragma once

#include <vector>

namespace spk
{
	template<typename TType>
	class InheritanceSystem {
	private:
		TType* _parent;
		std::vector<TType*> _children;

	public:
		InheritanceSystem() :
			_parent(nullptr),
			_children()
		{
		
		}

		void addChild(TType* child) {
			if (child != nullptr)
			{
				if (child->_parent != nullptr)
				{
					child->_parent->removeChild(child);
				}
				_children.push_back(child);
				child->_parent = static_cast<TType*>(this);
			}
		}

		void removeChild(TType* child) {
			if (child != nullptr)
			{
				_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
				child->_parent = nullptr;
			}
		}

		std::vector<TType*>& childrens() { return (_children); }
		const std::vector<TType*>& childrens() const { return (_children); }

		TType* parent() { return (_parent); }
		const TType* parent() const { return (_parent); }
	};
}