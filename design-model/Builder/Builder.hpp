// 以创建一个人为例


// 构建人的过程
class Builder {
public:
    virtual void buildHead() {}  
    virtual void buildBody() {}  
    virtual void buildLeftArm(){}  
    virtual void buildRightArm() {}  
    virtual void buildLeftLeg() {}  
    virtual void buildRightLeg() {}  
};


// 在builder的过程中，每个过程执行不同的方法则可以创建不同的人

// 男人
class MaleBuilder : public Builder {
public:
	void buildHead() {
		cout << "Male head" << endl;
	}  

    void buildBody() {
    	cout << "Male body" << endl;
    }  

    void buildLeftArm(){
    	cout << "Male left arm" << endl;
    }  

    void buildRightArm() {
    	cout << "Male right arm" << endl;
    }  

    void buildLeftLeg() {
    	cout << "Male left leg" << endl;
    }  

    void buildRightLeg() {
    	cout << "Male right leg" << endl;
    } 
};


// 女人
class MaleBuilder : public Builder {
public:
	void buildHead() {
		cout << "Female head" << endl;
	}  

    void buildBody() {
    	cout << "Female body" << endl;
    }  

    void buildLeftArm(){
    	cout << "Female left arm" << endl;
    }  

    void buildRightArm() {
    	cout << "Female right arm" << endl;
    }  

    void buildLeftLeg() {
    	cout << "Female left leg" << endl;
    }  

    void buildRightLeg() {
    	cout << "Female right leg" << endl;
    } 
};


// 指挥构建
class Director {
private:
	Builder *m_pBuilder;

public:
	Director(Builder *b) : m_pBuilder(b) {}

	void create() {
		m_pBuilder->buildHead();
		m_pBuilder->buildBody();
		m_pBuilder->buildRightLeg();
		m_pBuilder->buildLeftLeg();
		m_pBuilder->buildRightArm();
		m_pBuilder->buildLeftArm();
	}
}