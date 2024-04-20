#pragma once
#include "Config.hpp"
#include <new>

template <class T1, class T2>
inline void Swap(T1& a, T2& b)
{
    T1 c(a);
    a = b;
    b = c;
}

template<typename T>
class Allocator
{
public:
     T* allocate(size_t cnt)
	{
		return (T*)operator new(cnt* sizeof(T));
	}
	void deallocate(T* ptr)
	{
		::operator delete(ptr);
	}
    void deallocate(T* ptr, size_t n) 
	{
        ::operator delete(ptr, n * sizeof(T));
    }
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};

inline u32 CalculateCapacityGrow( u32 capacity,  u32 minCapacity) 
        {
            if (capacity < minCapacity)
                capacity = minCapacity;
            if (capacity < 8)
            {
                capacity = 8;
            }
            else
            {
                // Round up to the next power of 2 and multiply by 2 (http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2)
                capacity--;
                capacity |= capacity >> 1;
                capacity |= capacity >> 2;
                capacity |= capacity >> 4;
                capacity |= capacity >> 8;
                capacity |= capacity >> 16;
				capacity++;
			
 
            }
            return capacity;
        }

//! Sinks an element into the heap.
template<class T>
inline void heapsink(T*array, s32 element, s32 max)
{
	while ((element<<1) < max) // there is a left child
	{
		s32 j = (element<<1);

		if (j+1 < max && array[j] < array[j+1])
			j = j+1; // take right child

		if (array[element] < array[j])
		{
			T t = array[j]; // swap elements
			array[j] = array[element];
			array[element] = t;
			element = j;
		}
		else
			return;
	}
}


//! Sorts an array with size 'size' using heapsort.
template<class T>
inline void heapsort(T* array_, s32 size)
{
	// for heapsink we pretent this is not c++, where
	// arrays start with index 0. So we decrease the array pointer,
	// the maximum always +2 and the element always +1

	T* virtualArray = array_ - 1;
	s32 virtualSize = size + 2;
	s32 i;

	// build heap

	for (i=((size-1)/2); i>=0; --i)
		heapsink(virtualArray, i+1, virtualSize-1);

	// sort array, leave out the last element (0)
	for (i=size-1; i>0; --i)
	{
		T t = array_[0];
		array_[0] = array_[i];
		array_[i] = t;
		heapsink(virtualArray, 1, i + 1);
	}
}



template<class T>
inline void heapify(T* array_, s32 root, s32 end,s32 (*comp)(T, T))
{
		s32 largest = root;
		s32 left = 2 * root + 1;
		s32 right = 2 * root + 2;

		if (left < end && comp(array_[left], array_[largest]) > 0)
			largest = left;

		if (right < end && comp(array_[right], array_[largest]) > 0)
			largest = right;

		if (largest != root) {
			Swap(array_[root], array_[largest]);
			heapify(array_,largest, end,comp);
		}

}

template<class T>
inline void heapsort(T* array_, s32 size,s32 (*comp)(T, T))
{
	

    // Construir o heap
    for (s32 i = size / 2 - 1; i >= 0; --i)
        heapify(array_,i, size,comp);

    // Ordenar o heap
    for (s32 i = size - 1; i > 0; --i) 
	{
        Swap(array_[0], array_[i]);
        heapify(array_,0, i,comp);
    }
	

}


template <class T>
struct AccessIterator
{

    AccessIterator() : ptr(0) {}
    explicit AccessIterator(T *ptr) : ptr(ptr) {}
    T *operator->() const { return ptr; }
    T &operator*() const { return *ptr; }
    AccessIterator<T> &operator++()    {        ++ptr;        return *this;    }
    AccessIterator<T> operator++(int)    {        AccessIterator<T> it = *this;       ++ptr;        return it;    }
    AccessIterator<T> &operator--()    {        --ptr;        return *this;    }
    AccessIterator<T> operator--(int)    {        AccessIterator<T> it = *this;        --ptr;        return it;    }
    AccessIterator<T> &operator+=(int value)    {        ptr += value;        return *this;    }
    AccessIterator<T> &operator-=(int value)    {        ptr -= value;        return *this;    }
    AccessIterator<T> operator+(int value) const { return AccessIterator<T>(ptr + value); }
    AccessIterator<T> operator-(int value) const { return AccessIterator<T>(ptr - value); }
    int operator-(const AccessIterator &rhs) const { return (int)(ptr - rhs.ptr); }
    bool operator==(const AccessIterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const AccessIterator &rhs) const { return ptr != rhs.ptr; }
    bool operator<(const AccessIterator &rhs) const { return ptr < rhs.ptr; }
    bool operator>(const AccessIterator &rhs) const { return ptr > rhs.ptr; }
    bool operator<=(const AccessIterator &rhs) const { return ptr <= rhs.ptr; }
    bool operator>=(const AccessIterator &rhs) const { return ptr >= rhs.ptr; }
    T *ptr;
};

template <class T>
struct AccessConstIterator
{
    AccessConstIterator() : ptr(0)    {    }
    explicit AccessConstIterator(T *ptr) : ptr(ptr)    {    }
    AccessConstIterator(const AccessIterator<T> &rhs) : ptr(rhs.ptr)    {    }
    AccessConstIterator<T> &operator=(const AccessIterator<T> &rhs)    {        ptr = rhs.ptr;        return *this;    }
    const T *operator->() const { return ptr; }
    const T &operator*() const { return *ptr; }
    AccessConstIterator<T> operator++(int)    {        AccessConstIterator<T> it = *this;        ++ptr;        return it;    }
    AccessConstIterator<T> &operator--()    {        --ptr;        return *this;    }
    AccessConstIterator<T> operator--(int)    {        AccessConstIterator<T> it = *this;        --ptr;        return it;    }
    AccessConstIterator<T> &operator+=(int value)    {        ptr += value;        return *this;    }
    AccessConstIterator<T> &operator-=(int value)    {        ptr -= value;        return *this;    }
    AccessConstIterator<T> operator+(int value) const { return AccessConstIterator<T>(ptr + value); }
    AccessConstIterator<T> operator-(int value) const { return AccessConstIterator<T>(ptr - value); }
    int operator-(const AccessConstIterator &rhs) const { return (int)(ptr - rhs.ptr); }
    bool operator==(const AccessConstIterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const AccessConstIterator &rhs) const { return ptr != rhs.ptr; }
    bool operator<(const AccessConstIterator &rhs) const { return ptr < rhs.ptr; }
    bool operator>(const AccessConstIterator &rhs) const { return ptr > rhs.ptr; }
    bool operator<=(const AccessConstIterator &rhs) const { return ptr <= rhs.ptr; }
    bool operator>=(const AccessConstIterator &rhs) const { return ptr >= rhs.ptr; }

