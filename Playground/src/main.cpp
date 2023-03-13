#include "sparkle.h"

class GraphicalApplication : public spk::Application, public spk::Singleton<GraphicalApplication>
{
	friend class spk::Singleton<GraphicalApplication>;
private:
	GraphicalApplication(std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor)
	{

	}

public:

};

int main()
{
	GraphicalApplication* app = GraphicalApplication::instanciate("Erelia", 450, spk::Color::blue);

	return (app->run());
}