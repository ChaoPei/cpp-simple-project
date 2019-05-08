// 操作系统借口
class OS {
public:
	virtual void installOS() {}
};

// 不同操作系统
class Windows : public OS {
public:
	void installOS() {
		cout << "Install windows os" << endl;
	}
};

class Linux : public OS {
public:
	void installOS() {
		cout << "Install linux os" << endl;
	}
};


class Mac : public OS {
public:
	void installOS() {
		cout << "Install mac os" << endl;
	}
};


// 计算机
class Computer {
public:
	virtual void installOS(OS *os) {}
};

// 不同计算机
class Dell : public Computer {
public:
	void installOS(OS *os) {
		cout << "Dell computer : " << endl;
		os->installOS();
	}
};

class Apple : public Computer {
public:
	void installOS(OS *os) {
		cout << "Apple computer : " << endl;
		os->installOS();
	}
};

class Asus : public Computer {
public:
	void installOS(OS *os) {
		cout << "Asus computer : " << endl;
		os->installOS();
	}
};



