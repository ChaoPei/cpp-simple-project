// Single core
class SingleCore {
public:
	virtual void show() = 0;
};

class SingleCoreA : public SingleCore {
public:
	void show() {
		cout << "SingleCore A" << endl;
	}
};

class SingleCoreB: public SingleCore {
public:
	void show() {
		cout << "SingleCore B" << endl;
	}
};

// Multi core
class MultiCore {
public:
	virtual void show() = 0;
};

class MultiCoreA : public Multi {
public:
	void show() {
		cout << "MultiCore A" << endl;
	}
};

class MultiCoreB : public Multi {
public:
	void show() {
		cout << "MultiCore B" << endl;
	}
};


// Factory interface, can produce single and multi core
class CoreFactory {
public:
	virtual SingleCore* createSingleCore() = 0;
	virtual MultiCore* createMultiCore() = 0;
};

// Factory A
class FactoryA: public CoreFactory {
public:
	SingleCore* createSingleCore() {
		return new SingleCoreA();
	}

	MultiCore* createMultiCore() {
		return new MultiCoreA();
	}
};

// Factory B
class FactoryB: public CoreFactory {
public:
	SingleCore* createSingleCore() {
		return new SingleCoreB();
	}

	MultiCore* createMultiCore() {
		return new MultiCoreB();
	}
};