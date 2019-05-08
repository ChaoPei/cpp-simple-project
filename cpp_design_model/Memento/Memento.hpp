// 需要保存的信息
class Memento {
public:
	// 要保存的信息
	int m_vitality;		
	int m_attack;
	int m_defense;
public:
	Memento(int v, int a, int d) : m_vitality(v), m_attack(a), m_defense(d) {}
	Memento& operator=(const Memento &m) {
		m_vitality = m.m_vitality;
		m_attack = m.m_attack;
		m_defense = m.m_defense;
		return *this;
	}
};


// 游戏角色
class GameRole {
private:
	int m_vitality;
	int m_attack;
	int m_defense;

public:
	GameRole(): m_vitality(100), m_defense(100), m_attack(100) {}
	
	// 保存进度
	Memento saveProcess() {
		Memento memento(m_vitality, m_attack, m_defense);
		return memento;
	}

	// 加载进度
	void loadMemento(const Memento &memento) {
		m_vitality = memento.m_vitality;
		m_attack = memento.m_attack;
		m_defense = memento.m_defense;
	}

	void show() {
		cout << "m_vitality: " << m_vitality << endl;
		cout << "m_attack: " << m_attack << endl;
		cout << "m_defense: " << m_defense << endl;
	}

	void attack() {
		m_vitality -= 10;
		m_attack -= 10;
		m_defense -= 10;
	}
};


// 保存进度的库
class CareTake {
public:
	CareTake() {}
	void save(Memento memento) {
		m_vec.push_back(memento);
	}
	Memento load(int state) {
		return m_vec[state];
	}
private:
	std::vector<Memento> m_vec;
}
