// 双端队列
class Deque {
public:
	void push_back(int x) {
		cout << "Deque push_back: " << x << endl;
	}

	void push_front(int x) {
		cout << "Deque push_back: " << x << endl;
	}

	void pop_back() {
		cout << "Deque pop back" << endl;
	}

	void pop_front() {
		cout << "Deque pop front" << endl;
	}
};


// 顺序容器
class Sequence {
public:
	virtual void push(int x) = 0;
	virtual void pop() = 0;
};



// 栈
class Stack: public Sequence {
public:
	void push(int x) {
		deque.push_back();
	}
	void pop() {
		deque.pop_back();
	}

private:
	Deque deque;
};


// 队列
class Queue: public Sequence {
public:
	void push(int x) {
		deque.push_back();
	}
	void pop() {
		deque.pop_front();
	}

private:
	Deque deque;
};