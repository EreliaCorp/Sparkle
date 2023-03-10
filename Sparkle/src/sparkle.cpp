#include "sparkle.h"

namespace spk
{
	thread_local spk::IOStream cout = spk::IOStream(std::cout);
	thread_local spk::IOStream cerr = spk::IOStream(std::cerr);
}