#pragma once
#include <vector>
#include <initializer_list>

template<typename T>
class FreeList {
private:
	class Iterator;
	class Const_Iterator;
	struct elt;
public:
	FreeList(): m_size(0), m_firstFree(-1) {}
	FreeList(int n) : m_size(0), m_firstFree(-1) {
		reserve(n);
		for (; n >= 0; --n) {
			push(0);
		}
	}
	FreeList(std::initializer_list<T> list) : m_size(0), m_firstFree(-1) {
		reserve(list.size());
		for (auto i = list.begin(); i != list.end(); ++i) {
			push(*i);
		}
	}
	~FreeList() {
		std::vector<elt>().swap(m_elements);
	}

	void swap(FreeList<T>& other) {
		m_elements.swap(other.m_elements);
		std::swap(m_firstFree, other.m_firstFree);
		std::swap(m_size, other.m_size);
	}

	Iterator push(const T& t) {
		++m_size;
		if (m_firstFree != -1) {
			const int index = m_firstFree;
			m_firstFree = m_elements[m_firstFree].next;
			m_elements[index].value = t;
			m_elements[index].removed = 0;
			return Iterator(this, index);
		}
		m_elements.emplace_back(t);
		return Iterator(this, m_size - 1);
	}
	Iterator push(T&& t) {
		++m_size;
		if (m_firstFree != -1) {
			const int index = m_firstFree;
			m_firstFree = m_elements[m_firstFree].next;
			m_elements[index].value = std::move(t);
			m_elements[index].removed = 0;
			return Iterator(this, index);
		}
		m_elements.push_back(elt(std::move(t)));
		return Iterator(this, m_size - 1);
	}

	template<typename ... Ts>
	Iterator emplace(Ts&&... ts) {
		++m_size;
		if (m_firstFree != -1) {
			const int index = m_firstFree;
			m_firstFree = m_elements[m_firstFree].next;
			m_elements[index].value = T(std::forward<Ts>(ts)...);
			m_elements[index].removed = 0;
			return Iterator(this, index);
		}
		m_elements.push_back(elt(std::forward<Ts>(ts)...));
		return Iterator(this, m_size - 1);
	}

	void remove(int index) {
		--m_size;
		m_elements[index].next = m_firstFree;
		m_elements[index].removed = 1;
		m_firstFree = index;
	}

	void remove(const Iterator& iter) {
		--m_size;
		m_elements[iter.m_index].next = m_firstFree;
		m_elements[iter.m_index].removed = 1;
		m_firstFree = iter.m_index;
	}

	void clear() {
		m_size = 0;
		m_elements.clear();
		m_firstFree = -1;
	}

	int size() const {
		return m_size;
	}

	Iterator find(const T& t) {
		Iterator itr = begin();
		for (; itr != end(); ++itr) {
			if (t == *itr) {
				return itr;
			}
		}
		return end();
	}

	void reserve(int n) {
		m_elements.reserve(n);
	}

	// Returns the nth element.
	T& operator[](int n) {
		return m_elements[n];
	}

	// Returns the nth element.
	const T& operator[](int n) const {
		return m_elements[n];
	}
	
	Iterator begin() {
		int i = 0; while (i < m_elements.size() && m_elements[i].removed) ++i;
		return Iterator(this, i);
	}

	// Return the Iterator at the end of the list
	Iterator end() {
		return Iterator(this, m_elements.size());
	}
	Const_Iterator begin() const {
		int i = 0; while (i < m_elements.size() && m_elements[i].removed) ++i;
		return Const_Iterator(this, i);
	}

	// Return the Iterator at the end of the list
	Const_Iterator end() const {
		return Const_Iterator(this, m_elements.size());
	}
private:
	struct elt {
		union {
			T value;
			int next;
		};
		bool removed;

		elt(): removed(0), next(-1) { }
		elt(const T& t) : removed(0), value(t) { }
		~elt() {};
	};
	std::vector<elt> m_elements;
	int m_firstFree;
	int m_size;	
private:
	class Iterator {
		friend class FreeList<T>;
	public:
		Iterator(FreeList* list, int index): m_list(list), m_index(index) {

		}
		~Iterator() {}

		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using const_pointer = T const*;
		using reference = T&;
		using const_reference = const T&;
		using iterator_category = std::forward_iterator_tag;

		// Pre increment (++it)
		Iterator& operator++() {
			do {
				m_index++;
			} while (m_index < m_list->m_elements.size() && m_list->m_elements[m_index].removed); // Skip the removed elements
			return *this;
		}

		// Post increment (it++)
		Iterator operator++(int) {
			Iterator retval = *this;
			++(*this);
			return retval;
		}

		// Deferencable
		reference operator*() {
			return m_list->m_elements[m_index].value;
		}

		// Const Deferencable
		const_reference operator*() const {
			return m_list->m_elements[m_index].value;
		}

		// Deferencable
		pointer operator->() {
			return &m_list->m_elements[m_index].value;
		}

		// Const Deferencable
		const_pointer operator->() const {
			return &m_list->m_elements[m_index].value;
		}

		// Iterator distance.
		int operator-(Iterator other) const {
			return std::abs(other.m_index - m_index);
		}

		// Boolean operations

		bool operator<(Iterator other) {
			return m_index < other.m_index;
		}

		bool operator<=(Iterator other) {
			return m_index <= other.m_index;
		}

		bool operator>(Iterator other) {
			return m_index < other.m_index;
		}

		bool operator>=(Iterator other) {
			return m_index <= other.m_index;
		}

		bool operator==(Iterator other) {
			return m_index == other.m_index;
		}

		bool operator!=(Iterator other) {
			return m_index != other.m_index;
		}
	private:
		FreeList<T>* m_list;
		int m_index;
	};

	class Const_Iterator {
		friend class FreeList<T>;
	public:
		Const_Iterator(const FreeList* list, int index) : m_list(list), m_index(index) {

		}
		~Const_Iterator() {}

		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator_category = std::forward_iterator_tag;

		// Pre increment (++it)
		Const_Iterator& operator++() {
			do {
				m_index++;
			} while (m_index < m_list->m_elements.size() && m_list->m_elements[m_index].removed); // Skip the removed elements
			return *this;
		}

		// Post increment (it++)
		Const_Iterator operator++(int) {
			Const_Iterator retval = *this;
			++(*this);
			return retval;
		}

		// Const Deferencable
		const_reference operator*() const {
			return m_list->m_elements[m_index].value;
		}

		// Deferencable
		const_pointer operator->() const {
			return &m_list->m_elements[m_index].value;
		}

		// Iterator distance.
		int operator-(Const_Iterator other) const {
			return std::abs(other.m_index - m_index);
		}

		// Boolean operations

		bool operator<(Const_Iterator other) const {
			return m_index < other.m_index;
		}

		bool operator<=(Const_Iterator other) const {
			return m_index <= other.m_index;
		}

		bool operator>(Const_Iterator other) const{
			return m_index < other.m_index;
		}

		bool operator>=(Const_Iterator other) const {
			return m_index <= other.m_index;
		}

		bool operator==(Const_Iterator other) const {
			return m_index == other.m_index;
		}

		bool operator!=(Const_Iterator other) const {
			return m_index != other.m_index;
		}
	private:
		const FreeList<T>* m_list;
		int m_index;
	};
};