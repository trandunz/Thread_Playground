#include <thread>
#include <mutex>
#include <vector>
#include <stack>
#include <iostream>

using lock_guard = std::lock_guard<std::mutex>;

std::mutex GlobalStackMutex;

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

	inline void Pop() const
	{
		lock_guard lock(GlobalStackMutex);
		if (!m_Stack.empty())
		{
			m_Stack.pop();
		}
	}

	inline bool Empty()
	{
		lock_guard lock(GlobalStackMutex);
		return m_Stack.empty();
	}

	inline void Push(T&& _item)
	{
		lock_guard lock(GlobalStackMutex);
		m_Stack.push(_item);
	}

	inline void Clear()
	{
		lock_guard lock(GlobalStackMutex);
		while (!m_Stack.empty())
		{
			m_Stack.pop();
		}
	}

private:
	std::stack<T> m_Stack;
};

void Safe_Join(std::thread&& _thread)
{
	if (_thread.joinable())
		_thread.join();
}

void Print(int _value)
{
	std::lock_guard<std::mutex> lock(GlobalStackMutex);
	std::cout << _value << std::endl;
}

std::vector<std::thread> Threads;

int main()
{
	Threads.emplace_back(std::thread{ Print, 2 });

	for (auto& item : Threads)
	{
		Safe_Join(std::move(item));
	}

	return 0;
}
