#pragma once
#include <memory>
#include <iostream>
#include <exception>
#include <fstream>

template <class List_Type>
class blist
{
private:

	struct Node;
	// ��������� ��� ��������
	typedef std::shared_ptr < Node > node_ptr;

	/* --------------------------- CLASS NODE --------------------------------- */
	// ���� ������
	struct Node
	{
		// �������� ����� ����
		List_Type _value;
		// ����� ��������� �� ���������� �������
		node_ptr _previous;
		// ����� ��������� �� ��������� �������
		node_ptr _next;

		// �����������, ��������������� �������� ����� ��� value
		// � ��� ��������� - nullptr
		Node(List_Type value)
			: Node{ value, nullptr, nullptr } {}

		// ����������� � �����������
		Node(List_Type value, node_ptr previous, node_ptr next)
			: _value{ value }
			, _previous{ previous }
			, _next{ next } {}
	};

	/* --------------------------- CLASS ITERATOR --------------------------------- */
	template <typename It_Type>
	class blist_iterator
	{
	private:
		friend class blist < List_Type >;
	public:
		/* --------------------------- ITERATOR TRAITs --------------------------------- */
		typedef It_Type iterator_type;
		typedef std::random_access_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;
		// ������, �� ������� ��������� ��������
		iterator_type* ptr_value;
	private:
		// ����������� � ����������
		blist_iterator(iterator_type* ptr)

			: ptr_value{ ptr } {}

	public:
		// ����������� �����������
		blist_iterator(const blist_iterator& it)
			: ptr_value{ it.ptr_value } {};

		/* --------------------------- ITERATOR OPERATORS OVERLOADing ------------------------------------ */

		/* <summary> ���������� ��������� ����������� ��� ����������. </summary>
		 * <param name=it> �������� ��� ���������. </param>
		 * <returns> ���� </returns> */
		bool operator!=(blist_iterator const& it) { return this->ptr_value != it.ptr_value; }
		/* <summary> ���������� ��������� ��������� ��� ����������. </summary>
		 * <param name=it> �������� ��� ���������. </param>
		 * <returns> ���� </returns> */
		bool operator==(blist_iterator const& it) { return this->ptr_value == it.ptr_value; }
		bool operator > (blist_iterator const& it) { return this->ptr_value > it.ptr_value; }
		bool operator >= (blist_iterator const& it) { return this->ptr_value >= it.ptr_value; }
		bool operator <= (blist_iterator const& it) { return this->ptr_value <= it.ptr_value; }
		bool operator < (blist_iterator const& it) { return this->ptr_value < it.ptr_value; }

		/* <summary> ���������� ��������� ������������. </summary>
		 * <returns> �������� �������� ������ �� ������� ��������� ��������. </returns> */
		List_Type operator*() const { return ptr_value->_value; }
		/* <summary> ���������� ��������� ����������. </summary>
		 * <returns> ������ �� ��������, ����������� �� ��������� �������. </returns> */
		blist_iterator& operator += (size_t rha) {
			for (size_t i = 0; i < rha; i++)
			{
				ptr_value = ptr_value->_next ? ptr_value->_next.get() : nullptr;
			}
			return *this;
		}
		blist_iterator& operator -= (size_t rha) {
			for (size_t i = 0; i < rha; i++)
			{
				ptr_value = ptr_value->_previous ? ptr_value->_previous.get() : nullptr;
			}
			return *this;
		}
		blist_iterator& operator = (const blist_iterator& rha) { ptr_value = rha.ptr_value; return *this; }
		blist_iterator& operator++()
		{
			ptr_value = ptr_value->_next ? ptr_value->_next.get() : nullptr;
			return *this;
		}
		blist_iterator operator +(const int& n) {
			iterator_type * temp = ptr_value;
			for (size_t i = 0; i < n; i++)
			{
				temp = temp->_next ? ptr_value->_next.get() : nullptr;
			}
			return temp;
		}
		
		blist_iterator operator -(size_t n) {
			iterator_type* temp = ptr_value;
			for (size_t i = 0; i < n; i++)
			{
				temp = temp->_previous ? ptr_value->_previous.get() : nullptr;
			}
			return temp;
		}

		blist_iterator& operator--() {
			ptr_value = ptr_value->_previous ? ptr_value->_previous.get() : nullptr;
			return *this;
		}
		List_Type& operator[](size_t index) {
			for (size_t i = 0; i < index; i++)
			{
				ptr_value = ptr_value->_next ? ptr_value->_next.get() : nullptr;
			}
			return ptr_value->_value;
		}
		List_Type* operator->()const {
			return ptr_value;
		}
	};

public:

	/* --------------------------- CONSTRUCTORS --------------------------------------------------------- */

	/* <summary> ����������� �� ���������. </summary> */
	blist()
		: _head{ nullptr }
		, _size{ 0 } {}

	/* <summary> ����������� �������� �������������. </summary>
	 * <param name=value> ������ �������������  </param> */
	blist(const std::initializer_list < List_Type >& init_list)
	{
		for (auto& it : init_list)
			push_back(it);
	}

	/* --------------------------- GET - SET LIST ELEMENTs METHODS ------------------------------------------ */

