class Company {
public:
	Company(string name): m_name(name) {}
	virtual ~Company() {}
	virtual void add(Company *pCom) {}
	virtual void show(int depth) {}
protected:
	string m_name;
}

// 一级公司
class ConcreteCompany : public Company {

public:
	ConcreteCompany(string name) : Company(name) {}
	
	virtual ~ConcreteCompany() {}
	
	void add(Company *pCom) {
		m_listCompany.push_back(pCom);
	}

	// 递归遍历树
	void show(int depth) {
		for(int i = 0; i < depth; i++) {
			cout << "-";
		cout << m_name << endl;
		list<Company*>::iterator iter = m_listCompany.begin();
		for(; iter != m_listCompany.end(); ++iter)
			(*iter)->show(depth+2);
		}
	}

private:
	list<Company*> m_listCompany;
};

// 部门作为公司的一部分(对外是一个整体，一棵树)

// 财务部门
class FinanceDepartment : public Company {
public:
	FinanceDepartment(string name) : Company(name) {}
	virtual ~FinanceDepartment() {}

	// 叶子节点
	virtual void show(int depth) {
		for(int i = 0; i < depth; ++i) {
			cout << "-";
		cout << m_name << endl;
		}
	}
}


// 人事部门
class HRDepartment : public Company {
public:
	HRDepartment(string name) : Company(name) {}
	virtual ~HRDepartment() {}

	// 叶子节点
	virtual void show(int depth) {
		for(int i = 0; i < depth; ++i) {
			cout << "-";
		cout << m_name << endl;
		}
	}
}