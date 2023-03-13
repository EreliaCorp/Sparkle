#include "sparkle.h"

int main()
{
	spk::GraphicalApplication* app = spk::GraphicalApplication::instanciate("Erelia", 450, spk::Color::blue);

	return (app->run());
}