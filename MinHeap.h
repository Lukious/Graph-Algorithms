#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <utility>
#include <vector>
#include <string>

template<typename TKey, typename TValue>
class MinHeap
{
private:
    // array for the elements which should be heap-sorted
    std::vector<std::pair<TKey, TValue>> m_vec;

public:
    MinHeap() 
	{
		// start index from 1 to make calculation easier
		m_vec.push_back(std::make_pair(NULL, NULL));
	}

    /// <summary>
    /// insert key-value pair
    /// </summary>
    ///
    /// <param name="key">
    /// the key that is used for sorting
    /// </param>
    ///
    /// <param name="value">
    /// the value that is managed in this heap
    /// </param>
	void Push(TKey key, TValue value)
	{
		// Dummy data at the end for calculation
		m_vec.push_back(std::make_pair(NULL, NULL));

		// Pushing data to find the right place to enter
		int child = m_vec.size() - 1;
		while (m_vec[child / 2].second > value && child != 1)
		{
			m_vec[child] = m_vec[child / 2];
			child /= 2;
		}

		// Insert data where found
		m_vec[child] = std::make_pair(key, value);
	}
    /// <summary>
    /// remove the minimum element
    /// </summary>
	void Pop()
	{
		if (IsEmpty()) return;
		if (m_vec.size() == 2) { m_vec.pop_back(); return; }

		// Save the value at the end of the heap temporarily.
		std::pair<TKey, TValue> temp = m_vec[m_vec.size() - 1];

		// Delete Data
		m_vec.pop_back();

		//Resort
		int parent = 1;
		unsigned int child = 2;

		while (child <= m_vec.size() - 1)
		{
			// Not the last node (=if you have more than one child, =if you have a brother node)
			if (child < m_vec.size() - 1)
			{
				//the smaller of the child's nodes.
				child = m_vec[child] < m_vec[child + 1] ? child : child + 1;
			}

			// If smaller than the child (find a seat), exit
			if (temp.second <= m_vec[child].second)
			{
				break;
			}

			// Data pushed
			m_vec[parent] = m_vec[child];

			parent = child;
			child *= 2;
		}

		m_vec[parent] = temp;
	}
    /// <summary>
    /// get the minimum element
    /// </summary>
    ///
    /// <returns>
    /// the minimum element
    /// </returns>
	std::pair<TKey, TValue> Top()
	{
		//Returns the value at the top
		return m_vec[1];
	}
    /// <summary>
    /// get the key-value pair which the value is the same as the target
    /// </summary>
    ///
    /// <returns>
    /// the key-value pair which the value is the same as the target
    /// </returns>
	std::pair<TKey, TValue> Get(TValue target)
	{
		for (int i = 1; i < m_vec.size(); i++)
		{
			if (m_vec[i].second == target)
			{
				return m_vec[i];
			}
		}
	}
    /// <summary>
    /// check whether this heap is empty or not
    /// </summary>
    ///
    /// <returns>
    /// true if this heap is empty
    /// </returns>
	bool IsEmpty()
	{
		// Because size starts from 1, 2 or higher means there is data
		return m_vec.size() <= 1;
	}
    /// <summary>
    /// change the key of the node which the value is the target.<para/>
    /// In general, the newKey should be smaller than the old key.<para/>
    /// </summary>
    ///
    /// <parma name="target">
    /// the target to change the key
    /// </param>
    ///
    /// <param name="newKey">
    /// new key for the target
    /// </param>
	void DecKey(TValue target, TKey newKey)
	{

	}

private:
    /// <summary>
    /// heap-sort, heapify.<para/>
    /// this function can be called recursively
    /// </summary>
	void Heapify(int index)
	{

	}
};

#endif