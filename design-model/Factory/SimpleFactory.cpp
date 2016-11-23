// Class enum
enum CTYPE {COREA, COREB}


// Public core interface
class SingleCore {
public:
	virtual void show() = 0;
}

// Class one
class SingleCoreA : public SingleCore {
public:
	void show() {
		cout << "SingleCore A" << endl;
	}
}

// Class two
class SingleCoreB : public SingleCore {
public:
	void show() {
		cout << "SingleCore B" << endl;
	}
}


// The only one factory
class Factory {
public:
	// Create object depends on type
	SingleCore* createSingleCore(enum CTYPE ctype) {		
		if(ctype == COREA)
			return new SingleCoreA();
		else if(ctype == COREB) 
			return new SingleCoreB();
		else
			return NULL;
	}
}