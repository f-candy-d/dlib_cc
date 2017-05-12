#ifndef DLIB_CC_MOBIUS_STRIP_MOBIUS_STRIP_H
#define DLIB_CC_MOBIUS_STRIP_MOBIUS_STRIP_H

#include <utility>
#include <cassert>
#include <iostream>

namespace dlib_cc
{
/**
 * this class expects that a template argument T is a numeric type (int, float, double etc...)
 */
template <typename T> struct mobius_strip
{
	T value;
	T begin;
	T end;
	T tick;

	mobius_strip() {}

	mobius_strip(T v, T b, T e)
	:value(v),begin(b),end(e),tick(1)
	{
		assert(b <= e && b <= v && v <= e);
	}

	mobius_strip(T b, T e)
	:value(b),begin(b),end(e),tick(1)
	{
		assert(b <= e);
	}

	mobius_strip(T v, T b, T e, T t)
	:value(v),begin(b),end(e),tick(t)
	{
		assert(b <= e && b <= v && v <= e);
	}

	T& operator()()
	{
		return value;
	}

	const T& operator()()
	const
	{
		return value;
	}

	mobius_strip& operator++()
	{
		advance(1);
		return *this;
	}

	mobius_strip& operator--()
	{
		advance(-1);
		return *this;
	}

	mobius_strip operator++(int)
	{
		mobius_strip old(this->value, this->begin, this->end, this->tick);
		advance(1);
		return std::move(old);
	}

	mobius_strip operator--(int)
	{
		mobius_strip old(this->value, this->begin, this->end, this->tick);
		advance(-1);
		return std::move(old);
	}

	T& advance(int adv)
	{
		value += adv * tick;
		if(end < value)
			value = begin + value - end - tick;
		else if(value < begin)
			value = end + tick - (begin - value);

		if(value < begin || end < value)
			return advance(0);
		else
			return value;
	}
};

} /* namespace dlib_cc */

#endif
