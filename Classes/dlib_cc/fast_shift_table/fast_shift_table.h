#ifndef DLIB_CC_FAST_SHIFT_TABLE_H
#define DLIB_CC_FAST_SHIFT_TABLE_H

#include "../mobius_strip/mobius_strip.h"
#include "../vec2/vec2.h"
#include <vector>

namespace dlib_cc
{
template <typename T> class fast_shift_table
{
public:
	fast_shift_table();
	fast_shift_table(int width, int height);
	fast_shift_table(int width, int height, int def);
	void resize(int width, int height);
	void resize(int width, int height, int def);
	int size() const;
	const T& read(int col, int row) const;
	void write(int col, int row, T val);
	void shift_columns(int distance);
	void shift_rows(int distance);
	int to_index(int col, int row) const;

	// operators
	T& operator()(int col, int row) { return buffer_[to_index(col, row)]; }
	const T& operator()(int col, int row) const { return buffer_[to_index(col, row)]; }

	// accessors
	int width() const { return width_; }
	int height() const { return height_; }

private:
	int width_;
	int height_;
	dlib_cc::mobius_strip<int> row_index_origin_;
	dlib_cc::mobius_strip<int> column_index_origin_;
	std::vector<T> buffer_;

	/**********************************************************************************
	 *
	 * ITERATOR
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
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

	/**
	 * iterator names
	 */
	template <typename iterator_type, typename reference_type, typename pointer_type>
	struct template_iterator;
	using row_iterator = template_iterator<row_iterator_type, reference, pointer>;
	using column_iterator = template_iterator<column_iterator_type, reference, pointer>;
	using const_row_iterator = template_iterator<const_row_iterator_type, const_reference, const_pointer>;
	using const_column_iterator = template_iterator<const_column_iterator_type, const_reference, const_pointer>;
	using unordered_iterator = typename std::vector<T>::iterator;
	using const_unordered_iterator = typename std::vector<T>::const_iterator;

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
	template <typename iterator_type, typename reference_type, typename pointer_type>
	struct template_iterator : public std::iterator<
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
		dlib_cc::vec2<int> position;
		difference_type tick;

		template_iterator();
		template_iterator(pointer_type bs, pointer_type bg, pointer_type ed, pointer_type zr, int c, int r, fast_shift_table<T>* parent);
		template_iterator(const template_iterator& other);
		template_iterator& operator=(const template_iterator& other);
		template_iterator& operator++();
		template_iterator& operator--();
		template_iterator& operator+=(difference_type n);

		// inline methods
		difference_type operator-(const template_iterator& other) const { return static_cast<difference_type>(base - other.base); }
		template_iterator operator++(int) { auto old = *this; ++*this; return old; }
		template_iterator operator--(int) { auto old = *this; --*this; return old; }
		template_iterator operator+(difference_type n) const { auto cp = *this; return (cp += n); }
		template_iterator& operator-=(difference_type n) { return (*this += -n); }
		template_iterator operator-(difference_type n) const { auto cp = *this; return (cp -= n); }
		reference_type operator*() const { return *base; }
		pointer_type operator->() const { return &**this; }
		bool operator==(const template_iterator& other) const { return (base == other.base); }
		bool operator!=(const template_iterator& other) const { return !(*this == other); }
		bool operator<(const template_iterator& other) const { return (base < other.base); }
		bool operator>(const template_iterator& other) const { return  (other < *this); }
		bool operator<=(const template_iterator& other) const { return !(*this > other); }
		bool operator>=(const template_iterator& other) const { return  !(*this < other); }

	private:
		// function object
		template <typename itr_type, typename = void> struct ex_advance_position
		{
			void operator()(dlib_cc::vec2<int>& pos, int delta);
		};

		template <typename itr_type, typename = void> struct ex_init
		{
			void operator()(fast_shift_table<T>* parent, template_iterator<iterator_type, reference_type, pointer_type>* iterator);
		};

	}; /* struct template_iterator */
}; /* class fast_shift_table */

} /* namespace dlib_cc */

/**
 * include the implementation file
 */
#include "fast_shift_table_impl.h"

#endif