    T *ptr;
};




template <class T, typename TAlloc = Allocator<T> > 
class Array
{
public:
    typedef AccessIterator<T> Iterator;
    typedef AccessConstIterator<T> ConstIterator;
  


    Array() :data(0), used(0), allocated(0), is_sorted(true), safeResize(false) {}

    Array(const Array<T, TAlloc>& other) : data(0)	{		*this = other;	}

    Array(u32 size) :data(0), used(0), allocated(0), is_sorted(true), safeResize(false)    {       allocate(size);    }

    ~Array()    {        clear();    }

    void resize(u32 new_capacity)
    {
        if (new_capacity <= allocated)            return;
        reserve(new_capacity);    
            
    }

    void compact()
    {
        if (used < allocated)	reserve(used);
    }


    void reserve(size_t new_size)//reallocate
    {
        if (new_size <= allocated)            return;
       	T* old_data = data;

		data = allocator.allocate(new_size); 
		allocated = new_size;

		s32 end = used < new_size ? used : new_size;

		for (s32 i=0; i<end; ++i)
		{
			allocator.construct(&data[i], old_data[i]);
		}
		for (u32 j=0; j<used; ++j)
			allocator.destruct(&old_data[j]);

		if (allocated < used)
			used = allocated;
		
		
		allocator.deallocate(old_data); 
    }

    const Array<T, TAlloc>& operator=(const Array<T, TAlloc>& other)
	{
		if (this == &other)			return *this;


		if (data) clear();


		if (other.allocated == 0)
			data = 0;
		else
			data = allocator.allocate(other.allocated); 

		used = other.used;

		is_sorted = other.is_sorted;
		allocated = other.allocated;
        safeResize = other.safeResize;

		for (u32 i=0; i<other.used; ++i)
			allocator.construct(&data[i], other.data[i]); 

		return *this;
	}


	bool operator == (const Array<T, TAlloc>& other) const
	{
		if (used != other.used)
			return false;
            
        if (allocated != other.allocated)
            return false;

        if (safeResize == other.safeResize)
            return true;
       
        if (is_sorted != other.is_sorted)
            return false;

		for (u32 i=0; i<other.used; ++i)
			if (data[i] != other[i])
				return false;
		return true;
	}


	void insert(const T& element, u32 index=0)
	{
		DEBUG_BREAK_IF(index > used) 

		if (used + 1 > allocated)
		{
			const T e(element);

			u32 newAlloc;
            if (safeResize)
            {

                newAlloc = used + 2;

            } else 
            {
                newAlloc = used + 1 + (allocated < 500 ?
							(allocated < 5 ? 5 : used) : used >> 2);
            }
            


			
			reserve(newAlloc);

            for (u32 i=used; i>index; --i)
			{
				if (i<used)	allocator.destruct(&data[i]);
				allocator.construct(&data[i], data[i-1]); 
			}
			//  add new element
			if (used > index)	
				allocator.destruct(&data[index]);
			
            allocator.construct(&data[index], e); 
			
		}
		else
		{

			if ( used > index )
			{

				allocator.construct(&data[used], data[used-1]);

		
				for (u32 i=used-1; i>index; --i)
				{
					data[i] = data[i-1];
				}
		
				data[index] = element;
			}
			else
			{
		
				allocator.construct(&data[index], element);
			}
		}

		++used;
		is_sorted = false;
	}


  	void push_back(const T& element)
	{
		insert(element, used);
	}

    void push_front(const T& element)
	{
		insert(element);
	}

    bool pop()
    {
        if (!used) return false;
        if (used)
        {
            allocator.destruct(&data[used-1]);
            --used;
            return true;
        }
        return false;      
    }

	void erase(u32 index)
	{
		DEBUG_BREAK_IF(index>=used) // access violation

		for (u32 i=index+1; i<used; ++i)
		{
			allocator.destruct(&data[i-1]);
			allocator.construct(&data[i-1], data[i]); 
		}

		allocator.destruct(&data[used-1]);

		--used;
	}

	void erase(u32 index, s32 count)
	{
		if (index>=used || count<1)
			return;
		if (index+count>used)
			count = used-index;

		u32 i;
		for (i=index; i<index+count; ++i)
			allocator.destruct(&data[i]);

		for (i=index+count; i<used; ++i)
		{
			if (i-count >= index+count)	
				allocator.destruct(&data[i-count]);

			allocator.construct(&data[i-count], data[i]); 

			if (i >= used-count)	
				allocator.destruct(&data[i]);
		}

		used-= count;
	}

	bool operator     !=(const Array<T, TAlloc>& other) const	{		return !(*this==other);	}
	T& operator       [](u32 index)	                   
	 {		DEBUG_BREAK_IF(index>=allocated) 		return data[index];	}
	const T& operator [](u32 index) const	        {		DEBUG_BREAK_IF(index>=allocated) 		return data[index];	}


	T* pointer()	{		return data;	}
	const T* const_pointer() const	{		return data;	}

    u32 size() const    {        return used;    }
    u32 capacity() const    {        return allocated;    }
	bool empty() const	{		return used == 0;	}	

 
    void clear()
    {
        for (u32 i=0; i<used; ++i)
             allocator.destruct(&data[i]);

		allocator.deallocate(data); 		
		data = 0;
		used = 0;
		allocated = 0;
		is_sorted = true;       
    }



	int search(const T& value) const
	{
		if (is_sorted)
		{
			return binary_search(value);
		}
		else
		{
			return linear_search(value);
		}
	}



	void sort()
	{
		if (!is_sorted)
		{
			heapsort(data, used);
			is_sorted = true;
		}
	}

    void sort(s32(*cmp)(T, T))
    {
        if (!is_sorted)
        {
            heapsort(data, used, cmp);
            is_sorted = true;
        }
    }


    Iterator begin()    {        return Iterator(data);    }
    Iterator end()    {        return Iterator(data + used);    }
    ConstIterator begin() const    {        return ConstIterator(data);    }
    ConstIterator end() const    {        return ConstIterator(data + used);    }
   

private:
    T* data;
    u32 used;//o que estamos usar "cursor"
    u32 allocated; // o que temos alocado
    bool is_sorted;
    bool safeResize;
    TAlloc allocator;


