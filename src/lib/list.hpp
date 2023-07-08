/*
 * list.hpp
 *
 *  Created on: 04.07.2023
 *      Author: nick
 */

#ifndef SRC_LIST_HPP_
#define SRC_LIST_HPP_

#include <array>

template<class T>
class list {
private:
	std::array<T*, 10> m_array;
	unsigned char m_count;

	class Iterator {
		std::array<T*, 10> *m_array;
		int m_index;
	public:
		Iterator(std::array<T*, 10> *array, int index) :
				m_array(array), m_index(index) {
		}

		Iterator begin() {
			return Iterator(m_array, 0);
		}
		Iterator end() {
			return Iterator(m_array, m_array->size());
		}
		bool operator==(const Iterator &b) {
			return this->m_index == b.m_index;
		}
		bool operator!=(const Iterator &b) {
			return this->m_index != b.m_index;
		}
		T* operator*() const {
			return (*this->m_array)[m_index];
		}
		// Prefix increment
		Iterator& operator++() {
			m_index++;
			return *this;
		}

		// Postfix increment
		Iterator operator++(int) {
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

	};
public:
	list() :
			m_count(0) {

	}
	void push(T *value) {
		m_array[m_count] = value;
		m_count++;
	}
	template<class ...Args>
	void emplace(Args ... args) {
		m_array[m_count] = new T(args...);
		m_count++;
	}
	bool empty() {
		return m_count == 0;
	}
	int size() {
		return m_count;
	}
	void pop() {
		m_count--;
		delete m_array[m_count];
	}
	Iterator iter() {
		return Iterator(&m_array, 0);
	}
	T* operator[](int index) {
		return m_array[index];
	}
};

#endif /* SRC_LIST_HPP_ */
