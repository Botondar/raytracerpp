#pragma once

#include <algorithm>
#include <utility>
#include <memory>

/** Heterogoneous collection to store objects of the same interface but different subtypes. 
    Non-copyable.
*/
template<class T>
class CHeteroStore
{
public:
    /** Constructor.
        Creates the an empty container.
    */
    CHeteroStore() :
        m_MaxSize(DEFAULT_SIZE),
        m_Size(0),
        m_Data(new std::shared_ptr<T>[DEFAULT_SIZE])
    {

    }

    CHeteroStore(const CHeteroStore& Other) = delete;
    CHeteroStore& operator=(const CHeteroStore& Other) = delete;

    ~CHeteroStore()
    {
        delete[] m_Data;
    }

    size_t Size() const
    {
        return m_Size;
    }

    /** Resizes the container, copying the objects. */
    void Resize(size_t NewSize)
    {
        m_MaxSize = NewSize;
        std::shared_ptr<T>* NewData = new std::shared_ptr<T>[NewSize];
        
        size_t CopySize = std::min(NewSize, m_Size);
        for(size_t i = 0; i < CopySize; i++)
        {
            NewData[i] = m_Data[i];
        }

        m_Size = CopySize;
        delete[] m_Data;
        m_Data = NewData;
    }

    /** Adds an element at the and of the container.
        Resizes the containter if needed.
    */
    void PushBack(std::shared_ptr<T> Elem)
    {
        if(m_Size == m_MaxSize)
        {
            Resize(m_MaxSize + DEFAULT_INCREMENT_SIZE);
        }
        m_Data[m_Size++] = Elem;
    }


    /** Index operator to access elements. Throws, if Index is out of range. */
    std::shared_ptr<T>& operator[](size_t Index)
    {
        if(Index >= m_Size)
        {
            throw std::out_of_range("Invalid index in CHeteroStore");
        }
        return m_Data[Index];
    }

    /** Index operator to access elements. Throws, if Index is out of range. */
    const std::shared_ptr<T>& operator[](size_t Index) const
    {
        if(Index >= m_Size)
        {
            throw std::out_of_range("Invalid index in CHeteroStore");
        }
        return m_Data[Index];
    }
private:
    const static size_t DEFAULT_SIZE = 256; /**< Starting size of the container. */
    const static size_t DEFAULT_INCREMENT_SIZE = 64; /**< The size with which to increase the container if it's full. */

    size_t m_MaxSize; /** Current size of the containter. */
    size_t m_Size; /** Number of elements in the container. */
    std::shared_ptr<T>* m_Data; /** Pointer to the elements of the containter. */
};