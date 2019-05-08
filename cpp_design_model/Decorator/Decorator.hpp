// Public interface
class Phone {
public:
	Phone() {}
	virtual ~Phone() {}
	virtual void showDecorate() {}
};


// Class 
class iPhone : public Phone {
private:
	string m_name;
public:
	iPhone(string name): m_name(name) {}
	~iPhone() {}
	void showDecorate () {
		cout << "iPhone: " << m_name << " show decorate" << endl;
	}
};

class Android : public Phone {
private:
	string m_name;
public:
	Android(string name): m_name(name) {}
	~Android() {}
	void showDecorate() {
		cout << "Android: " << m_name << " show decorate" << endl;
	}
};


// Decorator interface
class PhoneDecorator : public Phone {
private:
	Phone *m_phone;		// The phone to be decorated
public:
	PhoneDecorator(Phone *phone): m_phone(phone) {}
	virtual void showDecorate() {
		m_phone->showDecorate();
	}
};

// Decorator class A
class DecoratorA : public PhoneDecorator {
public:
	DecoratorA(Phone *phone): PhoneDecorator(phone) {}
	void showDecorate() {
		PhoneDecorator::showDecorate();
		addDecorate();  	
	}
private:
	// Add decorate
	void addDecorate() {
		cout << "Add A decorate" << endl;  
	}
};


// Decorator class B
class DecoratorB : public PhoneDecorator {
public:
	DecoratorB(Phone *phone): PhoneDecorator(phone) {}
	void showDecorate() {
		PhoneDecorator::showDecorate();
		addDecorate();  	
	}
private:
	// Add decorate
	void addDecorate() {
		cout << "Add B decorate" << endl;  
	}
};
