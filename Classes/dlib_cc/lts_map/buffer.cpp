#include "buffer.h"
#include <cassert>
#include <iostream>

namespace dlib_cc
{
namespace lts_map_unit
{

/**********************************************************************************
 *
 * DLIB_CC::LTS_MAP::BUFFER CLASS
 *
 **********************************************************************************/

Buffer::Buffer()
:width_(0)
,height_(0)
,que_after_write_(true)
,row_index_origin_(0, 0)
,column_index_origin_(0, 0)
{}

Buffer::Buffer(int w, int h)
:width_(w)
,height_(h)
,que_after_write_(true)
,row_index_origin_(0, w - 1)
,column_index_origin_(0, h - 1)
,terrain_buff_(w * h)
{
	assert(0 <= w && 0 <= h);
}

Buffer::Buffer(int w, int h, int f)
:width_(w)
,height_(h)
,que_after_write_(true)
,row_index_origin_(0, w - 1)
,column_index_origin_(0, h - 1)
,terrain_buff_(w * h, f)
{
	assert(0 <= w && 0 <= h);
}

void Buffer::Resize(int w, int h)
{
	assert(0 <= w && 0 <= h);
	terrain_buff_.resize(w * h);
	width_ = w;
	height_ = h;
	row_index_origin_.begin = 0;
	column_index_origin_.begin = 0;
	row_index_origin_.end = w - 1;
	column_index_origin_.end = h - 1;
}

void Buffer::Resize(int w, int h, int f)
{
	assert(0 <= w && 0 <= h);
	terrain_buff_.resize(w * h, f);
	width_ = w;
	height_ = h;
	row_index_origin_.begin = 0;
	column_index_origin_.begin = 0;
	row_index_origin_.end = w - 1;
	column_index_origin_.end = h - 1;
}

int Buffer::Size()
const
{
	return width_ * height_;
}

const int& Buffer::Read(int col, int row)
const
{
	return terrain_buff_[ToIndex(col, row)];
}

void Buffer::Write(int col, int row, int val)
{
	auto index = ToIndex(col, row);
	terrain_buff_[index] = val;

	if(que_after_write_)
	{
		Queue(index);
	}
}

void Buffer::ShiftColumns(int distance)
{
	column_index_origin_.advance(-1 * distance);
}

void Buffer::ShiftRows(int distance)
{
	row_index_origin_.advance(-1 * distance);
}

void Buffer::DisableQueuingAfterWrite(bool bol)
{
	que_after_write_ = !bol;
}

void Buffer::Queue(int col, int row)
{
	Queue(ToIndex(col, row));
}

void Buffer::Queue(int index)
{
	assert(0 <= index && index < width_ * height_);
	rendering_que_.push_back(index);
}

int Buffer::ToIndex(int col, int row)
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
 * ITERATOR OF DLIB_CC::LTS_MAP::BUFFER CLASS
 *
 **********************************************************************************/

using unordered_iterator = Buffer::unordered_iterator;
using const_unordered_iterator = Buffer::const_unordered_iterator;
using row_iterator = Buffer::row_iterator;
using const_row_iterator = Buffer::const_row_iterator;
using column_iterator = Buffer::column_iterator;
using const_column_iterator = Buffer::const_column_iterator;
using difference_type = Buffer::difference_type;
using size_type = Buffer::size_type;
using value_type = Buffer::value_type;
using pointer = Buffer::pointer;
using const_pointer = Buffer::const_pointer;
using reference = Buffer::reference;
using const_reference = Buffer::const_reference;

unordered_iterator Buffer::begin()
{
	return terrain_buff_.begin();
}

unordered_iterator Buffer::end()
{
	return terrain_buff_.end();
}

const_unordered_iterator Buffer::begin() const
{
	return terrain_buff_.cbegin();
}

const_unordered_iterator Buffer::end() const
{
	return terrain_buff_.cend();
}

row_iterator Buffer::begin_row(int row)
{
	assert(0 <= row && row < height_);

	auto raw_row = row_index_origin_;
	raw_row.advance(row);
	pointer zero = &terrain_buff_[raw_row() * width_];
	return row_iterator(
		zero + column_index_origin_(),
		zero + column_index_origin_(),
		zero + width_,
		zero,
		0,
		row);
}

row_iterator Buffer::end_row(int row)
{
	auto itr = begin_row(row);
	itr.base = itr.zero + width_;
	itr.col = width_;
	return itr;
}

const_row_iterator Buffer::begin_row(int row) const
{
	assert(0 <= row && row < height_);

	auto raw_row = row_index_origin_;
	raw_row.advance(row);
	const_pointer zero = &terrain_buff_[raw_row() * width_];
	return const_row_iterator(
		zero + column_index_origin_(),
		zero + column_index_origin_(),
		zero + width_,
		zero,
		0,
		row);
}

const_row_iterator Buffer::end_row(int row) const
{
	auto itr = begin_row(row);
	itr.base = itr.zero + width_;
	itr.col = width_;
	return itr;
}

column_iterator Buffer::begin_column(int col)
{
	assert(0 <= col && col < width_);

	auto raw_col = column_index_origin_;
	raw_col.advance(col);
	pointer zero = &terrain_buff_[raw_col()];
	return column_iterator(
		zero + row_index_origin_() * width_,
		zero + row_index_origin_() * width_,
		zero + width_ * height_,
		zero,
		col,
		0,
		width_);
}

column_iterator Buffer::end_column(int col)
{
	auto itr = begin_column(col);
	itr.base = itr.zero + width_ * height_;
	itr.row = height_;
	return itr;
}

const_column_iterator Buffer::begin_column(int col) const
{
	assert(0 <= col && col < width_);

	auto raw_col = column_index_origin_;
	raw_col.advance(col);
	const_pointer zero = &terrain_buff_[raw_col()];
	return const_column_iterator(
		zero + row_index_origin_() * width_,
		zero + row_index_origin_() * width_,
		zero + width_ * height_,
		zero,
		col,
		0,
		width_);
}

const_column_iterator Buffer::end_column(int col) const
{
	auto itr = begin_column(col);
	itr.base = itr.zero + width_ * height_;
	itr.row = height_;
	return itr;
}

/**
 * SPECIALIZATION OF go_forward_coordinate() AND go_backward_coordinate() METHODS IN template_iterator CLASS
 */
SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(Buffer::row_iterator);
SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(Buffer::const_row_iterator);
SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(Buffer::column_iterator);
SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(Buffer::const_column_iterator);
SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(Buffer::row_iterator);
SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(Buffer::const_row_iterator);
SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(Buffer::column_iterator);
SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(Buffer::const_column_iterator);

} /* namespace lts_map_unit */
} /* namespace dlib_cc */
