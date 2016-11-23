// Abstract interface
class ReplaceAlgorithm {
public:
	virtual void replace() = 0;
};


// Three implementation for interface
class LRU_Replace : public ReplaceAlgorithm {
public:
	void replace() {
		cout << " LRU Replace Algorithm" << endl;
	}
};

class FIFO_Replace : public ReplaceAlgorithm {
public:
	void replace() {
		cout << " FIFO Replace Algorithm" << endl;
	}
};

class Random_Replace : public ReplaceAlgorithm {
public:
	void replace() {
		cout << " Random Replace Algorithm" << endl;
	}
};
// inherit is a strategy model


// Custom: cache
class Cache {
private:
	ReplaceAlgorithm *m_ra;		// Must has a ReplaceAlgorithm object
public:
	Cache(ReplaceAlgorithm *ra) { 
		m_ra = ra;
	}
	~Cache() {
		delete m_ra;
	}
	void replace() {
		m_ra->replace();
	}
};