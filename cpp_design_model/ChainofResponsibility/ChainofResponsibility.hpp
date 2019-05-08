
/* 
 * 管理者接口
 * 每一级的管理者保存有下一级管理者的句柄。
 * 当任务到达时，首先判断自己能否处理，如果不能，则交给下一级
 */
class Manager {
protected:
	Manager *m_manager;		// 下一级的管理者
	String m_name;			// 本级管理者的名字
public:
	Manager(Manager *manager, string name) : m_manager(manager), m_name(name) {}
	virtual void dealWithRequest(string name, int num) {}
};

// 一级
class FirstManager : public Manager {
public:
	FirstManager(Manager *manager, string name) : Manager(manager), m_name(name) {}
	void dealWithRequest(string name, int num) {
		if(num < 500) {		// FirstManager可以处理
			cout << "FirstManager " << m_name << " deal with it" << endl;
		}
		else {				// 交给下一级SecondManager处理
			cout << "FirstManager " << m_name << " has no permission to deal" << endl;
			m_manager->dealWithRequest(name, num);
		}
	}
};


// 二级
class SecondManager : public Manager {
public:
	SecondManager(Manager *manager, string name) : m_manager(manager), m_name(name) {}
	void dealWithRequest(string name, int num) {
		if(num < 1000)
			cout << "SecondManager" << m_name << " deal with it" << endl;
		else {
			cout << "SecondManager " << m_name << " has no permission to deal" << endl;
			m_manager->dealWithRequest(name, num);		// 交给ThirdManager来处理
		}

	}
};

// 三级
class ThirdManager : public Manager {
public:
	ThirdManager(Manager* manager, string name) : m_manager(manager), m_name(name) {}
	void dealWithRequest(string name, int num) {
		cout << "ThirdManager " << m_name << " can deal everything." << endl;
	}

};