	 void allocate(u32 new_size)
    {      

		data = allocator.allocate(new_size); 
		allocated = new_size;
		used = new_size;
		for (u32 i=0; i<allocated; ++i)
		{
			allocator.construct(&data[i], T());
		}
		
    }

	int linear_search(const T& value) const
	{
	
		const T* ptr = data;
		const T* end = data + used;
		for (; ptr != end; ++ptr) 
		{
			if (*ptr == value) 
			{
				return ptr - data; 
			}
		}
		return -1;
	}

	int binary_search(const T& value) const
{
    const T* low = data;
    const T* high = data + used - 1;

    while (low <= high) 
    {
        const T* mid = low + (high - low) / 2;
        if (*mid == value) 
        {
            return mid - data;
        }
        if (*mid < value) 
        {
            low = mid + 1;
        } 
        else 
        {
            high = mid - 1;
        }
    }
    return -1;
}

};





const u32 NPOS = 0xffffffff;
static char endZero = 0;

class String
{
	public:


	String() : m_length(0), m_capacity(0) ,m_buffer(&endZero){}
	String(const String &str) : m_length(0), m_capacity(0), m_buffer(0)  { *this = str; }
	String(const char* str)   : m_length(0), m_capacity(0), m_buffer(0) { *this = str; }
	String(const char* str, u32 length) : m_length(0), m_capacity(0), m_buffer(0) { append(str, length); }

	explicit String(int number):m_length(0), m_capacity(0), m_buffer(0)
	{
		char tempBuffer[128];
    	sprintf(tempBuffer, "%d", number);
    	*this = tempBuffer;

	}
	explicit String(u32 number):m_length(0), m_capacity(0), m_buffer(0)
	{
		char tempBuffer[128];
		sprintf(tempBuffer, "%u", number);
		*this = tempBuffer;
	}
	explicit String(long number):m_length(0), m_capacity(0), m_buffer(0)
	{
		char tempBuffer[128];
		sprintf(tempBuffer, "%ld", number);
		*this = tempBuffer;
	
	}

	explicit String(unsigned long number):m_length(0), m_capacity(0), m_buffer(0)
	{
		char tempBuffer[128];
		sprintf(tempBuffer, "%lu", number);
		*this = tempBuffer;
		
	}


	~String() { if (m_capacity) delete[] m_buffer;}

	void compact()
	{
		if (m_capacity)	reserve(m_length + 1);
	}

	String& operator=(const String &str)
	{
		if (this == &str)			return *this;
		resize(str.m_length);
		Copy(str.m_buffer, m_length);
		return *this;
	}



	String& operator=(const char* rhs)
    {
       size_t rhsm_Length = strlen(rhs);
       resize(rhsm_Length);
       Copy(rhs, rhsm_Length);
       return *this;
    }

	String operator + (const String& rhs) const
	{
		String ret;
        ret.resize(m_length + rhs.m_length);
        CopyChars(ret.m_buffer, m_buffer, m_length);
        CopyChars(ret.m_buffer + m_length, rhs.m_buffer, rhs.m_length);
        
        return ret;
	}
	String operator + (const char* rhs) const
    {
		size_t rhsm_Length = strlen(rhs);
		String ret;
		ret.resize(m_length + rhsm_Length);
		CopyChars(ret.m_buffer, m_buffer, m_length);
		CopyChars(ret.m_buffer + m_length, rhs, rhsm_Length);
		return ret;
	
	}
	   String operator + (char rhs) const
    {
        String ret(*this);
        ret += rhs;
        
        return ret;
    }




	String& operator +=(const String& rhs)
    {
        u32 oldm_Length = m_length;
        resize(m_length + rhs.m_length);
        memcpy(&m_buffer[oldm_Length],  rhs.m_buffer, rhs.m_length);
        return *this;
    }


    String& operator +=(const char* rhs)
    {

		u32 rhsm_Length = strlen(rhs);
		u32 oldm_Length = m_length;
		resize(m_length + rhsm_Length);
		memcpy(&m_buffer[oldm_Length], rhs, rhsm_Length);
		return *this;	
	}

	String& operator+=(const char rhs)
	{
		u32 oldm_Length = m_length;
		resize(m_length + 1);
		m_buffer[oldm_Length] = rhs;
		return *this;
	}


    String& append(const String& str)        {            return *this += str;        }
    String& append(const char* str)        {            return *this += str;        }
    String& append(char c)        {            return *this += c;        }
    String& append(const char* str, u32 length)
    {
		if (str)
		{
			u32 oldm_Length = m_length;
			resize(m_length + length);
			memcpy(&m_buffer[oldm_Length], str, length);
		}
       
        return *this;
    }

	float ToFloat()
    {
        return (float)atof(m_buffer);
    }
    double ToDouble()
    {
        return atof(m_buffer);
    }
    int ToInt()
    {
        return atoi(m_buffer);
    }




	const char*  c_str() const { return m_buffer; }
    int 	    length() const { return(int) m_length; }
    int         size() const { return(int) m_length; }
    int       capacity() const { return(int) m_capacity; }
    bool         empty() const { return m_length == 0; }

	char& operator[](u32 index) { DEBUG_BREAK_IF(index>m_length); return m_buffer[index]; }
	const char& operator[](u32 index) const {  DEBUG_BREAK_IF(index>m_length); return m_buffer[index]; }

    bool operator ==(const String& rhs) const
    {
         if (m_length != rhs.m_length) return false;
         return strcmp(m_buffer, rhs.m_buffer) == 0; 
    }
    bool operator !=(const String& rhs) const { return strcmp(m_buffer, rhs.m_buffer) != 0; }
    bool operator <(const String& rhs) const { return strcmp(m_buffer, rhs.m_buffer) < 0; }
    bool operator >(const String& rhs) const { return strcmp(m_buffer, rhs.m_buffer) > 0; }
    bool operator ==(const char* rhs) const { return strcmp(m_buffer, rhs) == 0; }
    bool operator !=(const char* rhs) const { return strcmp(m_buffer, rhs) != 0; }
    bool operator <(const char* rhs) const { return strcmp(m_buffer, rhs) < 0; }
    bool operator >(const char* rhs) const { return strcmp(m_buffer, rhs) > 0; }

