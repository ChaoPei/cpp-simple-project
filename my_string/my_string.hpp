/**
 * 实现一个string类
 * 联系C++11的右值引用
 */

#ifndef _MY_STRING_HPP
#define _MY_STRING_HPP

#include <cstring>
#include <iostream>

namespace Rocco {
    class MyString {
    private:
        char* _data;
        std::size_t _len;
    
        void _init_data(const char *s) {
            _data = new char[_len + 1];
            memcpy(_data, s, _len);
            _data[_len] = '\0';
        }
    
    public:
        // 构造函数
        MyString(): _data(nullptr), _len(0) {}
        MyString(const char* s): _len(strlen(s)) {
            _init_data(s);
        }
    
        // 拷贝构造
        MyString(const MyString& rhs) {
            _len = rhs._len;
            _init_data(rhs._data);
            std::cout << "Copy Constructor is called! source: " << rhs._data << std::endl; 
        }
    
        // 转移构造
        MyString(MyString&& rhs) {
            std::cout << "Move Constructor is called! source: " << rhs._data << std::endl; 
            _len = rhs._len;
            _init_data(rhs._data);
            rhs._len = 0;
            rhs._data = nullptr;
        }
    
        // 赋值
        MyString& operator=(const MyString& rhs) {
            if(this != &rhs) {
                MyString tmp(rhs);
                std::swap(tmp._len, this->_len);
                std::swap(tmp._data, this->_data);
            }
            std::cout << "Copy Assignment is called! source: " << rhs._data << std::endl; 
            return *this;
        }
    
        // 转移赋值
        MyString& operator=(MyString&& rhs) {
            std::cout << "Move Constructor is called! source: " << rhs._data << std::endl; 
            if(this != &rhs) {
                MyString tmp(rhs);
                std::swap(tmp._len, this->_len);
                std::swap(tmp._data, this->_data);
    
                rhs._len = 0;
                rhs._data = nullptr;
            }
            return *this;
        }
    
        virtual ~MyString() { 
            if (_data) {
                free(_data); 
            } 
        } 
    };
}

#endif // _MY_STRING_HPP

