#ifndef _THREADPOOL_HPP
#define _THREADPOOL_HPP

#include <iostream>
#include <functional>
#include <thread>
#include <condition_variable>		// 条件变量,线程间同步
#include <future>
#include <atomic>					// 原子操作
#include <vector>
#include <queue>

// 命名空间
namespace thread_pool {
	class ThreadPool;
}

class thread_pool::ThreadPool {
	// function接收原型为void的可调用对象
	using Task = std::function<void()>;

private:
	// 线程池
	std::vector<std::thread> pool;
	// 任务队列
	std::queue<Task> tasks;
	// 同步
	std::mutex m_task;
	std::condition_variable cv_task;
	std::atomic<bool> stop;		// 是否关闭提交

public:
	// 构造函数
	ThreadPool(size_t size=4):stop(false) {
		size = size < 1 ? 1 : size;
		for(size_t i = 0; i < size; ++i){
			pool.emplace_back(&ThreadPool::schedual, this);
		}
	}

	// 析构函数
	~ThreadPool() {
		for(std::thread&  t : pool){
			t.detach();   
		}
	}

	// 停止任务提交
	void shutdown() {
		this->stop.store(true);
	}

	// 重启任务提交
	void restart() {
		this->stop.store(false);
	}

	// 提交一个任务
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))> {
		if(stop.load()){
			throw std::runtime_error("task executor has closed commit");
		}
		
		using ResType = decltype(f(args...));
		auto task = std::make_shared<std::packaged_task<ResType()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

		// 添加任务到队列
		{
			std::lock_guard<std::mutex> lock(m_task); // 锁定临界区
			tasks.emplace(
					[task](){ (*task)(); }
					);
		}
		
		// 唤醒线程执行
		cv_task.notify_all();
		
		std::future<ResType> future = task->get_future();
		return future;
	}

private:
	// 获取一个待执行的task
	Task get_one_task() {
		std::unique_lock<std::mutex> lock(m_task);
		// wait 直到有task
		cv_task.wait(lock, 
				[this](){ return !tasks.empty(); });
		// 取一个task
		Task task(std::move(tasks.front()));
		tasks.pop();
		return task;
	}

	// 任务调度
	void schedual() {
		// 每个线程循环获取task来执行，实现线程的复用
		while(true) {
			if(Task task = get_one_task()) {
				task();
			}
			else {
				// return;
			}
		}
	}

};

#endif // _THREADPOOL_HPP

