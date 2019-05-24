#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {

private:
	size_t currentlength;
	size_t initsize;
	T ** data;

	void doublespace(){
        T ** tmp = data;

        initsize *= 2;
        data = new T*[initsize];
        for(int i = 0;i < currentlength; ++i)
            data[i] = new T(*tmp[i]);
        for(int i = 0;i < currentlength;++i)
			delete tmp[i];
        delete [] tmp;
	};

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator worith same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */

	class const_iterator;
	class iterator {
		friend class vector<T>;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 T ** it;
		 size_t index;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator(T ** n = nullptr, size_t ind = 0 ):it(n), index(ind){}

		iterator operator+(const int &n) const {
			T ** ret = it + n;
			iterator rete(ret,index + (size_t)n);
			return rete;
		}
		iterator operator-(const int &n) const {
			T ** ret = it - n;
			iterator rete(ret,index - (size_t)n);
			return rete;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			return (index - rhs.index);
		}
		iterator operator+=(const int &n) {
			it = it + n;
			index += n;
			return *this;
		}
		iterator operator-=(const int &n) {
			it = it - n;
			index -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			const int n = 1;
			it = it + n;
			index += n;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			const int n = 1;
			it = it + n;
			index += n;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = * this;
			const int n = 1;
			it = it - n;
			index -= n;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			const int n = 1;
			it = it - n;
			index -= n;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return **it;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return(index == rhs.index);
		}
		bool operator==(const const_iterator &rhs) const {
			return(index == rhs.index);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return(index != rhs.index);
		}
		bool operator!=(const const_iterator &rhs) const {
			return(index != rhs.index);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
	private:
		T ** it;
		size_t index;
	public:
		const_iterator(T ** n = nullptr, size_t ind = 0):it(n), index(ind){}

		iterator operator+(const int &n) const {
			T ** ret = it + n;
			iterator rete(ret,index + n);
			return rete;
		}
		iterator operator-(const int &n) const {
			T ** ret = it - n;
			iterator rete(ret,index - n);
			return rete;
		}

		int operator-(const iterator &rhs) const {
			return (it - rhs.it);
		}

		iterator operator+=(const int &n) {
			it = it + n;
			index += n;
			return *this;
		}
		iterator operator-=(const int &n) {
			it = it - n;
			index -= n;
			return *this;
		}

		iterator operator++(int) {
			const int n = 1;
			it = it + n;
			index += n;
			return *this;
		}

		const_iterator& operator++() {
			const int n = 1;
			it = it + n;
			index += n;
			return *this;
		}

		const_iterator operator--(int) {
			const int n = 1;
			it = it - n;
			index -= n;
			return *this;
		}

		const_iterator& operator--() {
			const int n = 1;
			it = it - n;
			index -= n;
			return *this;
		}

		T& operator*() const{
			T & ret = ** it;
			return ret;
		}
		bool operator==(const iterator &rhs) const {
			return(index == rhs.index);
		}
		bool operator==(const const_iterator &rhs) const {
			return(index == rhs.index);
		}

		bool operator!=(const iterator &rhs) const {
			return(index != rhs.index);
		}
		bool operator!=(const const_iterator &rhs) const {
			return(index != rhs.index);
		}
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector(size_t i = 0, size_t j = 10):currentlength(i),initsize(j) {
        data =  new T* [initsize];
	}
	vector(const vector &other) {
	    initsize = other.capacity();
	    data = new T *[initsize];
	    for(currentlength = 0;currentlength < other.size();++currentlength)
	        data[currentlength] = new T (other[currentlength]);
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for(int i = 0;i < currentlength;++i)
			delete data[i];
	    delete [] data;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return (*this);
		for(int i = 0;i < currentlength;++i)
			delete data[i];
	    delete[]data;

        initsize = other.capacity();
        currentlength = other.currentlength;

        data = new T* [initsize];
        for(int i = 0; i < other.size();++i)
            data[i] = new T(other[i]);
        return (*this);
	}//afhoansnajlngkdlsngl
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
	    return *data[pos];
	}
	const T & at(const size_t &pos) const {
        return *data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
	    return *data[pos];
	}
	const T & operator[](const size_t &pos) const {
	    return *data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    return *data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
	    return *data[currentlength - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
         iterator iter(data, 0);
         return iter;
	}
	const_iterator cbegin() const {
        const_iterator iter(data, 0);
        return iter;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    iterator iter( data + currentlength,currentlength);
	    return iter;
	}
	const_iterator cend() const {
		const_iterator iter(data +currentlength,currentlength);
		return iter;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    if (currentlength == 0) return true;
	    return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return currentlength;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {
	    return initsize;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i = 0;i < currentlength;++i)
			delete data[i];
	    currentlength = 0;

	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
        if (currentlength >= initsize)
            doublespace();
		size_t i = currentlength;
		while(i > pos.index){
			data[i] = data[i - 1];
			i--;
		}
		data[pos.index] = new T(value);
		currentlength++;
		iterator iter( & data[pos.index],pos.index);
		return iter;

	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
        if(currentlength >= initsize)
            doublespace();
        size_t i = currentlength;
        while(i > ind){
            data[i] = data[i - 1];
        }
        data[ind] = new T(value);
        currentlength++;
        iterator iter( & data[ind]);
        return iter;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		size_t i = pos.index;
		delete data[i];
		currentlength--;
		if(i == currentlength) {
			return this->end();
		}

		while ( i < currentlength) {
			data[i] = data[i + 1];
			i++;
		}
		iterator ret(& data[pos.index],pos.index);
		return ret;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
	    size_t i = ind;
	    delete data[ind];
        while(i < currentlength) {
			data[i] = data[i + 1];
			i++;
		}
        currentlength--;
        iterator ret( & data[ind], ind);
        return ret;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    if(currentlength >= initsize)
	        doublespace();
        data[currentlength] =new T(value);
        currentlength++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
        currentlength--;
        delete data[currentlength];
	}
};
}

#endif
