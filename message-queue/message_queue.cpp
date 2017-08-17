#include <queue>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <sstream>
#include <condition_variable>

std::mutex print_mtx;       // 打印过程不是线程安全的，必须加锁

// 队列中任务(消息)
typedef struct task_tag
{
    int data;
    task_tag( int i ) : data(i) { }
} Task, *PTask;

class MessageQueue
{
public:
    MessageQueue(){}
    ~MessageQueue()
    {
        if ( !m_queue.empty() )
        {
            PTask pRtn = m_queue.front();
            delete pRtn;
        }

    }

    // 生产者持队列锁，生成消息任务
    void PushTask( PTask pTask )
    {
        std::unique_lock<std::mutex> lock( m_queueMutex );
        m_queue.push( pTask );
        m_cond.notify_one();
    }

    // 消费者wait条件变量的唤醒，持队列锁，然后消费消息
    PTask PopTask()
    {
        PTask pRtn = NULL;
        std::unique_lock<std::mutex> lock( m_queueMutex );
        while ( m_queue.empty() )
        {
            m_cond.wait_for( lock, std::chrono::seconds(1) );
        }

        if ( !m_queue.empty() )
        {
            pRtn = m_queue.front();
            if ( pRtn->data != 0 ) {
                m_queue.pop();
            }
        }

        return pRtn;
    }

private:
    std::mutex m_queueMutex;
    std::condition_variable m_cond;
    std::queue<PTask> m_queue;
};

void thread_fun(MessageQueue *arguments, int thread_id)
{
    while ( true )
    {
        PTask data = arguments->PopTask();

        if (data != NULL)
        {   

            {
                std::lock_guard<std::mutex> locker( print_mtx );
                printf( "Thread is: %d, pop: %d\n", std::this_thread::get_id(), data->data);
            }
            
            if ( 0 == data->data ) //Thread end.
                break;
            else
                delete data;
        }
    }

    return;
}

int main( int argc, char *argv[] )
{
    MessageQueue cq;

#define THREAD_NUM 3
    std::thread threads[THREAD_NUM];

    for ( int i=0; i<THREAD_NUM; ++i )
        threads[i] = std::thread(thread_fun, &cq, i);

    int i = 100;
    while( i > 0 )
    {
        Task *pTask = new Task( --i );
        {
            std::lock_guard<std::mutex> locker( print_mtx );
            printf("push %d\n", pTask->data);
        }
        cq.PushTask( pTask );
    }

    for ( int i=0; i<THREAD_NUM; ++i)
        threads[i].join();

    system( "pause" );
    return 0;
}