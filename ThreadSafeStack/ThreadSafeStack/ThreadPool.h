#pragma once
#include "CTask.h"
#include "CWorkQueue.h"
#include <vector>
#include <thread>
#include <iostream>
class ThreadPool
{
public:
	static ThreadPool& GetInstance()
	{
		if (!s_pThreadPool)
			s_pThreadPool = new ThreadPool();// Guaranteed to be destroyed.
		return *s_pThreadPool;
	}
	~ThreadPool();
	static void DestoryInstance()
	{
		if (s_pThreadPool)
		{
			delete s_pThreadPool;
		}
		s_pThreadPool = nullptr;
	}
	void Initialize();
	void Submit(CTask _iworkItem);
	void DoWork();
	void Start();
	void Stop();
	std::atomic_int& GetItemsProcessed();

private:
	// Make the ThreadPool a singleton
	ThreadPool();
	ThreadPool(unsigned _size);

	//Make the ThreadPool non-copyable.
	ThreadPool(const ThreadPool&); // non construction-copyable
	ThreadPool& operator=(const ThreadPool&) { return *this; }; // non copyable
private:
	inline static ThreadPool* s_pThreadPool;
	//The Threadpool has a workqueue
	CWorkQueue<CTask >* m_pWorkQueue;
	CTask m_CurrentJob;
	//The ThreadPool has a number of worker threads
	std::vector<std::thread> m_workerThreads;
	//A variable to hold the number of threads we want in the pool
	unsigned int m_iNumberOfThreads;
	std::atomic_bool m_bWorking{ false };
	std::atomic_int m_ItemsProcessed{ 0 };
};

