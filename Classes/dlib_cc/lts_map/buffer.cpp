#include "buffer.h"
#include <cassert>
#include <iostream>

/**********************************************************************************
 *
 * DLIB_CC::LTS_MAP::BUFFER CLASS
 *
 **********************************************************************************/

namespace dlib_cc
{
namespace lts_map_unit
{

Buffer::Buffer()
:que_after_write_(true)
{}

Buffer::Buffer(int w, int h)
:que_after_write_(true)
,terrain_buff_(w, h)
{
	assert(0 <= w && 0 <= h);
}

Buffer::Buffer(int w, int h, int f)
:que_after_write_(true)
,terrain_buff_(w, h, f)
{
	assert(0 <= w && 0 <= h);
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

void Buffer::Queue(int index)
{
	assert(0 <= index && index < terrain_buff_.size());
	rendering_que_.push_back(index);
}

} /* namespace lts_map_unit */
} /* namespace dlib_cc */