    String Left(u32 count) const
    {
        if (count >= m_length)
            return *this;
        return String(m_buffer, count);
    }

    String Right(u32 count) const
    {
        if (count >= m_length)
            return *this;
        return String(m_buffer + m_length - count, count);
    }

    String Mid(u32 pos, u32 count) const
    {
        if (pos >= m_length)
            return "";
        if (pos + count >= m_length)
            return String(m_buffer + pos);
        return String(m_buffer + pos, count);
    }

    String Section(u32 Pos1, u32 Pos2) const
    {
        if (Pos1 >= m_length || Pos2 >= m_length || Pos1 > Pos2)
            return "";
        
        return this->substr(Pos1 , Pos2 - Pos1);       
    }


	unsigned long ToHash() const
    {
        u32 hash = 2166136261u;
        const char* ptr = m_buffer;
        while (*ptr)
        {
            hash ^= (uint8_t)(*ptr);
			hash *= 16777619;	 
            ++ptr;
        }
        return hash;
    }
	String& say()
	{
		printf("%s", m_buffer);
		return *this;
	}
	String& sayln()
	{
		printf("%s\n", m_buffer);
		return *this;
	}
	 void replace(char replaceThis, char replaceWith, bool caseSensitive)
        {
            if (caseSensitive)
            {
                for (u32 i = 0; i < m_length; ++i)
                {
                    if (m_buffer[i] == replaceThis)
                        m_buffer[i] = replaceWith;
                }
            }
            else
            {
                replaceThis = (char)tolower(replaceThis);
                for (u32 i = 0; i < m_length; ++i)
                {
                    if (tolower(m_buffer[i]) == replaceThis)
                        m_buffer[i] = replaceWith;
                }
            }
        }

        void replace(const String& replaceThis, const String& replaceWith, bool caseSensitive)
        {
            u32 nextPos = 0;

            while (nextPos < m_length)
            {
                u32 pos = find(replaceThis, nextPos, caseSensitive);
                if (pos == NPOS)
                    break;
                replace(pos, replaceThis.m_length, replaceWith);
                nextPos = pos + replaceWith.m_length;
            }
        }

    void replace(u32 pos, u32 length, const String& replaceWith)
    {
        // If substring is illegal, do nothing
        if (pos + length > m_length)
            return;

        replace(pos, length, replaceWith.m_buffer, replaceWith.m_length);
    }

    void replace(u32 pos, u32 length, const char* replaceWith)
    {
        // If substring is illegal, do nothing
        if (pos + length > m_length)
            return;

        replace(pos, length, replaceWith, strlen(replaceWith));
    }
        
    u32 find(char c, u32 startPos, bool caseSensitive) const
    {
        if (caseSensitive)
        {
            for (u32 i = startPos; i < m_length; ++i)
            {
                if (m_buffer[i] == c)
                    return i;
            }
        }
        else
        {
            c = (char)tolower(c);
            for (u32 i = startPos; i < m_length; ++i)
            {
                if (tolower(m_buffer[i]) == c)
                    return i;
            }
        }

        return NPOS;
    }

        u32 find(const String& str, u32 startPos, bool caseSensitive) const
        {
            if (!str.m_length || str.m_length > m_length)
                return NPOS;

            char first = str.m_buffer[0];
            if (!caseSensitive)
                first = (char)tolower(first);

            for (u32 i = startPos; i <= m_length - str.m_length; ++i)
            {
                char c = m_buffer[i];
                if (!caseSensitive)
                    c = (char)tolower(c);

                if (c == first)
                {
                    u32 skip = NPOS;
                    bool found = true;
                    for (u32 j = 1; j < str.m_length; ++j)
                    {
                        c = m_buffer[i + j];
                        char d = str.m_buffer[j];
                        if (!caseSensitive)
                        {
                            c = (char)tolower(c);
                            d = (char)tolower(d);
                        }

                        if (skip == NPOS && c == first)
                            skip = i + j - 1;

                        if (c != d)
                        {
                            found = false;
                            if (skip != NPOS)
                                i = skip;
                            break;
                        }
                    }
                    if (found)
                        return i;
                }
            }

            return NPOS;
        }
	
	String replace(char replaceThis, char replaceWith, bool caseSensitive) const
    {
        String ret(*this);
        ret.replace(replaceThis, replaceWith, caseSensitive);
        return ret;
    }

    String replace(const String& replaceThis, const String& replaceWith, bool caseSensitive=true) const
    {
        String ret(*this);
        ret.replace(replaceThis, replaceWith, caseSensitive);
        return ret;
    }
    void replace(u32 pos, u32 length, const char* srcStart, u32 srcLength)
    {
        int delta = (int)srcLength - (int)length;

        if (pos + length < m_length)
        {
            if (delta < 0)
            {
                Move(pos + srcLength, pos + length, m_length - pos - length);
                resize(m_length + delta);
            }
            if (delta > 0)
            {
                resize(m_length + delta);
                Move(pos + srcLength, pos + length, m_length - pos - length - delta);
            }
        }
        else
            resize(m_length + delta);

        CopyChars(m_buffer + pos, srcStart, srcLength);
    }
    String substr(u32 pos) const
    {
        if (pos < m_length)
        {
            String ret;
            ret.resize(m_length - pos);
            CopyChars(ret.m_buffer, m_buffer + pos, ret.m_length);

            return ret;
        }
        else
            return String();
    }

    String substr(u32 pos, u32 length) const
    {
        if (pos < m_length)
        {
            String ret;
            if (pos + length > m_length)
                length = m_length - pos;
            ret.resize(length);
            CopyChars(ret.m_buffer, m_buffer + pos, ret.m_length);

            return ret;
        }
        else
            return String("");
    }

    String trim() const
    {
        u32 trimStart = 0;
        u32 trimEnd = m_length;

        while (trimStart < trimEnd)
        {
            char c = m_buffer[trimStart];
            if (c != ' ' && c != 9)
                break;
            ++trimStart;
        }
        while (trimEnd > trimStart)
        {
            char c = m_buffer[trimEnd - 1];
            if (c != ' ' && c != 9)
                break;
            --trimEnd;
        }

        return substr(trimStart, trimEnd - trimStart);
    }

    String trim_left() const
    {
        u32 trimStart = 0;
        while (trimStart < m_length)
        {
            char c = m_buffer[trimStart];
            if (c != ' ' && c != 9)
                break;
            ++trimStart;
        }

        return substr(trimStart);
    }

