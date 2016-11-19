#include "CircleDataBuffer.hpp"

std::mutex CircleDataBuffer::mtx;

// 构造函数
CircleDataBuffer::CircleDataBuffer(const size_t size):buffer_size(size) {
	init();
}


// 析构函数
CircleDataBuffer::~CircleDataBuffer() {
	// 释放内存
	free(buffer_ptr);
	buffer_ptr = NULL;
}



// 初始化成员变量
void CircleDataBuffer::init() {
	buffer_ptr = (unsigned char*)malloc(buffer_size);
	
	assert(buffer_ptr);
	
    // 初始化信号量
    sem_init(&sem, 0, 0);
	read_cursor = 0;
	write_cursor = 0;
	complete_flag = false;
}


// 重置读写位置
void CircleDataBuffer::reset() {
	mtx.lock();
	read_cursor = 0;
	write_cursor = 0;
    sem_init(&sem, 0, 0);
	mtx.unlock();
}


const size_t CircleDataBuffer::getReadCursor() const {
    return read_cursor;
}

const size_t CircleDataBuffer::getWriteCursor() const {
    return write_cursor;
}

// 获取未使用的内存
const size_t CircleDataBuffer::getFreeSize() const {
	size_t free_bytes_num;
	
	// 线程安全
	mtx.lock();
	// 读指针在前面, 说明数据存储形成了环形, 二者之间是空闲内存
	if(write_cursor < read_cursor) {
		free_bytes_num = read_cursor - write_cursor;
	}
	else if (write_cursor == read_cursor) {        // 游标在同一位置, 说明全部空闲
		free_bytes_num = buffer_size;
	}
	else {		// 写指针在前面
		free_bytes_num = buffer_size - (write_cursor - read_cursor);
	}

	mtx.unlock();
	return free_bytes_num;
}

		
// 获取已经使用的内存
const size_t CircleDataBuffer::getUsedSize() const {
	return buffer_size - getFreeSize();
}


// 设置完成标志
void CircleDataBuffer::setComplete() { 
    mtx.lock();
    complete_flag = true;
    mtx.unlock();
}


// 写入数据
void CircleDataBuffer::writeBuff(const void *src, const size_t write_size) {
    size_t write_bytes_num = write_size;
    unsigned char* src_ptr = (unsigned char*)src;

    // 判断是否有足够的空闲空间
    if(write_bytes_num > getFreeSize()) {
        std::cout << "Free size is not enough" << std::endl;
        return;
    }

    // 未完成状态
    assert(complete_flag == false);

    // 线程安全
    mtx.lock();

    // 写指针位置在读指针前面，读写指针之间有数据，其余部分(首尾)是空闲的
    if(write_cursor >= read_cursor) {
        
        // 计算尾部的可写入数据的空闲空间大小
        size_t tail_writable_size = buffer_size - write_cursor;
        
        // 如果尾部空间过大
        if(tail_writable_size > write_bytes_num) {
            tail_writable_size = write_bytes_num;
        }
        
        // 写入数据
        memcpy(buffer_ptr+write_cursor, src_ptr, tail_writable_size);
        
        // 更新指针和变量
        write_bytes_num -= tail_writable_size;      // 剩余要写入数据的大小
        src_ptr += tail_writable_size;         // 剩余数据的指针
        write_cursor += tail_writable_size;    // 缓冲区写指针

        // 如果尾部全部写入了
        if(write_cursor >= buffer_size) {
            write_cursor -= buffer_size;    // 写指针到达缓冲区末尾之后, 从头部开始写入，次你过程环形
        }

    }

    // 剩余数据从头部开始写入
    // 对于写指针在读指针后面的情况，等同
    if(write_bytes_num) {
        memcpy(buffer_ptr+write_cursor, src_ptr, write_bytes_num);
        write_cursor += write_bytes_num;
        assert(write_cursor <= read_cursor);        // 防止覆盖
    }

    // 设置数据写好的信号量
    sem_post(&sem);
    mtx.unlock();
}


// 读取数据
// 返回是否读取了完整个缓冲区的数据
bool CircleDataBuffer::readBuff(void* dst, const size_t read_size) {
    // 要读取的数据长度
    size_t read_bytes_num = read_size;
    // 已经读取了的数据长度
    size_t have_read = 0;
    // 是否读取完全部数据
    bool is_complete = false;

    while(read_bytes_num > 0 && complete_flag == false) {
        // 等待写入完毕的信号量 
        sem_wait(&sem);
        // 资源上锁
        mtx.lock();

        //  0                            buffer_size
        //  |---------------|======|------|
        //                 write  read
        // 读指针在写指针前面, 说明读写之间是空闲
        if(read_cursor > write_cursor) {
            // 先计算读指针和尾部之间的数据是否满足要读取的长度
            size_t tail_readable_size = buffer_size - read_cursor;
            
            // 如果尾部数据长度过长
            if(tail_readable_size > read_bytes_num) {
                tail_readable_size = read_bytes_num;
            }

            // 读取数据
            memcpy((unsigned char*)dst+have_read, buffer_ptr+read_cursor, tail_readable_size);

            // 更新指针的变量
            have_read += tail_readable_size;
            read_cursor += tail_readable_size;
            read_bytes_num -= tail_readable_size;

            // 读指针达到末尾
            if(read_cursor >= buffer_size)
                read_cursor -= buffer_size;
        }


        // 另一种情况以及读指针返回到头部的情况
        if(read_bytes_num && read_cursor < write_cursor) {
            // 头部能读取的数据大小
            size_t front_readable_size = write_cursor - read_cursor;
            if(front_readable_size > read_bytes_num) {
                front_readable_size = read_bytes_num;
            }

            memcpy((unsigned char*)dst+have_read, buffer_ptr+read_cursor, front_readable_size);

            have_read += front_readable_size;
            read_cursor += front_readable_size;
            read_bytes_num -= front_readable_size;

        }

        // 如果数据全部被读取了
        if(read_cursor == write_cursor) {
            is_complete = true;
            setComplete();      // 跳出while循环
        }
        else {  // 还有数据可以读，设置可操作信号量
            sem_post(&sem);
        }

        mtx.unlock();
    }

    return is_complete;
}


