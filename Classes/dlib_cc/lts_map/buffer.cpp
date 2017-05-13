/**********************************************************************************
 *
 * dlib_cc::lts_map_unit::Buffer class
 *
 **********************************************************************************/
#include "buffer.h"
#include <cassert>
#include <iostream>

namespace dlib_cc
{
namespace lts_map_unit
{

Buffer::Buffer()
:que_after_write_(true)
,loading_range_()
{}

Buffer::Buffer(int width, int height)
:que_after_write_(true)
,terrain_buff_(width, height)
,loading_range_(0, 0, width, height)
{
	assert(0 <= height && 0 <= height);
}

Buffer::Buffer(int width, int height, int def)
:que_after_write_(true)
,terrain_buff_(width, height, def)
,loading_range_(0, 0, width, height)
{
	assert(0 <= width && 0 <= height);
}

Buffer::Buffer(int orig_col, int orig_row, int width, int height)
:que_after_write_(true)
,terrain_buff_(width, height)
,loading_range_(orig_col, orig_row, width, height)
{
	assert(0 <= width && 0 <= height);
}

Buffer::Buffer(int orig_col, int orig_row, int width, int height, int def)
:que_after_write_(true)
,terrain_buff_(width, height, def)
,loading_range_(orig_col, orig_row, width, height)
{
	assert(0 <= width && 0 <= height);
}

void Buffer::Write(int col, int row, int val)
{
	auto index = terrain_buff_.to_index(col, row);
	terrain_buff_[index] = val;

	if(que_after_write_)
	{
		Queue(index);
	}
}

void Buffer::ShiftColumns(int distance)
{
	terrain_buff_.shift_columns(distance);
	loading_range_.move_hor(distance);
}

void Buffer::ShiftRows(int distance)
{
	terrain_buff_.shift_rows(distance);
	loading_range_.move_ver(distance);
}

void Buffer::Queue(int index)
{
	assert(0 <= index && index < terrain_buff_.size());
	rendering_que_.push_back(index);
}

} /* namespace lts_map_unit */
} /* namespace dlib_cc */