    String trim_right() const
    {
        u32 trimEnd = m_length;
        while (trimEnd > 0)
        {
            char c = m_buffer[trimEnd - 1];
            if (c != ' ' && c != 9)
                break;
            --trimEnd;
        }

        return substr(0, trimEnd);
    }

   void join(const Array<String>& subStrings, const String& glue)
    {
        *this = joined(subStrings, glue);
    }

    Array<String> split(char separator, bool keepEmptyStrings=false) 
    {
        return split(c_str(), separator, keepEmptyStrings);
    }

 

    Array<String> split(const char* str, char separator, bool keepEmptyStrings=false)
    {
        Array<String> ret;
        const char* strEnd = str + strlen(str);

        for (const char* splitEnd = str; splitEnd != strEnd; ++splitEnd)
        {
            if (*splitEnd == separator)
            {
                const ptrdiff_t splitLen = splitEnd - str;
                if (splitLen > 0 || keepEmptyStrings)
                    ret.push_back(String(str, splitLen));
                str = splitEnd + 1;
            }
        }

        const ptrdiff_t splitLen = strEnd - str;
        if (splitLen > 0 || keepEmptyStrings)
            ret.push_back(String(str, splitLen));
        
        return ret;
    }

        String joined(const Array<String>& subStrings, const String& glue)
        {
            if (subStrings.empty())
                return String();

            String joinedString(subStrings[0]);
            for (unsigned i = 1; i < subStrings.size(); ++i)
                joinedString.append(glue).append(subStrings[i]);

            return joinedString;
        }


        u32 find_last_of(char c, u32 startPos, bool caseSensitive=true) const
        {
            if (startPos >= m_length)
                startPos = m_length - 1;

            if (caseSensitive)
            {
                for (u32 i = startPos; i < m_length; --i)
                {
                    if (m_buffer[i] == c)
                        return i;
                }
            }
            else
            {
                c = (char)tolower(c);
                for (u32 i = startPos; i < m_length; --i)
                {
                    if (tolower(m_buffer[i]) == c)
                        return i;
                }
            }

            return NPOS;
        }

            u32 find_last_of(const String& str, u32 startPos=NPOS, bool caseSensitive=true) const
            {
                if (!str.m_length || str.m_length > m_length)
                    return NPOS;
                if (startPos > m_length - str.m_length)
                    startPos = m_length - str.m_length;

                char first = str.m_buffer[0];
                if (!caseSensitive)
                    first = (char)tolower(first);

                for (u32 i = startPos; i < m_length; --i)
                {
                    char c = m_buffer[i];
                    if (!caseSensitive)
                        c = (char)tolower(c);

                    if (c == first)
                    {
                        bool found = true;
                        for (u32 j = 1; j < str.m_length; ++j)
                        {
                            c = m_buffer[i + j];
                            char d = str.m_buffer[j];
                            if (!caseSensitive)
                            {
                                c = (char)tolower(c);
                                d = (char)tolower(d);
                            }

                            if (c != d)
                            {
                                found = false;
                                break;
                            }
                        }
                        if (found)
                            return i;
                    }
                }

                return NPOS;
            }

            bool starts_with(const String& str, bool caseSensitive=true) const
            {
                return find(str, 0, caseSensitive) == 0;
            }

            bool ends_with(const String& str, bool caseSensitive=true) const
            {
                u32 pos = find_last_of(str, length() - 1, caseSensitive);
                return pos != NPOS && pos ==(u32) length() - str.length();
            }




	private:
		u32 m_length;
		u32 m_capacity;
		char*       m_buffer;


	void Move(u32 dest, u32 src, u32 count)
    {
        if (count)
            memmove(m_buffer + dest, m_buffer + src, count);
    }

    void Copy(const char* src, u32 count)
    {
        if (count)
            memcpy(m_buffer, src, count);
    }
	static void CopyChars(char* dest, const char* src, unsigned count)
    {
      
        if (count)
            memcpy(dest, src, count);
       
    }
	void reserve(u32 newCapacity)
	{
	    if (newCapacity < m_length+1)
			newCapacity = m_length+1;
		if (newCapacity == m_capacity) return;

		char * newBuffer = new char[newCapacity];
		if (m_length)
			memcpy(newBuffer, m_buffer, m_length+1);
		
		if (m_capacity)
			delete[] m_buffer;

		m_buffer = newBuffer;
		m_capacity = newCapacity;

	}
	void resize(u32 newLength)
	{
			
		if (!m_capacity)
   		{
        	if (!newLength)       return;
        
			// Calculate initial capacity
			m_capacity = newLength + 1;
			if (m_capacity < 8)	m_capacity = 8;
			
			m_buffer = new char[m_capacity];
		}
		else
		{
			if (newLength && m_capacity < newLength + 1)
			{
				// Increase the capacity with half each time it is exceeded
				while (m_capacity < newLength + 1)
					m_capacity += (m_capacity + 1) >> 1;
				
				char* newBuffer = new char[m_capacity];
				if (m_length) memcpy(newBuffer, m_buffer, m_length);
				delete[] m_buffer;
				
				m_buffer = newBuffer;
			}
		}
    
    m_buffer[newLength] = '\0';
    m_length = newLength;


	}
};




//***********************************************************************************************************



template<typename T>
class Vector
{

    public:
     typedef AccessIterator<T> Iterator;
     typedef AccessConstIterator<T> ConstIterator;
   

        Vector(u32 capacity=8) : m_data(0x0), m_size(0), m_capacity(capacity) 
        {
            Reserve(m_capacity);
        }

        ~Vector()
        {
            clear();
             allocator.deallocate(m_data);
        }

        T& operator[](u32 index)
        {
            DEBUG_BREAK_IF(index>=m_size);
            return m_data[index];
        }

        const T& operator[](u32 index) const
        {
            DEBUG_BREAK_IF(index>=m_size);
            return m_data[index];
        }

        T* data() { return m_data; }
        const T* data() const { return m_data; }


        u32 size() const { return m_size; }
        bool empty() const { return m_size==0; }


        void clear()
        {
           for (u32 i=0; i<m_size; i++)
                allocator.destruct(&m_data[i]);
           m_size = 0;
        }

        void compact()
        {
            if (m_size<m_capacity)
            {
                reallocate(m_size);
            }
        }

        void reserve(u32 newCapacity)
        {
            if (newCapacity>m_capacity)
            {
                reallocate(newCapacity);
            }
        }
    

