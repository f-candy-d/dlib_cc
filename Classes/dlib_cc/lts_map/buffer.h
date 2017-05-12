#ifndef DLIB_CC_LTS_MAP_BUFFER_CP_H
#define DLIB_CC_LTS_MAP_BUFFER_CP_H

#include "../mobius_strip/mobius_strip.h"
#include "../fast_shift_table/fast_shift_table.h"
#include <vector>

namespace dlib_cc
{
namespace lts_map_unit
{

class Buffer
{
public:
	// the value type of elements in this container
	using value_type = int;

	// iterators
	using unordered_iterator = dlib_cc::fast_shift_table<value_type>::unordered_iterator;
	using const_unordered_iterator = dlib_cc::fast_shift_table<value_type>::const_unordered_iterator;
	using row_iterator = dlib_cc::fast_shift_table<value_type>::row_iterator;
	using const_row_iterator = dlib_cc::fast_shift_table<value_type>::const_row_iterator;
	using column_iterator = dlib_cc::fast_shift_table<value_type>::column_iterator;
	using const_column_iterator = dlib_cc::fast_shift_table<value_type>::const_column_iterator;

	Buffer();
	Buffer(int w, int h);
	Buffer(int w, int h, int f);
	void Resize(int w, int h) { terrain_buff_.resize(w, h); }
	void Resize(int w, int h, int f) { terrain_buff_.resize(w, h, f); }
	int Size() const { return terrain_buff_.size(); }
	const value_type& Read(int col, int row) const { return terrain_buff_(col, row); }
	void Write(int col, int row, value_type val);
	void ShiftColumns(int distance) { terrain_buff_.shift_columns(distance); }
	void ShiftRows(int distance) { terrain_buff_.shift_rows(distance); }
	void DisableQueuingAfterWrite(bool bol) { que_after_write_ = !bol; }
	void Queue(int col, int row) { Queue(terrain_buff_.to_index(col, row)); }

	unordered_iterator begin() { return terrain_buff_.begin(); }
	unordered_iterator end() { return terrain_buff_.end(); }
	const_unordered_iterator begin() const { return terrain_buff_.begin(); }
	const_unordered_iterator end() const { return terrain_buff_.end(); }
	row_iterator begin_row(int row) { return terrain_buff_.begin_row(row); }
	row_iterator end_row(int row) { return terrain_buff_.end_row(row); }
	const_row_iterator begin_row(int row) const { return std::move(terrain_buff_.begin_row(row)); }
	const_row_iterator end_row(int row) const { return terrain_buff_.end_row(row); }
	column_iterator begin_column(int col) { return terrain_buff_.begin_column(col); }
	column_iterator end_column(int col) { return terrain_buff_.end_column(col); }
	const_column_iterator begin_column(int col) const { return terrain_buff_.begin_column(col); }
	const_column_iterator end_column(int col) const { return terrain_buff_.end_column(col); }

	// operators
	value_type& operator()(int col, int row) { return terrain_buff_(col, row); }
	const value_type& operator()(int col, int row) const { return terrain_buff_(col, row); }

	// accessors & mutators
	int width() const { return terrain_buff_.width(); }
	int height() const { return terrain_buff_.height(); }
	std::vector<int>& rendering_queue () { return rendering_que_; }
	const std::vector<int>& rendering_queue () const { return rendering_que_; }

private:
	bool que_after_write_;
	dlib_cc::fast_shift_table<value_type> terrain_buff_;
	std::vector<int> rendering_que_;

	void Queue(int index);
};

} /* namespace lts_map_unit */
} /* namespace dlib_cc */

#endif
