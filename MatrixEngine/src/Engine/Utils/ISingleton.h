#pragma once

#ifndef ISINGLETON_H
#define ISINGLETON_H

/*!
\struct ISingleton
\brief	Base class for creating singleton classes.
*/
template <class T>
struct ISingleton
{
	// not copyable
	ISingleton(const ISingleton &) = delete;
	const ISingleton & operator=(const ISingleton &) = delete;

	// public interface
	static T * Instance()
	{
		static T * singleton = new T();
		return singleton;
	}

protected:
	// only with the getInstance function can be instantiable
	ISingleton() = default;

};	// struct ISingleton

#endif // !ISINGLETON_H