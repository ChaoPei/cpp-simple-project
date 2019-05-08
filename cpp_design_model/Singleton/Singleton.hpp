// Class
class Singleton {
public:
	// Get instance
	static Singleton* getInstance() {
		if(singleton == NULL)
			singleton = new Singleton();
		return singleton;
	}
private:
	Singleton() {}  // Constructor must be private
	static Singleton* singleton;  	// The only instance
};

// 这个版本存在一个问题，如果是多个线程同时调用，可能会生成
// 多个实例，所以写单例需要加锁
