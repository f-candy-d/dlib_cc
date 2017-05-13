#ifndef DLIB_CC_RANGE_RANGE_H
#define DLIB_CC_RANGE_RANGE_H

namespace dlib_cc
{
	template <typename T> struct rect
	{
		T top;
		T bottom;
		T left;
		T right;

		rect() :top(0),bottom(0),left(0),right(0) {}
		rect(T t, T b, T l, T r) :top(t),bottom(b),left(l),right(r) {}
		T size() const { height() * width(); }
		T width() const { right - left; }
		T height() const { top - bottom; }
		void move_ver(T delta) { top += delta; bottom += delta; }
		void move_hor(T delta) { left += delta; right += delta; }
	};
} /* namespace dlib_cc */

#endif
