class War;

class State {
public:
	// 三个不同的阶段
	virtual void firstPhase() {}
	virtual void secondPhase() {}
	virtual void thirdPhase() {}
	// 当前状态
	virtual void currentState(War *war) {}
};


class ThirdState: public State {
public:
	// 战争对应当前状态的行为
	void thirdPhase(War *war) {
		cout << "War is end ! Peace coming!" << endl
	}

	// 设定当前状态对应的行为
	void currentState(War *war) {
		End(war);
	}

}


class SecondState: public State {
public:
	// 战争对应当前状态的行为
	void secondPhase(War *war) {
		if(war->getDays() < 300)
			cout << "War is going on ! Come on ! " << endl;
		else {
			war->setState(new ThirdPhase());
			war->getState();
		}
	}

	// 设定当前状态对应的行为
	void currentState(War *war) {
		secondPhase(war);
	}

}


class FirstState: public State {
public:
	// 战争对应当前状态的行为
	void firstPhase(War *war) {
		if(war->getDays() < 30) 
			cout << "War will start ! " << endl;
		else {
			war->setState(new SecondState());
			war->getState();
		}
	}

	// 设定当前状态对应的行为
	void currentState(War *war) {
		firstPhase(war);
	}
}