        T& push_back(const T& v)
        {
            if (m_size>=m_capacity)
                reallocate(m_capacity + m_capacity  / 2);
            m_data[m_size] = v;
            m_size++;
            return m_data[m_size-1];
        }

        T& push_back(T&& v)
        {
            if (m_size>=m_capacity)
                reallocate(m_capacity + m_capacity  / 2);
            m_data[m_size] = std::move(v);
            m_size++;
            return m_data[m_size-1];
        }

        template<typename... Args>
        T& emplace_back(Args&&... args)
        {
            if (m_size>=m_capacity)
            {
                reallocate(m_capacity + m_capacity  / 2);
            }
             allocator.construct(&m_data[m_size], T(std::forward<Args>(args)...));
            m_size++;
            return m_data[m_size-1];
        }

        bool pop_back()
        {
           if (m_size==0) return false;
            m_size--;
            allocator.destruct(&m_data[m_size]);
           return true;
        }

        bool remove(u32 index)
        {
            if (index>=m_size) return false;
            allocator.destruct(&m_data[index]);
            for (u32 i=index; i<m_size-1; i++)
                 m_data[i] = m_data[i+1];
            m_size--;
            return true;
        }

        bool contains(const T& v) const
        {
            for (u32 i=0; i<m_size; i++)
            {
                if (m_data[i]==v)
                    return true;
            }
            return false;
        }

        u32 position(const T& v) const
        {
            for (u32 i=0; i<m_size; i++)
            {
                if (m_data[i]==v)
                    return i;
            }
            return NPOS;
        }

    Iterator begin()    {        return Iterator(m_data);    }
    Iterator end()    {        return Iterator(m_data + m_size);    }
    ConstIterator begin() const    {        return ConstIterator(m_data);    }
    ConstIterator end() const    {        return ConstIterator(m_data + m_size);    }
  

    Iterator find(const T& value)
    {
        for (Iterator it = begin(); it != end(); ++it)
        {
            if (*it == value)
            {
                return it;
            }
        }
        return end(); 
    }

    ConstIterator find(const T& value) const
    {
        for (ConstIterator it = begin(); it != end(); ++it)
        {
            if (*it == value)
            {
                return it;
            }
        }
        return end(); 
    }


     Iterator erase(const Iterator& it)
    {
        u32 pos = (u32)(it - begin());
        if (pos >= m_size)
            return end();
        remove(pos);
        return begin() + pos;
    }

    bool remove(const T& value)
    {
        Iterator i = find(value);
        if (i != end())
        {
            erase(i);
            return true;
        }
        else
            return false;
    }

    private:
        T* m_data;
        u32 m_size;
        u32 m_capacity;
        Allocator<T> allocator;
        
  void reallocate(u32 newCapacity)
    {
      
        T* newData =  allocator.allocate(newCapacity);
        u32 oldSize = m_size;
        if (newCapacity < m_size)
            m_size = newCapacity;

        for (u32 i=0; i<m_size; i++)
        {
             allocator.construct(&newData[i], T(std::move(m_data[i])));
        }
       

        for (u32 i=0; i<oldSize; i++)
             allocator.destruct(&m_data[i]);
           
       
         allocator.deallocate(m_data);
        m_data = newData;
        m_capacity = newCapacity;
    }

    void Reserve(u32 newCapacity)
    {
           m_data =  allocator.allocate(newCapacity);       
            for (u32 i=0; i<newCapacity; i++)
                  allocator.construct(&m_data[i],T());
            m_capacity = newCapacity;
    }
};


template <typename T>
class List
{
  private:
    struct Node 
    {
        
        T value;
        Node* next;
        Node(T v)
        {
            value = v;
            next = 0x0;
        }
    };

        Node* getNode(u32 index) const
        {
            if (m_size==0)     return 0x0;
            if (index ==0)     return head;
            if (index==m_size) return tail;
            Node* temp = 0x0;
            for (u32 i=0; i<index; i++)
                 temp = temp->next;
            return temp;
        }

    u32      m_size;
    Node* head;
    Node* tail;
  public:

  class Iterator
	{
	public:
		Iterator() : current(0x0) {}
		Iterator& operator ++()    { current = current->next; return *this; }
        Iterator  operator ++(s32) { Iterator tmp = *this; current = current->next; return tmp; }


        Iterator& operator+=(s32 num) 
        {
            while (num-- && current != nullptr){current = current->next;}        
            return *this;
        }

        Iterator operator+(s32 num) const {        Iterator tmp = *this;        return tmp += num;    }

  		bool operator ==(const Iterator&      other) const { return current == other.current; }
		bool operator !=(const Iterator&      other) const { return current != other.current; }
		T & operator * () { return current->value; }
		T * operator ->() { return &current->value; }
	private:
		explicit Iterator(Node* begin) : current(begin) {}

		Node* current;

		friend class List<T>;
	};

    class ConstIterator
    {
    public:
        ConstIterator() : current(0x0) {}
        ConstIterator& operator ++()    { current = current->next; return *this; }
        ConstIterator  operator ++(s32) { ConstIterator tmp = *this; current = current->next; return tmp; }

        ConstIterator& operator+=(s32 num) 
        {
            while (num-- && current != nullptr){current = current->next;}        
            return *this;
        }

        ConstIterator operator+(s32 num) const {        Iterator tmp = *this;        return tmp += num;    }

        bool operator ==(const ConstIterator& other) const { return current == other.current; }
        bool operator !=(const ConstIterator& other) const { return current != other.current; }
        const T & operator * () { return current->value; }
        const T * operator ->() { return &current->value; }
    private:
        explicit ConstIterator(Node* begin) : current(begin) {}

		Node* current;

		friend class List<T>;

       
    };
    
    List()
    {
        m_size = 0;
        head   = 0x0;   
        tail   = 0x0;
    }
    ~List()
    {
            clear();
    }

    u32  size()  const { return m_size; }
    bool empty() const { return m_size==0; }

    bool remove(u32 index)
    {
        
        if (index==0)         {pop_front();return true;}
        if (index>=m_size)    {pop_back(); return true;}

        Node* prev = getNode(index-1);
        Node* tmp = prev->next;
        prev->next = tmp->next;
        delete tmp;
        m_size--;
        return true;
    }

    void insert(u32 index, const T& v)
    {
        
        if (index==0 || m_size==0) {push_front(v);return;}
        if (index>=m_size) {push_back(v); return;}
        Node* prev = getNode(index-1);
        Node* node = new Node(v);
        node->next = prev->next;
        prev->next = node;
        m_size++;
    }

