#include "ThreadPool.h"
#include "Thread_Safe_Stack.h"
#include "Semaphore.h"

Semaphore g_SlotFree(10);
Semaphore g_Available(0);
Semaphore g_Mutex(1);
std::vector<std::thread> Threads;
Thread_safe_Stack<int> ThreadSafeStack;

int Cleanup();

void Safe_Join(std::thread&& _thread)
{
	if (_thread.joinable())
		_thread.join();
}

void Print(int&& _value)
{
	std::cout << _value << std::endl;
}

void Print(std::string_view&& _string)
{
	std::cout << _string << std::endl;
}

void Producer()
{
	g_SlotFree.Acquire();
	g_Mutex.Acquire();

	// Do Stuff
	ThreadSafeStack.Push(1);
	Print("Pushed");

	g_Mutex.Release();
	g_Available.Release();
}

void Consumer()
{
	g_Available.Acquire();
	g_Mutex.Acquire();

	// Do Stuff
	ThreadSafeStack.Pop();
	Print("Poped");

	g_Mutex.Release();
	g_SlotFree.Release();
}

int main()
{
	Threads.emplace_back(std::thread{ Producer });
	Threads.emplace_back(std::thread{ Consumer });
	
	for (auto& item : Threads)
	{
		Safe_Join(std::move(item));
	}

	return Cleanup();
}

int Cleanup()
{
	//Threads.clear();

	return NULL;
}
