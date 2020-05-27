#pragma once
#define gl_error() DIBO::_gl_error(__FILE__, __LINE__)

namespace DIBO
{
	void  _gl_error(const char * file, int line);
}