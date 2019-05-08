
// Public core interface
class SingleCore {
public:
	virtual void show() = 0;
};

// Class one
class SingleCoreA : public SingleCore {
public:
	void show() {
		cout << "SingleCore A" << endl;
	}
};


// Class two
class SingleCoreB : public SingleCore {
public:
	void show() {
		cout << "SingleCore B" << endl;
	}
};

// Factory interface 
class Factory {
public:
	virtual SingleCore* createSingleCore() = 0;
};

// SingleCoreA's factory
class FactoryA : public Factory {
public:
	SingleCore* createSingleCore() {
		return new SingleCoreA();
	}
};

// SingleCoreB's factory
class FactoryB : public Factory {
public:
	SingleCore* createSingleCore() {
		return new SingleCoreB();
	}
}