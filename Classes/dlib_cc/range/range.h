#ifndef DLIB_CC_RANGE_RANGE_H
#define DLIB_CC_RANGE_RANGE_H

namespace dlib_cc
{
	template <typename T> struct rect
	{
		T left;
		T bottom;
		T right;
		T top;

		rect() :left(0),bottom(0),right(0),top(0) {}
		rect(T l, T b, T r, T t) :left(l),bottom(b),right(r),top(t) {}
		T size() const { height() * width(); }
		T width() const { right - left; }
		T height() const { top - bottom; }
		void move_ver(T delta) { top += delta; bottom += delta; }
		void move_hor(T delta) { left += delta; right += delta; }
	};
} /* namespace dlib_cc */

#endif