    bool contains(const T &v)const 
    {
        Node* tmp = head;
        while(tmp)
        {
            if (tmp->value == v) return true;
            tmp = tmp->next;
        }
        return false;
    }


    bool set(u32 index, const T& v )
    {
        if (m_size==0) return false;
        Node* node = getNode (index);
        if (node!=0x0)
        {
            node->value=v;
            return true;
        }
        return false;
    }



    const T&  get(u32 index) const
    {
        DEBUG_BREAK_IF(index<0 || index>=m_size || m_size==0);
        if (index ==0)       return head->value;
        if (index == m_size) return tail->value;
        Node* node = head;
        for (u32 i=0; i < index;i++)
             node = node->next;
        return node->value;
    }

    void push_back(const T& v)
    {
        Node* node = new Node(v);
        if (m_size==0)
        {
             head=node;
             tail=node;
             m_size++;
             return;
        }

        tail->next = node;
        tail = node;
        m_size++;
    }

    void push_front(const T& v)
    {
        Node* node = new Node(v);
        if (m_size==0)
        {
             head=node;
             tail=node;
             m_size++;
             return;
        }
        node->next = head;
        head = node;               
        m_size++;
        
    }

    T pop_back()
    {
        DEBUG_BREAK_IF(m_size==0);
        T value = 0x0;
        if (m_size==0) return value;
        Node* tmp = head;
        Node* pre = head;
        while(tmp->next)
        {
             pre = tmp;
             tmp=tmp->next;   
        }

        tail = pre;
        tail->next=0x0;
        m_size--;
        if (m_size==0)
        {
            head=0x0;
            tail=0x0;
        }

        delete tmp;
        return value;       
    }

    T pop_front()
    {
        DEBUG_BREAK_IF(m_size==0);
        T value = 0x0;
        if (m_size==0) return value;
        Node* tmp = head;
        if (m_size==1)
        {
            tmp  = head;
            head = 0x0;
            tail = 0x0;
        } else 
        {
            head = head->next;
        }
        value = tmp->value;
        delete tmp;
        m_size--;       
        return value;
    }


    void clear()
    {
        while(!empty())
        {
            pop_front();
        }
    }

    ConstIterator begin() const
    {
        return ConstIterator(head);
    }

    ConstIterator end() const
    {
        return ConstIterator(0x0);
    }

    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(0x0);
    }

    Iterator find(const T& element)
    {
        for (Iterator it = begin(); it != end(); ++it)
        {
            if (*it == element)
                return it;
        }
        return end();
    }


};



template<typename T>
class HashNode 
{
public:
    char key[128];
    T value;
    HashNode* next;

    HashNode(const char* k, const T &v) : value(v), next(nullptr) 
    {
        u32 len = strlen(k);
        strcpy(key, k);
        key[len] = '\0';
    }
    ~HashNode() 
    {
      
    }



    u32 Count()
    {
        u32 count = 0;
        HashNode* current = this;
        while (current)
        {
            count++;
            current = current->next;
        }
        return count;
    
    }
};



template<typename T>
class HashTable 
{
private:
    HashNode<T>** table;
    HashNode<T>* m_Iter;
    u32 m_IterIndex;
    u32 capacity;
    u32 size;

    const float loadFactorThreshold = 0.75f;

    u32 HashStr( const char *index) const
    {
        u32 iIndex = 2166136261u;
        while( *index )
        {
            iIndex ^= *index;
            iIndex *= 16777619;
            index++;
        }
        iIndex = (iIndex >> 16) ^ iIndex;
        return iIndex & (capacity-1); // capacity must be a power of 2
    }


    void resizeTable(u32 newCapacity) 
    {
        size =0;

        u32 oldCapacity = capacity;
        capacity = newCapacity;

        HashNode<T>** newTable = new HashNode<T>*[newCapacity];
        for (size_t i = 0; i < newCapacity; ++i)
             newTable[i] = nullptr;

        for (size_t i = 0; i < oldCapacity; ++i)
         {
            HashNode<T>* currentNode = table[i];
            while (currentNode) 
            {
               
                size_t newIndex = HashStr(currentNode->key);

                HashNode<T>* newNode = new HashNode<T>(currentNode->key, currentNode->value);
                newNode->next = newTable[newIndex];
                newTable[newIndex] = newNode;            
                HashNode<T>* temp = currentNode;
                currentNode = currentNode->next;
                size ++;
                delete temp;
            }
        }
     

        delete[] table;
        table = newTable;
  
    }

