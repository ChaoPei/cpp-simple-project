/**
 * 消费者生产者队列
 * 练习C++11中条件变量的使用
 */

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <chrono>

int main() {

    // 产品队列
    std::queue<int> production_queue;

    // 互斥锁
    std::mutex mtx;

    // 条件变量
    std::condition_variable cond_var;

    // 唤醒标志，防止虚假唤醒
    bool notified = false;

    // 完成标志，消费者结束
    bool done = false;

    // 生产者线程
    std::thread producer(
        [&] () -> void {
            for(int i = 1; i < 10; ++i) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // 获取锁
                std::unique_lock<std::mutex> lock(mtx);

                // 生产
                std::cout << "producing: " << i << "\n";
                production_queue.push(i);

                // 唤醒消费者
                notified = true;
                cond_var.notify_one();
            }
        }
    );

    std::thread consumer(
        [&] () -> void {
            std::unique_lock<std::mutex> lock(mtx);
            while(!done) {
                while(!notified) {
                    cond_var.wait(lock);    // 释放lock，并进入线程等待，直到被唤醒，唤醒的时候会尝试重新获取锁
                }

                // 消费
                while(!production_queue.empty()) {
                    std::cout << "consuming: " << production_queue.front() << "\n";
                    production_queue.pop();
                }

                notified = false;
            }
        }
    );

    producer.join();
    consumer.join();

    return 0;
}