/**********************************************************************************
 *
 * dlib_cc::fast_shift_table class
 *
 **********************************************************************************/
#ifndef DLIB_CC_FAST_SHIFT_TABLE_IMPL_H
#define DLIB_CC_FAST_SHIFT_TABLE_IMPL_H

#include "fast_shift_table.h"

namespace dlib_cc
{
template <typename T>
fast_shift_table<T>::fast_shift_table()
:width_(0)
,height_(0)
,row_index_origin_(0, 0)
,column_index_origin_(0, 0)
{}

template <typename T>
fast_shift_table<T>::fast_shift_table(int w, int h)
:width_(w)
,height_(h)
,row_index_origin_(0, w - 1)
,column_index_origin_(0, h - 1)
,buffer_(w * h)
{
	assert(0 <= w && 0 <= h);
}

template <typename T>
fast_shift_table<T>::fast_shift_table(int w, int h, int f)
:width_(w)
,height_(h)
,row_index_origin_(0, w - 1)
,column_index_origin_(0, h - 1)
,buffer_(w * h, f)
{
	assert(0 <= w && 0 <= h);
}

template <typename T>
void fast_shift_table<T>::resize(int w, int h)
{
	assert(0 <= w && 0 <= h);
	buffer_.resize(w * h);
	width_ = w;
	height_ = h;
	row_index_origin_.begin = 0;
	column_index_origin_.begin = 0;
	row_index_origin_.end = w - 1;
	column_index_origin_.end = h - 1;
}

template <typename T>
void fast_shift_table<T>::resize(int w, int h, int f)
{
	assert(0 <= w && 0 <= h);
	buffer_.resize(w * h, f);
	width_ = w;
	height_ = h;
	row_index_origin_.begin = 0;
	column_index_origin_.begin = 0;
	row_index_origin_.end = w - 1;
	column_index_origin_.end = h - 1;
}

template <typename T>
int fast_shift_table<T>::size()
const
{
	return width_ * height_;
}

template <typename T>
const T& fast_shift_table<T>::read(int col, int row)
const
{
	return buffer_[to_index(col, row)];
}

template <typename T>
void fast_shift_table<T>::write(int col, int row, T val)
{
	buffer_[to_index(col, row)] = val;
}

template <typename T>
void fast_shift_table<T>::shift_columns(int distance)
{
	column_index_origin_.advance(-1 * distance);
}

template <typename T>
void fast_shift_table<T>::shift_rows(int distance)
{
	row_index_origin_.advance(-1 * distance);
}

template <typename T>
int fast_shift_table<T>::to_index(int col, int row)
const
{
	assert(0 <= row && row < height_);
	assert(0 <= col && col < width_);

	dlib_cc::mobius_strip<int> index(row_index_origin_() * width_, 0, width_ * (height_ - 1), width_);
	index.advance(row);

	index.tick = 1;
	index.begin = index();
	index.end = index.begin + width_ - 1;
	index.advance(column_index_origin_() + col);

	return index();
}

/**********************************************************************************
 *
 * GETTING ITERATOR METHODS OF DLIB_CC::FAST_SHIFT_TABLE CLASS
 *
 **********************************************************************************/
template <typename T>
using difference_type = typename fast_shift_table<T>::difference_type;
template <typename T>
using size_type = typename fast_shift_table<T>::size_type;
template <typename T>
using value_type = typename fast_shift_table<T>::value_type;
template <typename T>
using pointer = typename fast_shift_table<T>::pointer;
template <typename T>
using const_pointer = typename fast_shift_table<T>::const_pointer;
template <typename T>
using reference = typename fast_shift_table<T>::reference;
template <typename T>
using const_reference = typename fast_shift_table<T>::const_reference;

// template <typename T>
// typename std::vector<T>::iterator fast_shift_table<T>::begin()
template <typename T>
typename fast_shift_table<T>::unordered_iterator fast_shift_table<T>::begin()
{
	return buffer_.begin();
}

template <typename T>
typename fast_shift_table<T>::unordered_iterator fast_shift_table<T>::end()
{
	return buffer_.end();
}

template <typename T>
typename fast_shift_table<T>::const_unordered_iterator fast_shift_table<T>::begin()
const
{
	return buffer_.cbegin();
}

template <typename T>
typename fast_shift_table<T>::const_unordered_iterator fast_shift_table<T>::end()
const
{
	return buffer_.cend();
}

template <typename T>
typename fast_shift_table<T>::row_iterator fast_shift_table<T>::begin_row(int row)
{
	assert(0 <= row && row < height_);

	auto raw_row = row_index_origin_;
	raw_row.advance(row);
	pointer zero = &buffer_[raw_row() * width_];
	return row_iterator(
		zero + column_index_origin_(),
		zero + column_index_origin_(),
		zero + width_,
		zero,
		0,
		row,
		this);
}

template <typename T>
typename fast_shift_table<T>::row_iterator fast_shift_table<T>::end_row(int row)
{
	auto itr = begin_row(row);
	itr.base = itr.zero + width_;
	itr.position.x = width_;
	return itr;
}

template <typename T>
typename fast_shift_table<T>::const_row_iterator fast_shift_table<T>::begin_row(int row)
const
{
	assert(0 <= row && row < height_);

	auto raw_row = row_index_origin_;
	raw_row.advance(row);
	const_pointer zero = &buffer_[raw_row() * width_];
	return const_row_iterator(
		zero + column_index_origin_(),
		zero + column_index_origin_(),
		zero + width_,
		zero,
		0,
		row,
		this);
}

template <typename T>
typename fast_shift_table<T>::const_row_iterator fast_shift_table<T>::end_row(int row)
const
{
	auto itr = begin_row(row);
	itr.base = itr.zero + width_;
	itr.position.x = width_;
	return itr;
}

template <typename T>
typename fast_shift_table<T>::column_iterator fast_shift_table<T>::begin_column(int col)
{
	assert(0 <= col && col < width_);

	auto raw_col = column_index_origin_;
	raw_col.advance(col);
	pointer zero = &buffer_[raw_col()];
	return column_iterator(
		zero + row_index_origin_() * width_,
		zero + row_index_origin_() * width_,
		zero + width_ * height_,
		zero,
		col,
		0,
		this);
}

template <typename T>
typename fast_shift_table<T>::column_iterator fast_shift_table<T>::end_column(int col)
{
	auto itr = begin_column(col);
	itr.base = itr.zero + width_ * height_;
	itr.position.y = height_;
	return itr;
}

template <typename T>
typename fast_shift_table<T>::const_column_iterator fast_shift_table<T>::begin_column(int col)
const
{
	assert(0 <= col && col < width_);

	auto raw_col = column_index_origin_;
	raw_col.advance(col);
	const_pointer zero = &buffer_[raw_col()];
	return const_column_iterator(
		zero + row_index_origin_() * width_,
		zero + row_index_origin_() * width_,
		zero + width_ * height_,
		zero,
		col,
		0,
		this);
}

template <typename T>
typename fast_shift_table<T>::const_column_iterator fast_shift_table<T>::end_column(int col)
const
{
	auto itr = begin_column(col);
	itr.base = itr.zero + width_ * height_;
	itr.position.y = height_;
	return itr;
}

/**********************************************************************************
 *
 * DLIB_CC::FAST_SHIFT_TABLE::TEMPLATE_ITERATOR CLASS
 *
 **********************************************************************************/
template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::template_iterator()
:base(),begin(),end(),zero(),position(),tick()
{}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::template_iterator(
	pointer_type bs, pointer_type bg, pointer_type ed, pointer_type zr, int c, int r, const fast_shift_table<T>* parent)
:base(bs),begin(bg),end(ed),zero(zr),position(c, r),tick()
{
	assert(zero <= begin && begin <= end);
	ex_init<iterator_type>()(parent, this);
}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::template_iterator(const template_iterator& other)
:base(other.base),
begin(other.begin),
end(other.end),
zero(other.zero),
position(other.position),
tick(other.tick)
{}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>&
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::operator=(const template_iterator& other)
{
	if(this != &other)
	{
		base = other.base;
		begin = other.begin;
		end = other.end;
		zero = other.zero;
	}

	return *this;
}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>&
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::operator++()
{
	ex_advance_position<iterator_type>()(position, 1);

	if(zero == begin)
	{
		base += tick;
		return *this;
	}
	else if(base + tick == zero)
	base = begin;
	else if(base + tick == end)
	base = zero;
	else if(base + tick == begin)
	base = end;
	else
	base += tick;

	return *this;
}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>&
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::operator--()
{
	ex_advance_position<iterator_type>()(position, -1);

	if(zero == begin)
	{
		base -= tick;
		return *this;
	}
	else if(base == end)
	base = begin - tick;
	else if(base == zero)
	base = end - tick;
	else if(base == begin)
	base = zero - tick;
	else
	base -= tick;

	return *this;
}

template <typename T>
template <typename iterator_type, typename reference_type, typename pointer_type>
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>&
fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>
::operator+=(difference_type n)
{
	if(n < 0)
	{
		for(int i = 0; i < -1 * n; ++i)
		--*this;
	}
	else
	{
		for(int i = 0; i < n; ++i)
		++*this;
	}

	return *this;
}

/**********************************************************************************
 *
 * SPECIALIZETATION OF ex_advance_position CLASSES (FUNCTION OBJECT)
 *
 **********************************************************************************/
#define SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_ROW_ITERATOR(row_itr_type) \
template <typename T>\
template <typename iterator_type, typename reference_type, typename pointer_type>\
template <typename V>\
struct fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>\
::ex_advance_position<row_itr_type, V>\
{\
	void operator()(dlib_cc::vec2<int>& pos, int delta)\
	{\
		pos.x += delta;\
	}\
};\

#define SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_COLUMN_ITERATOR(column_itr_type) \
template <typename T>\
template <typename iterator_type, typename reference_type, typename pointer_type>\
template <typename V>\
struct fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>\
::ex_advance_position<column_itr_type, V>\
{\
	void operator()(dlib_cc::vec2<int>& pos, int delta)\
	{\
		pos.y += delta;\
	}\
}\


SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_ROW_ITERATOR(
	typename fast_shift_table<T>::row_iterator_type);
SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_ROW_ITERATOR(
	typename fast_shift_table<T>::const_row_iterator_type);
SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_COLUMN_ITERATOR(
	typename fast_shift_table<T>::column_iterator_type);
SPECIALIZE_ADVANCE_POS_FUNC_OBJ_FOR_COLUMN_ITERATOR(
	typename fast_shift_table<T>::const_column_iterator_type);

/**********************************************************************************
 *
 * SPECIALIZETATION OF ex_init CLASSES (FUNCTION OBJECT)
 *
 **********************************************************************************/
#define SPECIALIZE_INIT_FUNC_OBJ_FOR_ROW_ITERATOR(row_itr_type) \
template <typename T>\
template <typename iterator_type, typename reference_type, typename pointer_type>\
template <typename V>\
struct fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>\
::ex_init<row_itr_type, V>\
{\
	void operator()(\
		const fast_shift_table<T>* parent,\
		fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>* iterator)\
	{\
		/* the initialization for row iterators */\
		iterator->tick = 1;\
	}\
}\

#define SPECIALIZE_INIT_FUNC_OBJ_FOR_COLUMN_ITERATOR(column_itr_type) \
template <typename T>\
template <typename iterator_type, typename reference_type, typename pointer_type>\
template <typename V>\
struct fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>\
::ex_init<column_itr_type, V>\
{\
	void operator()(\
		const fast_shift_table<T>* parent,\
		fast_shift_table<T>::template_iterator<iterator_type, reference_type, pointer_type>* iterator)\
	{\
		/* the initialization for column iterators */\
		iterator->tick = parent->width();\
	}\
}\

SPECIALIZE_INIT_FUNC_OBJ_FOR_ROW_ITERATOR(typename fast_shift_table<T>::row_iterator_type);
SPECIALIZE_INIT_FUNC_OBJ_FOR_ROW_ITERATOR(typename fast_shift_table<T>::const_row_iterator_type);
SPECIALIZE_INIT_FUNC_OBJ_FOR_COLUMN_ITERATOR(typename fast_shift_table<T>::column_iterator_type);
SPECIALIZE_INIT_FUNC_OBJ_FOR_COLUMN_ITERATOR(typename fast_shift_table<T>::const_column_iterator_type);

} /* namespace dlib_cc */

#endif
