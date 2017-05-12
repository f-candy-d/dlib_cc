#ifndef DLIB_CC_LTS_MAP_BUFFER_CP_H
#define DLIB_CC_LTS_MAP_BUFFER_CP_H

#include "../mobius_strip/mobius_strip.h"
#include <vector>

namespace dlib_cc
{
namespace lts_map_unit
{

class Buffer
{
public:
	// the value type of elements in this container
	using value_typename = int;

	Buffer();
	Buffer(int w, int h);
	Buffer(int w, int h, int f);
	void Resize(int w, int h);
	void Resize(int w, int h, int f);
	int Size() const;
	const value_typename& Read(int col, int row) const;
	void Write(int col, int row, value_typename val);
	void ShiftColumns(int distance);
	void ShiftRows(int distance);
	void DisableQueuingAfterWrite(bool bol);
	void Queue(int col, int row);

	// operators
	value_typename& operator()(int col, int row) { return terrain_buff_[ToIndex(col, row)]; }
	const value_typename& operator()(int col, int row) const { return terrain_buff_[ToIndex(col, row)]; }

	// accessors & mutators
	int width() const { return width_; }
	int height() const { return height_; }
	std::vector<int>& rendering_queue () { return rendering_que_; }
	const std::vector<int>& rendering_queue () const { return rendering_que_; }

private:
	int width_;
	int height_;
	bool que_after_write_;
	dlib_cc::mobius_strip<int> row_index_origin_;
	dlib_cc::mobius_strip<int> column_index_origin_;
	std::vector<value_typename> terrain_buff_;
	std::vector<int> rendering_que_;

	void Queue(int index);
	int ToIndex(int col, int row) const;

	/**********************************************************************************
	 *
	 * ITERATOR OF BUFFER CLASS
	 *
	 **********************************************************************************/
private:
	/**
	 * the following structures are invisible from the other classes,
	 * and that have no body (only used for specialization of template_iterator class)
	 */
	struct row_iterator_type;
	struct column_iterator_type;
	struct const_row_iterator_type;
	struct const_column_iterator_type;

public:
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;
	using value_type = value_typename;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

	/**
	 * iterator names
	 */
	template <typename iterator_type, typename reference_type, typename pointer_type> struct template_iterator;
	using row_iterator = template_iterator<row_iterator_type, reference, pointer>;
	using column_iterator = template_iterator<column_iterator_type, reference, pointer>;
	using const_row_iterator = template_iterator<const_row_iterator_type, const_reference, const_pointer>;
	using const_column_iterator = template_iterator<const_column_iterator_type, const_reference, const_pointer>;
	using unordered_iterator = std::vector<value_typename>::iterator;
	using const_unordered_iterator = std::vector<value_typename>::const_iterator;

	/**
	* GETTER METHODS FOR ITERATORS
	*/
	unordered_iterator begin();
	unordered_iterator end();
	const_unordered_iterator begin() const;
	const_unordered_iterator end() const;
	row_iterator begin_row(int row);
	row_iterator end_row(int row);
	const_row_iterator begin_row(int row) const;
	const_row_iterator end_row(int row) const;
	column_iterator begin_column(int col);
	column_iterator end_column(int col);
	const_column_iterator begin_column(int col) const;
	const_column_iterator end_column(int col) const;

	/**
	 * TEMPLATE CLASS OF ITERATORS
	 */
	template <typename iterator_type, typename reference_type, typename pointer_type> struct template_iterator : public std::iterator<
		std::random_access_iterator_tag,
		value_type,
		difference_type,
		pointer_type,
		reference_type>
	{
	public:
		// a pointer to an elements in a container
		pointer_type base;
		pointer_type begin;
		pointer_type end;
		pointer_type zero;
		int col;
		int row;
		difference_type tick;

		template_iterator()
		:base(),begin(),end(),zero(),col(),row(),tick()
		{}

		template_iterator(
			pointer_type bs, pointer_type bg, pointer_type ed, pointer_type zr, int c, int r, difference_type tc = 1)
		:base(bs),begin(bg),end(ed),zero(zr),col(c),row(r),tick(tc)
		{
			assert(zero <= begin && begin <= end);
		}

		template_iterator(const template_iterator& other)
		:base(other.base),begin(other.begin),end(other.end),zero(other.zero),col(other.col),row(other.row),tick(other.tick)
		{}

		difference_type operator-(const template_iterator& other)
		const
		{
			return static_cast<difference_type>(base - other.base);
		}

		template_iterator& operator=(const template_iterator& other)
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

		template_iterator& operator++()
		{
			go_forward_coordinate();

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

		template_iterator operator++(int)
		{
			auto old = *this;
			++*this;
			return old;
		}

		template_iterator& operator--()
		{
			go_backward_coordinate();

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

		template_iterator operator--(int)
		{
			auto old = *this;
			--*this;
			return old;
		}

		template_iterator& operator+=(difference_type n)
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

		template_iterator operator+(difference_type n)
		const
		{
			auto cp = *this;
			return (cp += n);
		}

		template_iterator& operator-=(difference_type n)
		{
			return (*this += -n);
		}

		template_iterator operator-(difference_type n)
		const
		{
			auto cp = *this;
			return (cp -= n);
		}

		reference_type operator*() const { return *base; }
		pointer_type operator->() const { return &**this; }
		bool operator==(const template_iterator& other) const { return (base == other.base); }
		bool operator!=(const template_iterator& other) const { return !(*this == other); }
		bool operator<(const template_iterator& other) const { return (base < other.base); }
		bool operator>(const template_iterator& other) const { return  (other < *this); }
		bool operator<=(const template_iterator& other) const { return !(*this > other); }
		bool operator>=(const template_iterator& other) const { return  !(*this < other); }

	private:
		/**
		 * do specialization these methods in .cpp file for (const-)row-iterator and (const-)column-iterator
		 */
		void go_forward_coordinate();
		void go_backward_coordinate();
	};
};

/**
 * macros for specialization of go_forward_coordinate() and go_backward_coordinate() methods
 */
#define SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(iterator_type)\
template <> void Buffer::iterator_type::go_forward_coordinate() { ++col; }\

#define SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_ROW_ITERATOR(iterator_type)\
template <> void Buffer::iterator_type::go_backward_coordinate() { --col; }\

#define SPECIALIZE_GO_FORWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(iterator_type)\
template <> void Buffer::iterator_type::go_forward_coordinate() { ++row; }\

#define SPECIALIZE_GO_BACKWARD_COORDINATE_FUNC_FOR_COLUMN_ITERATOR(iterator_type)\
template <> void Buffer::iterator_type::go_backward_coordinate() { --row; }\

} /* namespace lts_map_unit */
} /* namespace dlib_cc */

#endif
