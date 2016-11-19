//
// 环形线程安全数据缓冲区
//
//

// C++11 mutex
#include <mutex>

#include <iostream>

// linux sem
#include <semaphore.h>

// C head file
#include <cassert>
#include <cstring>


#ifndef _CIRCLE_DATA_BUFFER_HPP
#define _CIRCLE_DATA_BUFFER_HPP


class CircleDataBuffer {
	public:
		CircleDataBuffer(const size_t size);
		~CircleDataBuffer();


		const size_t getFreeSize() const;
		const size_t getUsedSize() const;
		const size_t getReadCursor() const;
		const size_t getWriteCursor() const;
		
		void writeBuff(const void* src, const size_t write_size);
		bool readBuff(void* dst, const size_t read_size);

		void reset();

		void setComplete();
		bool isComplete() const;

	protected:
		void init();
		
	private:
		unsigned char*    buffer_ptr;
		size_t            buffer_size;
		size_t            read_cursor;
		size_t            write_cursor;
		bool              complete_flag;
        sem_t             sem;      // 读写资源信号量
        static std::mutex mtx;		// 互斥量
};

#endif
