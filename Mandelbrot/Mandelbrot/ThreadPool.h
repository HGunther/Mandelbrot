#ifndef ThreadPool_H
#define ThreadPool_H

#include <mutex>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <atomic>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(unsigned int num_threads, long num_jobs);
	~ThreadPool();
	void post(std::function<void(void)>);
	void postVector(std::vector<std::function<void(void)>>);

private:
	std::vector<std::thread> m_pool;
	std::queue<std::function<void(void)>> m_tasks;
	//std::atomic<bool> m_continue;
	std::mutex m_mutex;
	std::condition_variable m_taskAdded;
	//void stop();
	std::atomic<long> m_num_jobs;
};

#endif