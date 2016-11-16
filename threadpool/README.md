## ThreadPool

A simple C++11 Thread Pool implementation

Basic usage:

```
// create threa pool with n worker threads
thread_pool::ThreadPool pool(n)

// commit tasks
std::function<type> foo = pool.commit(task)

// get resutl
foo.get()
```

Using main example:

```
g++ -W -std=c++11 -pthread main.cpp -o run
```
