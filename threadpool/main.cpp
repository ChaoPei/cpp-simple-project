#include "threadpool.hpp"
#include <chrono>


// 普通函数
void hello() {
	std::cout << "common function!" << std::endl;
}

// 可调用对象
struct world {
	int operator()() {
		std::cout << "callable object!" << std::endl;
		return 0;
	}
};

int main() {
	try {
		thread_pool::ThreadPool pool(10);
		
		// 提交任务
		std::future<void> foo = pool.commit(hello);
		std::future<int> foo2 = pool.commit(world());
		// lambda函数
		std::future<std::string> foo3 = pool.commit(
				[]()->std::string { std::cout << "lambda expr!" << std::endl; return "lambda return";});
		
		// 停止提交
		pool.shutdown();
		
		// 获得结果
		foo.get();
		foo2.get();
		std::cout << foo3.get() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		pool.restart();
		pool.commit(hello).get();

		std::cout << "end..." << std::endl;
		return 0;
	}
	catch(std::exception &e) {
		std::cout << "Exception happened..." << e.what() << std::endl;
	}
}

