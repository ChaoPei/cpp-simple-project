class State;

// 战争中保存有当前的状态
// 战争的行为根据状态的不同而改变
class War {
private:
	State *m_state;		// 当前状态
	int m_days;

public:
	War(State *state) : m_state(state), m_days(0) {}
	
	~War() { 
		delete m_state;
	}

	int getDays() {
		return m_days;
	}

	void setDays(int days) {
		m_days = days;
	}

	void setState(State *state) {
		delete m_state;
		m_state = state;
	}

	// 获得当前状态对应的行为
	void getState() {
		m_state->currentState(this);
	}
};