	/* <returns> ������ ������� ������. </returns> */
	List_Type front() { if (_head) return _head->_value; else throw std::exception("blist is Empty"); }
	/* <returns> ��������� ������� ������. </returns> */
	List_Type back() { if (_tail) return _tail->_value; else  throw std::exception("blist is Empty"); }

	/* <summary> ���������� �������� � ����� ������. </summary>
	 * <param name=value> �������� ��-��. </param> */
	void push_back(List_Type value)
	{
		if (!_head) { _head = node_ptr(new Node{ value }); _tail = _head; }
		else
		{
			_tail->_next = node_ptr(new Node{ value, _tail, nullptr });
			_tail = _tail->_next;
		}

		_size++;
	}

	/* <summary> ���������� �������� � ����� ������ ������. </summary>
	 * <param name=value> �������� ��-��. </param> */
	void push_front(List_Type value)
	{
		if (!_head) { _head = node_ptr(new Node{ value }); _tail = _head; }
		else
			this->_head = node_ptr(new Node{ value, nullptr, _head });
		_size++;
	}

	/* <summary> �������� ���������� �������� ������. </summary>
	 * <returns> �������� ���������� �������� ������. </returns> */
	List_Type pop_back()
	{
		if (!_head && !_tail) throw std::exception("blist is Empty");

		auto result = _tail->_value;
		_tail = _tail->_previous;
		_tail->_next = nullptr;

		_size--;

		return result;
	}

	/* <summary> �������� ������� �������� ������. </summary>
	 * <returns> �������� ������� �������� ������. </returns> */
	List_Type pop_front()
	{
		if (!_head && !_tail) throw std::exception("blist is Empty");

		auto result = _head->_value;
		_head = _head->_next;

		_size--;
		return result;
	}

	/* <summary> ������� �������� � ������������ ������� ������. </summary>
	 * <param name=value> �������� �������� </param>
	 * <param name=value> ������ </param> */
	void insert(List_Type value, size_t index)
	{
		if (index == 0) { push_front(value); return; }
		if (index == _size) { push_back(value); return; }

		auto previous = this->get_node(index - 1);
		previous->_next = node_ptr(new Node{ value, previous, previous->_next });

		_size++;
	}

	/* <returns> ������ ������ </returns> */
	size_t size() { return this->_size; }
	/* <returns> ����: ������ �� ������ </returns> */
	bool empty() { return !this->_size; }
	/* <summary> ������� ������. </summary> */
	void clear()
	{
		for (size_t i = 0; i < this->_size; i++)
			pop_back();
	}

	typedef blist_iterator <Node> iterator;
	typedef blist_iterator <const Node> const_iterator;

	/* --------------------------- ITERATORs STL FUNCTIONS --------------------------------------------------------- */

	/* <returns> ��������, ����������� �� ������ ������ </returns> */
	iterator begin() { if (_head) return iterator(_head.get()); else throw std::exception("blist is Empty"); }
	/* <returns> ��������, ����������� �� ����� ������ </returns> */
	iterator end() { return nullptr; }

	/* <returns> ����������� ��������, ����������� �� ������ ������ </returns> */
	const_iterator cbegin() const { if (_head) return iterator(_head.get()); else throw std::exception("blist is Empty"); }
	/* <returns> ����������� ��������, ����������� �� ����� ������ </returns> */
	const_iterator cend() const { return nullptr; }

	/* --------------------------- BLIST OPERATORs OVERLOADing --------------------------------------------------------- */

	/* <summary> ���������� ��������� ������ ��� ���� ������ </summary>
	 * <param name=out> ����� ������ </param>
	 * <param name=node> ���� </param>
	 * <returns> ����� ������ </returns> */
	friend std::ostream& operator<<(std::ostream out, const Node& node) { return out << node._value; }

	/* <summary> ���������� ��������� [] </summary>
	 * <param name=n> ������ </param>
	 * <returns> ���������� ������� ������ ��� �������� n </returns> */
	List_Type operator[](size_t n)
	{
		// �������� ������� ��������� � ������
		if (empty()) throw std::exception("blist is Empty");
		if (n >= this->_size) throw std::exception("blist error: index out of range!");
		return get_node(n)->_value;
	}

	/* <summary> ��������� ���� �� ������� </summary>
	 * <param name=n> ������ </param>
	 * <returns> ���������� ���� ��� �������� n </returns> */
	node_ptr& get_node(size_t n)
	{
		// ��������� �� ������� �������
		Node* current = nullptr;

		if (n == 0) return this->_head;
		if (n == this->_size - 1) return this->_tail;

		// �����, � ����� ������� ������� ��������� �� ��������
		if (n < _size / 2)
		{
			// ����� ����� �������
			current = this->_head.get();
			for (size_t i = 0; i < n - 1; i++)
				current = current->_next.get();
			return current->_next;
		}
		else
		{
			// ����� ������ ������
			current = this->_tail.get();
			for (size_t i = _size - 1; i > n + 1; i--)
				current = current->_previous.get();
			return current->_previous;
		}
	}

private:
	// ������ - ������ ������
	node_ptr _head;
	// ����� - ����� ������
	node_ptr _tail;
	// ������ ������
	size_t _size;
};

