#include "sparkle.h"

int main()
{
	spk::Application* app = spk::Application::instanciate(spk::Application::Configuration::Graphical);

	return (app->run());
}