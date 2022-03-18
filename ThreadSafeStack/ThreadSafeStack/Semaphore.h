#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
	Semaphore(int&& _count = 0) : m_Count(_count) {}

	inline void Acquire()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		while (m_Count == 0)
		{
			m_ConditionVar.wait(lock);
		}
		m_Count--;
	}

	inline void Release()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Count++;
		m_ConditionVar.notify_one();
	}
private:
	std::mutex m_Mutex;
	std::condition_variable m_ConditionVar;
	int m_Count;
};

