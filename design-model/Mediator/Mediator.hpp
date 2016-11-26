class Person;

// 中介者接口
class Mediator {
public:
	virtual void send(string message, Person *sender) {}
	// 只保留两个交互者
	virtual void setA(Person *A) {}
	virtual void setB(Person *B) {}
};

// 房屋中介
class HouseMediator : public Mediator {
private:
	Person *m_A; 	// 租户
	Person *m_B; 	// 房东

public:
	HouseMediator() : m_A(0), m_B(0) {}
	
	void setA(Person *A) {
		m_A = A;
	}
	
	void setB(Person *B) {
		m_B = B;
	}

	void send(string message, Person *sender) {
		if(sender == m_A)				// 租户发消息
			m_B->getMessage(message); 	//  房东收
		else
			m_A->getMessage(message);
	}
};



// 交互者接口
class Person {
protected:
	Mediator *m_mediator;  // 每个人都持有中介者的句柄
public:
	virtual void setMediator(Mediator *mediator) {}
	virtual void sendMessage(string message) {}
	virtual void getMessage(string message) {}
};


// 租户
class Renter : public Person {
public:
	void setMediator(Mediator *mediator) {
		m_mediator = mediator;
	}

	// 当交互者要发送信息时，调用中介者，利用中介者发送
	void sendMessage(string message) {
		m_mediator->send(message, this);
	}

	void getMessage(string message) {
		cout << "Renter receive: " << message << endl;
	}
};

// 房东
class Landlord : public Person {
public:
	void setMediator(Mediator *mediator) {
		m_mediator = mediator;
	}

	// 当交互者要发送信息时，调用中介者，利用中介者发送
	void sendMessage(string message) {
		m_mediator->send(message, this);
	}

	void getMessage(string message) {
		cout << "Landlord receive: " << message << endl;
	}
};
