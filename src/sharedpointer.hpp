#pragma once

#include <stdexcept>

/** Shared pointer class which uses reference counting to keep track of its objects. */
template<class T>
class CSharedPointer
{
public:
    CSharedPointer() :
        m_Pointer(nullptr),
        m_RefCount(nullptr)
    {
    }

    CSharedPointer(T* Pointer) :
        m_Pointer(Pointer),
        m_RefCount(nullptr)
    {
        if(m_Pointer)
        {
            m_RefCount = new size_t(1);
        }
    }

    CSharedPointer(const CSharedPointer& Other) :
        m_Pointer(Other.m_Pointer),
        m_RefCount(Other.m_RefCount)
    {
        if(m_Pointer)
        {
            *m_RefCount += 1;
        }
    }

    ~CSharedPointer()
    {
        if(m_Pointer)
        {
            *m_RefCount -= 1;
            if(*m_RefCount == 0)
            {
                delete m_Pointer;
                delete m_RefCount;
            }
        }
    }

    CSharedPointer& operator=(const CSharedPointer& Other)
    {
        if(this != &Other)
        {
            if(m_Pointer)
            {
                *m_RefCount -= 1;
                if(*m_RefCount == 0)
                {
                    delete m_Pointer;
                    delete m_RefCount;
                }
            }
            m_Pointer = Other.m_Pointer;
            m_RefCount = Other.m_RefCount;
            if(m_Pointer)
            {
                *m_RefCount += 1;
            }
        }
        return *this;
    }

    operator T*()
    {
        return m_Pointer;
    }

    operator const T*() const
    {
        return m_Pointer;
    }

    T& operator*()
    {
        return *m_Pointer;
    }

    const T& operator*() const
    {
        return *m_Pointer;
    }

    T* operator->()
    {
        return m_Pointer;
    }

    const T* operator->() const
    {
        return m_Pointer;
    }

    T* Pointer()
    {
        return m_Pointer;
    }

    const T* Pointer() const
    {
        return m_Pointer;
    }

    bool IsNull() const
    {
        return (m_Pointer == nullptr);
    }

private:
    T* m_Pointer; /**< Raw pointer to the actual memory location. */
    size_t* m_RefCount; /**< Raw pointer to the reference count. */
};
