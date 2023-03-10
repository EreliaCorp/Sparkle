#include "sparkle.h"

namespace spk
{
	thread_local spk::IOStream cout = spk::IOStream(std::cout);
	thread_local spk::IOStream cerr = spk::IOStream(std::cerr);

    const Color Color::transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
    const Color Color::white = Color(1.0f, 1.0f, 1.0f, 1.0f);
    const Color Color::black = Color(0.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::red = Color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::green = Color(0.0f, 1.0f, 0.0f, 1.0f);
    const Color Color::blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
}