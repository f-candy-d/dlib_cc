#ifndef DLIB_CC_VEC2_VEC2_H
#define DLIB_CC_VEC2_VEC2_H

namespace dlib_cc
{
	template <typename T> struct vec2
	{
		T x;
		T y;

		vec2() :x(0),y(0) {}
		vec2(T p, T q) :x(p),y(q) {}
	};

} /* namespace dlib_cc */

#endif
