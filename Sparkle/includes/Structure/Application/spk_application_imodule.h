#pragma once

namespace spk
{
	class IModule
	{
	private:

	public:
		IModule()
		{

		}

		virtual void execute() = 0;
	};
}