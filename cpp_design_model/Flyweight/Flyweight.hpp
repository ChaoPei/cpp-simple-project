// 棋子颜色
enum  PieceColor { WHITE, BLACK };


// 棋子位置
struct PiecePos {
	int x;
	int y;
	PiecePos(int a, int b):x(a), y(b) {}
};


// 棋子接口
class Piece {
protected:
	PieceColor m_color; // 颜色
public:
	Piece(PieceColor color):m_color(color) {}
	~Piece() {}
	virtual void draw() {}
};

// 黑棋
class BlackPiece : public Piece{
public:
	BlackPiece(PieceColor color=BLACK) : m_color(color) {}
	~BlackPiece() {}
	void draw() {
		cout << "Draw a black piece" << endl;
	}
};

// 白棋
class WhitePiece : public Piece{
public:
	WhitePiece(PieceColor color=WHITE) : m_color(color) {}
	~WhitePiece() {}
	void draw() {
		cout << "Draw a white piece" << endl;
	}
};



// 棋盘
class PieceBoard {
private:
	vector<PiecePos> m_vecPos;		// 保存位置
	Piece *m_blackPiece;			// 黑棋
	Piece *m_whitePiece;			// 白棋
	string m_blackName;				// 黑棋棋手
	string m_whiteName;				// 白棋棋手

public:
	PieceBoard(string black, string white): m_blackName(black), m_whiteName(white) {
		m_blackPiece = NULL;
		m_whitePiece = NULL;
	}

	~PieceBoard() {
		delete m_blackPiece;
		delete m_whitePiece;
	}

	void setPiece(PieceColor color, PiecePos pos) {
		// 黑棋落子
		if(color == BLACK) {
			// 理论上只需要保存黑棋(可以写成单例)
			if(m_blackPiece == NULL) 
				m_blackPiece = new BlackPiece(color);
			cout << m_blackName << "is locate a piece on : " << pos.x << "," << pos.y << endl;
			// 黑棋通用属性
			m_blackPiece->draw();
		}
		// 白棋落子
		else {
			if(m_whitePiece == NULL) 
				m_whitePiece = new WhitePiece(color);
			cout << m_whiteName << "is locate a piece on : " << pos.x << "," << pos.y << endl;
			m_whitePiece->draw();
		}
		m_vecPos.push_back(pos);
	}
}