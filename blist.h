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
	// псевдоним для удобства
	typedef std::shared_ptr < Node > node_ptr;

	/* --------------------------- CLASS NODE --------------------------------- */
	// узел списка
	struct Node
	{
		// значащая часть узла
		List_Type _value;
		// умный указатель на предыдущий элемент
		node_ptr _previous;
		// умный указатель на следующий элемент
		node_ptr _next;

		// конструктор, устанавливающий значащую часть как value
		// а оба указателя - nullptr
		Node(List_Type value)
			: Node{ value, nullptr, nullptr } {}

		// конструктор с параметрами
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
		// данные, на которые указывает итератор
		iterator_type* ptr_value;
	private:
		// конструктор с параметром
		blist_iterator(iterator_type* ptr)

			: ptr_value{ ptr } {}

	public:
		// конструктор копирования
		blist_iterator(const blist_iterator& it)
			: ptr_value{ it.ptr_value } {};

		/* --------------------------- ITERATOR OPERATORS OVERLOADing ------------------------------------ */

		/* <summary> Перегрузка оператора неравенства для итераторов. </summary>
		 * <param name=it> Итератор для сравнения. </param>
		 * <returns> Флаг </returns> */
		bool operator!=(blist_iterator const& it) { return this->ptr_value != it.ptr_value; }
		/* <summary> Перегрузка оператора равенства для итераторов. </summary>
		 * <param name=it> Итератор для сравнения. </param>
		 * <returns> Флаг </returns> */
		bool operator==(blist_iterator const& it) { return this->ptr_value == it.ptr_value; }
		bool operator > (blist_iterator const& it) { return this->ptr_value > it.ptr_value; }
		bool operator >= (blist_iterator const& it) { return this->ptr_value >= it.ptr_value; }
		bool operator <= (blist_iterator const& it) { return this->ptr_value <= it.ptr_value; }
		bool operator < (blist_iterator const& it) { return this->ptr_value < it.ptr_value; }

		/* <summary> Перегрузка оператора разыменвания. </summary>
		 * <returns> Значение элемента списка на который указывает итератор. </returns> */
		List_Type operator*() const { return ptr_value->_value; }
		/* <summary> Перегрузка оператора инкремента. </summary>
		 * <returns> Ссылка на итератор, указывающий на следующий элемент. </returns> */
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

	/* <summary> Конструктор по умолчанию. </summary> */
	blist()
		: _head{ nullptr }
		, _size{ 0 } {}

	/* <summary> Конструктор фигурной инициализации. </summary>
	 * <param name=value> Список инициализации  </param> */
	blist(const std::initializer_list < List_Type >& init_list)
	{
		for (auto& it : init_list)
			push_back(it);
	}

	/* --------------------------- GET - SET LIST ELEMENTs METHODS ------------------------------------------ */

	/* <returns> Первый элемент списка. </returns> */
	List_Type front() { if (_head) return _head->_value; else throw std::exception("blist is Empty"); }
	/* <returns> Последний элемент списка. </returns> */
	List_Type back() { if (_tail) return _tail->_value; else  throw std::exception("blist is Empty"); }

	/* <summary> Добавление элемента в конец списка. </summary>
	 * <param name=value> Значение эл-та. </param> */
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

	/* <summary> Добавление элемента в конец начало списка. </summary>
	 * <param name=value> Значение эл-та. </param> */
	void push_front(List_Type value)
	{
		if (!_head) { _head = node_ptr(new Node{ value }); _tail = _head; }
		else
			this->_head = node_ptr(new Node{ value, nullptr, _head });
		_size++;
	}

	/* <summary> Удаление последнего элемента списка. </summary>
	 * <returns> Значение последнего элемента списка. </returns> */
	List_Type pop_back()
	{
		if (!_head && !_tail) throw std::exception("blist is Empty");

		auto result = _tail->_value;
		_tail = _tail->_previous;
		_tail->_next = nullptr;

		_size--;

		return result;
	}

	/* <summary> Удаление первого элемента списка. </summary>
	 * <returns> Значение первого элемента списка. </returns> */
	List_Type pop_front()
	{
		if (!_head && !_tail) throw std::exception("blist is Empty");

		auto result = _head->_value;
		_head = _head->_next;

		_size--;
		return result;
	}

	/* <summary> Вставка элемента в определенную позицию списка. </summary>
	 * <param name=value> Значение элемента </param>
	 * <param name=value> Индекс </param> */
	void insert(List_Type value, size_t index)
	{
		if (index == 0) { push_front(value); return; }
		if (index == _size) { push_back(value); return; }

		auto previous = this->get_node(index - 1);
		previous->_next = node_ptr(new Node{ value, previous, previous->_next });

		_size++;
	}

	/* <returns> Размер списка </returns> */
	size_t size() { return this->_size; }
	/* <returns> Флаг: пустой ли список </returns> */
	bool empty() { return !this->_size; }
	/* <summary> Очистка списка. </summary> */
	void clear()
	{
		for (size_t i = 0; i < this->_size; i++)
			pop_back();
	}

	typedef blist_iterator <Node> iterator;
	typedef blist_iterator <const Node> const_iterator;

	/* --------------------------- ITERATORs STL FUNCTIONS --------------------------------------------------------- */

	/* <returns> Итератор, указывающий на начало списка </returns> */
	iterator begin() { if (_head) return iterator(_head.get()); else throw std::exception("blist is Empty"); }
	/* <returns> Итератор, указывающий на конец списка </returns> */
	iterator end() { return nullptr; }

	/* <returns> Константный итератор, указывающий на начало списка </returns> */
	const_iterator cbegin() const { if (_head) return iterator(_head.get()); else throw std::exception("blist is Empty"); }
	/* <returns> Константный итератор, указывающий на конец списка </returns> */
	const_iterator cend() const { return nullptr; }

	/* --------------------------- BLIST OPERATORs OVERLOADing --------------------------------------------------------- */

	/* <summary> Перегрузка оператора вывода для узла списка </summary>
	 * <param name=out> Поток вывода </param>
	 * <param name=node> Узел </param>
	 * <returns> Поток вывода </returns> */
	friend std::ostream& operator<<(std::ostream out, const Node& node) { return out << node._value; }

	/* <summary> Перегрузка оператора [] </summary>
	 * <param name=n> Индекс </param>
	 * <returns> Возвращает элемент списка под индексом n </returns> */
	List_Type operator[](size_t n)
	{
		// проверка наличия элементов в списке
		if (empty()) throw std::exception("blist is Empty");
		if (n >= this->_size) throw std::exception("blist error: index out of range!");
		return get_node(n)->_value;
	}

	/* <summary> Получение узла по индексу </summary>
	 * <param name=n> Индекс </param>
	 * <returns> Возвращает узел под индексом n </returns> */
	node_ptr& get_node(size_t n)
	{
		// указатель на текущий элемент
		Node* current = nullptr;

		if (n == 0) return this->_head;
		if (n == this->_size - 1) return this->_tail;

		// выбор, с какой стороны удобнее добраться до элемента
		if (n < _size / 2)
		{
			// поиск слева направо
			current = this->_head.get();
			for (size_t i = 0; i < n - 1; i++)
				current = current->_next.get();
			return current->_next;
		}
		else
		{
			// поиск справа налево
			current = this->_tail.get();
			for (size_t i = _size - 1; i > n + 1; i--)
				current = current->_previous.get();
			return current->_previous;
		}
	}

private:
	// голова - начало списка
	node_ptr _head;
	// хвост - конец списка
	node_ptr _tail;
	// размер списка
	size_t _size;
};

