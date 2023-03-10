#include "sparkle.h"

#include <iostream>
#include <vector>
#include <Windows.h>

int main()
{
	spk::cout.setPrefix("Main");

	spk::GraphicalApplication* app = spk::GraphicalApplication::instanciate("Playground");

	return (app->run());
}