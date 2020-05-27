#pragma once

#include <future>

template<typename T> inline
bool ThreadIsReady(std::future<T> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}