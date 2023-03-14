#include "sparkle.h"

class Test : public spk::IWidget
{
private:
	spk::Image* _image;

	bool _onUpdate()
	{

		return (false);
	}

	void _onGeometryChange()
	{

	}

	void _onRender()
	{
		spk::drawRectangleColor(spk::Color(50, 50, 255), 0, size(), 0);
		_image->draw(10, 100, 0, 1, 1);
	}

public:
	Test()
	{
		_image = new spk::Image("imageTest.png");
	}
};

int main()
{
	spk::GraphicalApplication* app = spk::GraphicalApplication::instanciate("Erelia", 450, spk::Color::black);

	Test* tmp = app->addWidget<Test>();
	tmp->setGeometry(0, spk::Window::instance()->size());
	tmp->activate();

	return (app->run());
}