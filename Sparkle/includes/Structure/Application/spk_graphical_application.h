#pragma once

#include "Structure/Utils/spk_singleton.h"
#include "spk_empty_application.h"

namespace spk
{
	class GraphicalApplication : public spk::EmptyApplication, public spk::Singleton<GraphicalApplication>
	{
		friend class spk::Singleton<GraphicalApplication>;
	private:
		std::string _name;

		GraphicalApplication(std::string p_name) : spk::EmptyApplication(),
			_name(p_name)
		{

		}
	public:
		
	};
}