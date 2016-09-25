#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int num_threads, long num_jobs) :m_pool(), m_tasks()
{
	m_num_jobs = num_jobs;

	auto threadJob = [&]()
	{
		std::function<void(void)> f;
		while (m_num_jobs-- > 0) //there are more jobs to do
		{
			{//lock the queue
				std::unique_lock<std::mutex> lck(m_mutex);
				//wait for a job in the queue
				while (m_tasks.empty())
				{
					m_taskAdded.wait(lck);
				}
				f = m_tasks.front();
				m_tasks.pop();
			} //unlock the queue
			f();
		} 
		return;
	};

	//std::thread::hardware_concurrency
	for (int i = 0; i < num_threads; ++i)
	{
		m_pool.emplace_back(threadJob);
	}
}

ThreadPool::~ThreadPool()
{
	for (auto& t : m_pool)
		t.join();
}

//void ThreadPool::stop()
//{
//	m_continue = false;
//}

void ThreadPool::post(std::function<void(void)> f)
{
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		m_tasks.push(f);
	}
	m_taskAdded.notify_one();
}

void ThreadPool::postVector(std::vector<std::function<void(void)>> v)
{
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		for (auto f : v)
		{
			m_tasks.push(f);
		}
		//Lock will be unlocked in deconstructor
	}
	m_taskAdded.notify_all();
}