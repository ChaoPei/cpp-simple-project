#ifndef _MY_VECTOR_HPP
#define _MY_VECTOR_HPP

namespace Rocco {
    
    #define FREE_SPACE_CAPCITY 5
    
    template <typename T>
    class MyVector {
    
    private:
        int _size;
        int _capcity;
        T* _data;
    
    public:

        // 构造函数
        explicit MyVector(int sz = 0): _size(sz), _capcity(sz+FREE_SPACE_CAPCITY) {
            _data = new T[_capcity];    // 分配初始空间
        }

        // 拷贝构造函数
        MyVector(const MyVector& rhs): _size(0), _capcity(0), _data(nullptr) {
            // 调用赋值操作
            *this = rhs;
        }

        // 赋值操作
        MyVector& operator=(const MyVector& rhs) {
            if(this != &rhs) {
                delete[] _data;
                _size = rhs._size;
                _capcity = rhs._capcity;
                _data = new T[_capcity];
                for(int i = 0; i < _size; ++i) {
                    _data[i] = rhs._data[i];
                }
            }
            return *this;
        }

        // resize：改变size的大小
        void resize(int newSize) {
            if(newSize < _size) {
                return;     // 如果resize比原size小，则什么也不做
            }

            if(newSize > _capcity) {
                reserve(2 * newSize + 1);
            }
            _size = newSize;
        }

        // reserve：重新分配内存
        void reserve(int newCapcity) {
            if(newCapcity < _size) {
                return;
            }

            T* tmp = _data;
            T* _data = new T[newCapcity];
            for(int i = 0; i < _size; ++i) {
                _data[i] = tmp[i];
            }

            delete[] tmp;
        }

        T& operator[] (int idx) {
            return _data[idx];
        }

        const T& operator[](int idx) const {
            return _data[idx];
        }

        bool isEmpty() const {
            return size() == 0;
        }

        int size() const {
            return _size;
        }

        int capcity() const {
            return _capcity;
        }

        void push_back(T x) {
            if(_size == _capcity) {
                reserve(2 * _capcity + 1);
            }
            _data[_size++] = x;
        }

        void pop_back() {
            --_size;
        }

        const T& back() const {
            return _data[size() - 1];
        }

        const T& front() const {
            return _data[0];
        }

        typedef T* iterator;
        typedef const T* const_iterator;
        
        iterator begin() {
            return &_data[0];
        }

        iterator end() {
            return &_data[size()];
        }

        const_iterator cbegin() const {
            return &_data[0];
        }

        const_iterator cend() const {
            return &_data[size()];
        }
    };
}

#endif // _MY_VECTOR_HPP