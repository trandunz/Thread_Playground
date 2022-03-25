#pragma once
#include <queue>
#include <mutex>
template <typename T>
class CWorkQueue
{
public:
	CWorkQueue();
	~CWorkQueue();
	void Push(const T& _item);
	bool NonBlocking_Pop(T& _workItem);
	void Blocking_Pop(T& _workItem);
	T& Front();
	bool Empty() const;

private:
	std::queue<T> m_WorkQueue;
	mutable std::mutex m_WorkMutex;
	std::condition_variable m_WorkCondition;
};

template<typename T>
inline CWorkQueue<T>::CWorkQueue()
{

}

template<typename T>
inline CWorkQueue<T>::~CWorkQueue()
{
	std::queue<T> empty;
	std::swap(m_WorkQueue, empty);
}

template<typename T>
inline void CWorkQueue<T>::Push(const T& _item)
{
	std::lock_guard<std::mutex> _lock(m_WorkMutex);
	m_WorkQueue.push(_item);
	m_WorkCondition.notify_one();
}

template<typename T>
inline bool CWorkQueue<T>::NonBlocking_Pop(T& _workItem)
{
	std::lock_guard<std::mutex> _lock(m_WorkMutex);
	if (m_WorkQueue.empty())
		return false;

	_workItem = std::move(m_WorkQueue.front());
	m_WorkQueue.pop();
	return true;
}

template<typename T>
inline void CWorkQueue<T>::Blocking_Pop(T& _workItem)
{
	std::unique_lock<std::mutex> _lock(m_WorkMutex);
	m_WorkCondition.wait(_lock);
	_workItem = std::move(m_WorkQueue.front());
	m_WorkQueue.pop();
}

template<typename T>
inline T& CWorkQueue<T>::Front()
{
	std::lock_guard<std::mutex> _lock(m_WorkMutex);
	return m_WorkQueue.front();
}

template<typename T>
inline bool CWorkQueue<T>::Empty() const
{
	std::lock_guard<std::mutex> _lock(m_WorkMutex);
	return m_WorkQueue.empty();
}
