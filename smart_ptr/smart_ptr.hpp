/**
 * 实现一个智能指针，添加线程安全功能
 */

#ifndef SMART_PTR_HPP_

#define SMART_PTR_HPP_

#include <atomic>
#include <functional>

namespace Rocco {
    namespace impl {

        // 删除指针
        class DefaultDeleter {
        public:
            template <typename T>
            void operator()(T *ptr) {
                delete ptr;
            }
        };

        // 调用构造函数
        class DestructorDeleter {
        public:
            template <typename T>
            void operator()(T *ptr) {
                ptr->~T();
            }
        };

        template <typename T>
        class SmartPtrImpl {
        private:
            T* m_ptr;                               // 指针
            std::atomic<unsigned int> m_count;      // 计数器是原子操作
            std::function<void(T *)> m_deleter;     // 释放指针的方法，模板参数
        
        public:
            // 构造函数
            template <typename D = DefaultDeleter>
            SmartPtrImpl(T* ptr = nullptr, D deleter = D()): m_ptr(ptr), m_deleter(deleter), m_count(1) {}
            
            // 在模板中传入构造T的参数
            template <typename D = DestructorDeleter, typename... Args>
            SmartPtrImpl(T* ptr, D deleter, Args&&... args) : m_ptr(ptr), m_deleter(deleter), m_count(1) {
                new (m_ptr) T(std::forward<Args>(args)...);
            }   

            // 利用传入的方法释放指针
            ~SmartPtrImpl() {
                m_deleter(m_ptr);
            }

            // 实现指针的操作符重载
            T* operator->() const {
                return m_ptr;
            }

            T& operator*() const {
                return *m_ptr;
            }

            // 获取指针
            T* get() const {
                return m_ptr;
            }

            // 持有指针，计数增加
            void hold() {
                ++m_count;
            }

            // 释放，技术减少
            bool release() {
                return --m_count == 0;
            }
        };

    } // namespace impl

    template <typename T>
    class SmartPtr {
    
    private:
        impl::SmartPtrImpl<T> *m_impl;
    
    public:
        explicit SmartPtr(impl::SmartPtrImpl<T> *impl) : m_impl(impl) {}

        template <typename D = impl::DefaultDeleter>
        explicit SmartPtr(T* ptr = nullptr, D deleter = D()) : m_impl(new impl::SmartPtrImpl<T>(ptr, deleter)) {}

        template <typename U, typename D = impl::DefaultDeleter>
        explicit SmartPtr(U* ptr = nullptr, D deleter = D()) : m_impl(new impl::SmartPtrImpl<T>(ptr, deleter)) {}

        // 析构函数：计数器减少
        ~SmartPtr() {
            if(m_impl->release()) {
                delete m_impl;
            }
        }

        // 拷贝构造函数：计数器增加
        SmartPtr(const SmartPtr<T> &rhs) {
            m_impl = rhs.m_impl;
            m_impl->hold();
        }

        template <typename U>
        SmartPtr(const SmartPtr<U> &rhs) {
            m_impl = rhs.m_impl;
            m_impl->hold();
        }

        // 赋值操作：利用拷贝构造函数实现
        SmartPtr<T>& operator=(const SmartPtr<T> &rhs) {
            SmartPtr<T> tmp(rhs);
            swap(tmp);
            return *this;
        }

        T* operator->() const {
            return m_impl->operator->();
        }

        T& operator*() const {
            return **m_impl;
        }

        T* get() const {
            return m_impl->get();
        }

        template <typename U>
        bool operator<(const SmartPtr<U> &rhs) {
            return m_impl->get() < rhs.get();
        }

        template <typename D = impl::DefaultDeleter>
        void reset(T *ptr = nullptr, D deleter = D()) {
            if(m_impl->release()) {
                delete m_impl;
            }

            m_impl = new impl::SmartPtrImpl<T>(ptr, deleter);
        }

        void swap(SmartPtr<T> &rhs) {
            std::swap(m_impl, rhs.m_impl);
        }

    };     

    template <typename T>
    void swap(Rocco::SmartPtr<T> &left, Rocco::SmartPtr<T> &right) {
        left.swap(right);
    }

    template <typename T, typename... Args> 
    SmartPtr<T> make_smart(Args&&... args) {
        char* ptr = static_cast<char*>(operator new(sizeof(T) * sizeof(impl::SmartPtrImpl<T>)));
        if(ptr == nullptr) {
            throw std::bad_alloc();
        }

        auto t_ptr = reinterpret_cast<T *>(ptr + sizeof(impl::SmartPtrImpl<T>));

        try {
            auto impl = new(ptr) impl::SmartPtrImpl<T>(t_ptr, impl::DestructorDeleter(), std::forward<T>(args)...);
            return SmartPtr<T>(impl);
        } catch(...) {
            operator delete(ptr);
            throw;
        }
    }
} // namespace Rocco


#endif // end SMART_PTR_HPP_