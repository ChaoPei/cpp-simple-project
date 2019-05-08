// 简历填写模版
class Resume {
protected:
	virtual void setPersonInfo() {}
	virtual void setEducation() {}
	virtual void setWorkExp() {}

public:
	void fillResume() {
		setPersonInfo();
		setEducation();
		setWorkExp();
	}
};


// A 填写的简历
class ResumeA : public Resume {
protected:
	void setPersonInfo() {
		cout << "A's person info" << endl;
	}

	void setEducation() {
		cout << "A's education" << endl;
	}

	void setWorkExp() {
		cout << "A's work exps" << endl;
	}
};


// B填写的简历
class ResumeA : public Resume {
protected:
	void setPersonInfo() {
		cout << "A's person info" << endl;
	}

	void setEducation() {
		cout << "A's education" << endl;
	}

	void setWorkExp() {
		cout << "A's work exps" << endl;
	}
};

