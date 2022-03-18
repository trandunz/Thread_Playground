#pragma once
#include <stack>
#include <mutex>
#include <semaphore>

using lock_guard = std::lock_guard<std::mutex>;
std::binary_semaphore m_GlobalStackMutex(1);
template <typename T>
class Thread_safe_Stack
{
public:
	inline Thread_safe_Stack()
	{

	}
	inline Thread_safe_Stack(const Thread_safe_Stack& _copy)
	{
		m_Stack = _copy.m_Stack;
	}

	inline ~Thread_safe_Stack()
	{
		Clear();
	}

	inline void Pop()
	{
		m_GlobalStackMutex.acquire();
		if (!m_Stack.empty())
		{
			m_Stack.pop();
		}
		m_GlobalStackMutex.release();
	}

	inline bool Empty()
	{
		m_GlobalStackMutex.acquire();
		return m_Stack.empty();
		m_GlobalStackMutex.release();
	}

	inline void Push(T&& _item)
	{
		m_GlobalStackMutex.acquire();
		m_Stack.push(_item);
		m_GlobalStackMutex.release();
	}

	inline void Clear()
	{
		m_GlobalStackMutex.acquire();
		while (!m_Stack.empty())
		{
			m_Stack.pop();
		}
		m_GlobalStackMutex.release();
	}
	std::stack<T> m_Stack;
private:

	
};

