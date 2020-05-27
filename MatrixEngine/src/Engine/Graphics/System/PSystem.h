#pragma once

template <class T>
struct Singleton
{
	Singleton(const Singleton &) = delete;

	const Singleton &operator==(const Singleton &) = delete;

	static T* Instance()
	{
		static T * singleton = new T();
		return singleton;
	}

protected:
	Singleton() = default;
};
