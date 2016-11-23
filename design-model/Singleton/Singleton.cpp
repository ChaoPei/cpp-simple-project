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


