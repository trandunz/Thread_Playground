#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	m_iNumberOfThreads = std::thread::hardware_concurrency();
	m_bWorking = true;
}

ThreadPool::ThreadPool(unsigned _size)
{
	m_iNumberOfThreads = _size;
	m_bWorking = true;
}

ThreadPool::~ThreadPool()
{
	m_bWorking = false;
	for (int i = 0; i < m_iNumberOfThreads; i++)
	{
		m_workerThreads[i].join();
	}
	
	if (m_pWorkQueue)
	{
		delete m_pWorkQueue;
	}
	m_pWorkQueue = nullptr;
}

void ThreadPool::Initialize()
{
	if (!m_pWorkQueue)
	{
		m_pWorkQueue = new CWorkQueue<CTask>();
	}
}

void ThreadPool::Submit(CTask _iworkItem)
{
	m_pWorkQueue->Push(_iworkItem);
}

void ThreadPool::DoWork()
{
	while (m_bWorking)
	{
		m_pWorkQueue->Blocking_Pop(m_CurrentJob);
		std::cout << m_CurrentJob.GetValue() << std::endl;
		m_ItemsProcessed++;
	}
}

void ThreadPool::Start()
{
	for (unsigned int i = 0; i < m_iNumberOfThreads; i++)
	{
		m_workerThreads.push_back(std::thread(&ThreadPool::DoWork, this));
	}
}

void ThreadPool::Stop()
{
	m_bWorking = false;
}

std::atomic_int& ThreadPool::GetItemsProcessed()
{
	return m_ItemsProcessed;
}