    T eraseAt(size_t index, HashNode<T>* node) 
    {
        if (!node)        return T{}; 
        HashNode<T>* currentNode = table[index];
        HashNode<T>* prevNode = nullptr;
        while (currentNode)
        {
            if (currentNode == node) 
            {
                if (prevNode)
                    prevNode->next = currentNode->next;
                else
                    table[index] = currentNode->next;
                
                T value = currentNode->value; 
                delete currentNode; 
                --size;
                return value; 
            }
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        return T{}; 
    }

public:

     class Iterator 
     {
     private:
        HashTable<T>* table;
        size_t index;
        HashNode<T>* current;

    public:
        Iterator(HashTable<T>* table, size_t index, HashNode<T>* node)            : table(table), index(index), current(node) {}
        Iterator& operator++() 
        {
            if (current) 
            {
                current = current->next;
                if (!current) 
                {
                    ++index;
                    while (index < table->capacity && !table->table[index])
                        ++index;
                    if (index < table->capacity)
                        current = table->table[index];
                }
            }
            return *this;
        }
	    
        T & operator * () { return current->value; }
		T * operator ->() { return &current->value; }
        const char* key() const {return current->key;}
      
        bool operator==(const Iterator& other) const {return table == other.table && index == other.index && current == other.current; }
        bool operator!=(const Iterator& other) const {return !(*this == other);}

        void erase() {table->eraseAt(index, current);       }
    };

     class ConstIterator 
     {
     private:
        HashTable<T>* table;
        size_t index;
        HashNode<T>* current;

    public:
        ConstIterator(HashTable<T>* table, size_t index, HashNode<T>* node)       : table(table), index(index), current(node) {}

        ConstIterator& operator++() 
        {
            if (current) 
            {
                current = current->next;
                if (!current) 
                {
                    ++index;
                    while (index < table->capacity && !table->table[index])
                        ++index;
                    if (index < table->capacity)
                        current = table->table[index];
                }
            }
            return *this;
        }
	    
       const T & operator * () { return current->value; }
	   const T * operator ->() { return &current->value; }
    const char* key() const {return current->key;}
      

        bool operator==(const ConstIterator& other) const {return table == other.table && index == other.index && current == other.current; }
        bool operator!=(const ConstIterator& other) const {return !(*this == other);}

         void erase() {table->eraseAt(index, current);       }
    };

    HashTable(u32 Capacity=16) 
    {
        capacity =  CalculateCapacityGrow(Capacity, 8);
        size = 0;
        table = new HashNode<T>*[capacity];
        m_Iter = nullptr;
        m_IterIndex = 0;
        for (u32 i = 0; i < capacity; ++i)
        {
             table[i] = nullptr;
 
        }
    }

    ~HashTable() 
    {
        clear();
        delete[] table;
    }

    T first()  
    {
        if (size == 0)         return 0;
        T item = 0;
        m_IterIndex = 0;
        m_Iter = nullptr;

        for (size_t i = 0; i < capacity; i++) 
        {
            HashNode<T>* pCurrItem = table[i];
            while(pCurrItem)
            {
                    if (!item)    item = pCurrItem->value;
                    else
                    {
                        m_Iter = pCurrItem;
                        m_IterIndex = i;
                        return item;
                    }
                    pCurrItem = pCurrItem->next;
            }          
        }
        return item;
    }

    T next()  
    {
        if (!m_Iter)             return 0;
        T item = m_Iter->value;
        if (m_Iter->next)
        {
            m_Iter = m_Iter->next;
            return item;
        }
        for (size_t i = m_IterIndex+1; i < capacity; i++) 
        {
            if (table[i]) 
            {
                m_Iter = table[i];
                m_IterIndex = i;
                return item;
            }
          
        }
        m_Iter = nullptr;
        m_IterIndex = 0;
        return item;
    }



    void insert(const char* key, const T& value) 
    {
        if ((float)(size + 1) / capacity > loadFactorThreshold)
        {
            u32 newCapacity = CalculateCapacityGrow(capacity + 1, size + 1);
            resizeTable(newCapacity);
        }

            u32 index = HashStr(key);
            HashNode<T>* currentNode = table[index];

            HashNode<T>* newNode = new HashNode<T>(key, value);
            newNode->next = currentNode;
            table[index] = newNode;
            ++size;

    }

    bool find(const char* key,  T& value) const 
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        if (!currentNode)         {            return false;        }
        while (currentNode)
         {
          //  std::cout << "Key: " << currentNode->key << " Value: " << currentNode->value << std::endl;
            if (strcmp(currentNode->key, key) == 0) 
            {
                value = currentNode->value;
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    T get(const char* key) const
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        if (!currentNode)         {            return 0x0;        }
        while (currentNode)
         {
          //  std::cout << "Key: " << currentNode->key << " Value: " << currentNode->value << std::endl;
            if (strcmp(currentNode->key, key) == 0) 
            {
                return  currentNode->value;
            }
            currentNode = currentNode->next;
        }
        return 0x0;
    }
    bool contains(const char* key) const 
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        if (!currentNode)         {            return false;        }
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0)
            {
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }
    

    void erase(const char* key) 
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        HashNode<T>* prevNode = nullptr;
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0) 
           {
                if (prevNode) 
                {
                    prevNode->next = currentNode->next;
                } 
                else 
                {
                    table[index] = currentNode->next;
                }
                delete currentNode;
                --size;
                return;
            }
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
    }

    bool remove(const char* key,  T& value) 
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        if (!currentNode)         {            return false;        }
        HashNode<T>* prevNode = nullptr;
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0)
            {
                if (prevNode) 
                {
                    prevNode->next = currentNode->next;
                } 
                else 
                {
                    table[index] = currentNode->next;
                }
                value = currentNode->value;
                delete currentNode;
                --size;
                return true;
            }
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        return false;
    }
    bool remove(const char* key) 
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        if (!currentNode)         {            return false;        }
        HashNode<T>* prevNode = nullptr;
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0)
            {
                if (prevNode) 
                {
                    prevNode->next = currentNode->next;
                } 
                else 
                {
                    table[index] = currentNode->next;
                }
                delete currentNode;
                --size;
                return true;
            }
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        return false;
    }

    void clear()
    {
        if (size == 0)         {            return;        }

        for (size_t i = 0; i < capacity; ++i) 
        {
            HashNode<T>* currentNode = table[i];
            while (currentNode) 
            {
                HashNode<T>* nextNode = currentNode->next;
                delete currentNode;
                currentNode = nextNode;
            }
            table[i] = nullptr;
        }
        size = 0;
        m_Iter = nullptr;
        m_IterIndex = 0;
    }

     void clean() 
    {
        if (size == 0)         {            return;        }

        for (size_t i = 0; i < capacity; ++i) 
        {
            HashNode<T>* currentNode = table[i];
            while (currentNode) 
            {
                HashNode<T>* nextNode = currentNode->next;
                delete currentNode->value;
                delete currentNode;
                currentNode = nextNode;
            }
            table[i] = nullptr;
        }
        size = 0;
        m_Iter = nullptr;
        m_IterIndex = 0;
    }

    u32 Count() const { return size; }

   Iterator begin()
   {
        size_t index = 0;
        while (index < capacity && !table[index])
            ++index;
        if (index < capacity)
            return Iterator(this, index, table[index]);
        return end();
    }

    Iterator end() {return Iterator(this, capacity, nullptr);}


    ConstIterator begin() const
    {
        size_t index = 0;
        while (index < capacity && !table[index])
            ++index;
        if (index < capacity)
            return ConstIterator(this, index, table[index]);
        return end();
    }

    ConstIterator end() const {return ConstIterator(this, capacity, nullptr);}

    ConstIterator find(const char* key) const
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0)
                return ConstIterator(this, index, currentNode);
            currentNode = currentNode->next;
        }
        return end();
    }

    Iterator find(const char* key)
    {
        u32 index = HashStr(key);
        HashNode<T>* currentNode = table[index];
        while (currentNode) 
        {
            if (strcmp(currentNode->key, key) == 0)
                return Iterator(this, index, currentNode);
            currentNode = currentNode->next;
        }
        return end();
    }

    
private:
friend class Iterator;
friend class ConstIterator;

};

