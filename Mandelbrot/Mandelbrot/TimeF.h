#ifndef TimeF_H
#define TimeF_H

#include <chrono>
template<typename F>
auto timeFunction(F f)
{
	auto start = std::chrono::steady_clock::now();
	f();
	auto end = std::chrono::steady_clock::now();
	return end - start;
}

#endif