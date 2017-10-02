#ifndef _SIMPLE_SMART_PTR_HPP
#define _SIMPLE_SMART_PTR_HPP

namespace Rocco
{
    using uint = unsigned int;
    template <typename T>
    class SimpleSmartPtr
    {
    private:
        uint* m_count;
        T* m_ptr;
    
    public:
        SimpleSmartPtr(T* ptr = nullptr): m_ptr(ptr), m_count(new uint(1)) {}

        SimpleSmartPtr(const SimpleSmartPtr<T> &rhs): m_ptr(rhs.m_ptr), m_count(&(++(*rhs.m_count))) {}

        T* operator->() const {
            return m_ptr;
        }
        T& operator*() const {
            return *m_ptr;
        }

        T* get() const {
            return m_ptr;
        }

        SimpleSmartPtr<T>& operator=(SimpleSmartPtr<T> &rhs) {
            ++(*rhs.m_count);
            if(this->m_ptr && 0 == --(*(this->m_count))) {
                delete m_count;
                delete m_ptr;
            }

            this->m_ptr = rhs.m_ptr;
            this->m_count = rhs.m_count;
            
            return *this;
        }

        ~SimpleSmartPtr() {
            if(--(*m_count) == 0) {
                delete m_count;
                delete m_ptr;
            }
        }
    };
} // namespace Rocco

#endif // _SIMPLE_SMART_PTR_HPP