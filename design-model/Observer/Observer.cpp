// Observer 
class Observer {
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void update() {}
};


// Custom: Be observed
class Blog {
public:
	Blog() {}
	virtual ~Blog() {}
	// add a observer
	void Attach(Observer *observer) {
		m_observers.push_back(observer);
	}
	// remove a observer
	void Remove(Observer *observer) {
		m_observers.remove(observer);
	}

	// Notify all observers
	void notify() {
		list<Observer*>::iterator it = m_observers.begin();
		for(; it != m_observers.end(); ++it) {
			(*it)->update();	// every observer receive notice and update	
		}
	}

	virtual void setStatus(string s) {
		m_status = s;
	}

	virtual string getStatus() {
		return m_status;
	}

private:
	list<Observer*> m_observers;		// save all observers
protected:
	string m_status;					// save custom status

}
