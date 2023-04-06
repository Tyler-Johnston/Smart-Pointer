#pragma once
#include <iostream>
#include <utility>

namespace usu
{
    template <typename T>
    class shared_ptr
    {

      public:
        shared_ptr() = default;                // default constructor
        shared_ptr(T* data);                   // overloaded constructor
        shared_ptr(const shared_ptr& pointer); // copy constructor
        shared_ptr(shared_ptr&& pointer);      // move constructor
        ~shared_ptr();                         // destructor

        shared_ptr<T>& operator=(const shared_ptr& rhs); // Regular assignment operator
        shared_ptr<T>& operator=(shared_ptr&& rhs);      // Move assignment operator
        T* operator->();                                 // pointer operator
        T& operator*();                                  // dereference operator

        int use_count();
        T* get();

      private:
        T* m_data_ptr;
        unsigned int* m_count_ptr;
    };

    template <typename T>
    class shared_ptr<T[]>
    {

      public:
        shared_ptr() = default;                // default constructor
        shared_ptr(T data[], unsigned int N);  // overloaded constructor
        shared_ptr(const shared_ptr& pointer); // copy constructor
        shared_ptr(shared_ptr&& pointer);      // move constructor
        ~shared_ptr();                         // destructor

        shared_ptr<T[]>& operator=(const shared_ptr& rhs); // Regular assignment operator
        shared_ptr<T[]>& operator=(shared_ptr&& rhs);      // Move assignment operator
        T& operator[](unsigned int index);                 // Array-like access

        unsigned int size();

      private:
        T* m_data_ptr;
        unsigned int* m_count_ptr;
        unsigned int m_size;
    };

    // overloaded constructor
    template <typename T>
    shared_ptr<T>::shared_ptr(T* data) :
        m_data_ptr(data)
    {
        m_count_ptr = new unsigned int;
        *m_count_ptr = 1;
    }

    // copy constructor
    template <typename T>
    shared_ptr<T>::shared_ptr(const shared_ptr& pointer)
    {
        this->m_data_ptr = pointer.m_data_ptr;
        this->m_count_ptr = pointer.m_count_ptr;
        (*(this->m_count_ptr))++;
    }

    // regular assignment operator
    template <typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& rhs)
    {
        this->m_data_ptr = rhs.m_data_ptr;
        this->m_count_ptr = rhs.m_count_ptr;
        (*(this->m_count_ptr))++;
        return *this;
    }

    // move constructor
    template <typename T>
    shared_ptr<T>::shared_ptr(shared_ptr&& pointer)
    {
        this->m_data_ptr = pointer.m_data_ptr;
        this->m_count_ptr = pointer.m_count_ptr;
        pointer.m_data_ptr = nullptr;
        pointer.m_count_ptr = nullptr;
    }

    // move assignment operator
    template <typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& rhs)
    {
        if (this != &rhs)
        {
            std::swap(this->m_data_ptr, rhs.m_data_ptr);
            std::swap(this->m_count_ptr, rhs.m_count_ptr);
        }
        return *this;
    }

    // destructor
    template <typename T>
    shared_ptr<T>::~shared_ptr()
    {
        if (this->m_data_ptr != nullptr)
        {
            (*(this->m_count_ptr))--;
            if (*(this->m_count_ptr) == 0)
            {
                delete this->m_data_ptr;
                delete this->m_count_ptr;
            }
        }
    }

    template <typename T>
    T* shared_ptr<T>::operator->()
    {
        return this->m_data_ptr;
    }

    template <typename T>
    T& shared_ptr<T>::operator*()
    {
        return *this->m_data_ptr;
    }

    template <typename T>
    int shared_ptr<T>::use_count()
    {
        return *(this->m_count_ptr);
    }

    template <typename T>
    T* shared_ptr<T>::get()
    {
        return this->m_data_ptr;
    }

    template <typename T, typename... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        return shared_ptr<T>(new T(std::forward<Args>(args)...));
    }

    // overloaded constructor for array version.
    template <typename T>
    shared_ptr<T[]>::shared_ptr(T data[], unsigned int N) :
        m_data_ptr(data),
        m_size(N)
    {
        m_count_ptr = new unsigned int;
        *m_count_ptr = 1;
    }

    // copy constructor for array version
    template <typename T>
    shared_ptr<T[]>::shared_ptr(const shared_ptr& pointer)
    {
        this->m_data_ptr = pointer.m_data_ptr;
        this->m_count_ptr = pointer.m_count_ptr;
        (*(this->m_count_ptr))++;
    }

    // move constructor for array version
    template <typename T>
    shared_ptr<T[]>::shared_ptr(shared_ptr&& pointer)
    {
        this->m_data_ptr = pointer.m_data_ptr;
        this->m_count_ptr = pointer.m_count_ptr;
        pointer.m_data_ptr = nullptr;
        pointer.m_count_ptr = nullptr;
    }

    // size method for arrays
    template <typename T>
    unsigned int shared_ptr<T[]>::size()
    {
        return this->m_size;
    }

    // overload [] operator for array
    template <typename T>
    T& shared_ptr<T[]>::operator[](unsigned int index)
    {
        return this->m_data_ptr[index];
    }

    // destructor for array version
    template <typename T>
    shared_ptr<T[]>::~shared_ptr()
    {
        if (this->m_data_ptr != nullptr)
        {
            (*(this->m_count_ptr))--;
            if (*(this->m_count_ptr) == 0)
            {
                delete[] m_data_ptr;
                delete m_count_ptr;
            }
        }
    }

    template <typename T, unsigned int N>
    shared_ptr<T[]> make_shared_array()
    {
        return shared_ptr<T[]>(new T[N], N);
    }

} // namespace usu
