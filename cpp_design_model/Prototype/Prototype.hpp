// 父类
class Resume {
protected:
	string m_name;

public:
	Resume() {}
	virtual ~Resume() {}
	virtual Resume* clone() {
		return NULL;
	}

	virtual void set(string name) {}
	virtual void show() {}
};


class ResumeA : public Resume {
public:
	ResumeA(const string& name):m_name(name) {}

	// 实现拷贝构造函数
	ResumeA(const ResumeA &rhs) {
		m_name(rhs.m_name);
	}

	~ResumeA();

	ResumeA* clone() {
		return new ResumeA(*this);
	}

	void show() {
		cout << "ResumeA name: " << m_name << endl;
	}
};



class ResumeB : public Resume {
public:
	ResumeB(const string& name):m_name(name) {}

	// 实现拷贝构造函数
	ResumeB(const ResumeB &rhs) {
		m_name(rhs.m_name);
	}

	~ResumeB();

	ResumeB* clone() {
		return new ResumeB(*this);
	}

	void show() {
		cout << "ResumeB name: " << m_name << endl;
	}
};