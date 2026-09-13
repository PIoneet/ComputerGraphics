#include <iostream>
#include <windows.h>
#include <vector>
#include <map>
#include <functional>
#include <string>

using namespace std;

/*
1. struct Board 구조체 만들기 
- 1.1 30 x 30 크기를 가진다. 각 칸은 .를 찍어 표현한다. 
- 1.2 사용자는 좌표 1개와 가로 길이, 세로 길이를 입력받는다.
- 1.3 위 데이터를 저장 가능한 struct를 만든다. 
2. 사용자는 총 2개의 도형을 입력해서 표현한다. 
- 2.1 도형을 이동, 확대/축소 , 각 좌표축을 이용한 확대 및 축소가 가능하다. 
- 2.2 도형이 서로 겹치게 되면 그것도 검사해서 다른 색으로 표현해야 한다. 
3. x/X는 오른쪽/ 왼쪽으로 이동한다. 보드 밖으로 나가면 그 부분만 반대편에 나타나야 한다. 
- 3.1 y/Y는 아래쪽/ 위쪽으로 이동한다. 
- 3.2 s/S는 x축 y축 동시에 한 칸 축소/ 확대한다.
- 3.3 i/I는 x축 한칸 확대/ 축소한다. 
- 3.4 j/J는 y축 한칸 확대/ 측소한다. 
- 3.5 a/A는 x축 한칸 확대, y축 한칸 축소/ x축 한칸 축소, y축 한칸 확대한다. 
- 3.6 b는 사각형의 면적 출력 (가로 x 세로)한다. 
- 3.7 도형 2개는 다른 명령어를 적용해서 변환해야 한다. 
4. 입력 가능한 명령어들 
- 4.1 c: 보드칸을 x축 y축 한칸씩 늘릴 수 있다. 최대 10칸 가능. 
- 4.2 d: 보드칸을 x축, y축 모두 한칸씩 줄인다. 최대 20칸 줄일 수 있다. 도형을 보드칸 내에 있도록 shift
- 4.3 r: 모든 걸 리셋하고 다시 좌표값을 입력 받을 수 있어야 한다. 
- 4.4 q: 프로그램 종료.

*/
struct Shape {

	int x;
	int y;
	int w;
	int h;

	void moveRowRight() { ++x; }
	void moveRowLeft() { --x; }

	void moveColDown() { ++y; }
	void moveColUp() { --y; }
	
	void scaleMinus() { --w, --h; }
	void scalePlus() { ++w, ++h; }
	
	void scaleRowPlus() { ++w; }
	void scaleRowMinus() { --w; }
	
	void scaleColPlus() { ++h; }
	void scaleColMinus() { --h; }
	
	void plusRowMinusCol() { ++w, --h; }
	void plusColMinusRow() { --w, ++h; }

	int dimension() { return w* h; }

};




class Board {

private:
	vector<vector<char>> grid;
	// 보드칸은 어쩌피 30x30 칸의 그리드 이기는함.
	int width{30};
	int height{30};
	int expanBoard{ 10 };
	int cutBoard{ 20 };

public: 
	
	Board() {
		grid.resize(height);
		for (int i = 0; i < height; ++i) {
			grid[i].resize(50, '.');
		}
			
	}

	void setUp() {  // 다시 30x30의 grid로 만든다. 

		width = 30;
		height = 30;

		expanBoard = 10;
		cutBoard = 20;
	}

	// s1은 '0' 출력하고, s2는 'X' 출력하고 겹치는 부분은 다른 색깔로 # 출력하면됨.
	void print(const Shape& s1, const Shape& s2) {

		for (int i = 0; i < height; ++i) {

			for (int j = 0; j < width; ++j) {

				if (s1.x <= j && j <= s1.x + s1.w - 1 &&
					s1.y <= i && i <= s1.y + s1.h - 1 &&
					s2.x <= j && j <= s2.x + s2.w - 1 &&
					s2.y <= i && i <= s2.y + s2.h - 1) {

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
					grid[i][j] = '#';
					cout << grid[i][j] << " ";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else if (s1.x <= j && j <= s1.x + s1.w - 1 &&
					s1.y <= i && i <= s1.y + s1.h - 1) {

					grid[i][j] = '0';
					cout << grid[i][j] << " ";
				}
				else if (s2.x <= j && j <= s2.x + s2.w - 1 &&
					s2.y <= i && i <= s2.y + s2.h - 1) {

					grid[i][j] = 'X';
					cout << grid[i][j] << " ";

				}
				else {
					grid[i][j] = '.';
					cout << grid[i][j] << " ";
				}

			}
			cout << endl;
		}

	}

};


void inputShape(Board& board, Shape& s1, Shape& s2)
{
	int x, y, w, h;
	int x2, y2, w2, h2;

	cout << "1번 도형을 입력해주세요: ";

	cin >> x >> y >> w >> h;

	cout << "2번 도형을 입력해주세요: ";

	cin >> x2 >> y2 >> w2 >> h2;

	s1.x = x;
	s1.y = y;
	s1.w = w;
	s1.h = h;

	s2.x = x2;
	s2.y = y2;
	s2.w = w2;
	s2.h = h2;

	board.setUp();
}

Shape s1;
Shape s2;

map<string, function<void()>> s1Funcs{
	{"x", [&] {s1.moveRowRight(); }},
	{"X", [&] {s1.moveRowLeft(); }},
	{"y", [&] {s1.moveColDown(); }},
	{"Y", [&] {s1.moveColUp(); }},
	{"s", [&] {s1.scaleMinus(); }},
	{"S", [&] {s1.scalePlus(); }},
	{"i", [&] {s1.scaleRowPlus(); }},
	{"I", [&] {s1.scaleRowMinus(); }},
	{"j", [&] {s1.scaleColPlus(); }},
	{"J", [&] {s1.scaleColMinus(); }},
	{"a", [&] {s1.plusRowMinusCol(); }},
	{"A", [&] {s1.plusColMinusRow(); }},

};


map<string, function<void()>> s2Funcs{
	{"x2", [&] {s1.moveRowRight(); }},
	{"X2", [&] {s1.moveRowLeft(); }},
	{"y2", [&] {s1.moveColDown(); }},
	{"Y2", [&] {s1.moveColUp(); }},
	{"s2", [&] {s1.scaleMinus(); }},
	{"S2", [&] {s1.scalePlus(); }},
	{"i2", [&] {s1.scaleRowPlus(); }},
	{"I2", [&] {s1.scaleRowMinus(); }},
	{"j2", [&] {s1.scaleColPlus(); }},
	{"J2", [&] {s1.scaleColMinus(); }},
	{"a2", [&] {s1.plusRowMinusCol(); }},
	{"A2", [&] {s1.plusColMinusRow(); }},

};



int main()
{
	
	Board board;

	inputShape(board, s1, s2);

	do {
		string input;
	
		
		cout << "명령을 입력해주세요: ";
		cin >> input;

		if (input == "q") {
			cout << "프로그램을 종료합니다. " << endl;
			return 0;
		}
		else if (input == "c") {

		}
		else if (input == "d") {

		}
		else if (input == "r") {
			cout << endl;
			inputShape(board, s1, s2);
		}
		else {
			// 그냥 일반적으로 변환 명령어 적을떄 모든 경우의 수

			if (s1Funcs.count(input)) {
				s1Funcs[input]();  // 람다를 호출하라는 거인듯
			}
			else if (s2Funcs.count(input)) {
				s2Funcs[input](); 
			}

		}

		board.print(s1, s2);

	} while (true);

	

}



// 람다는 이름 없는 클래스다.
// auto f1 = [](int a){return a;}
// auto f2 = [](int a){return a;}

// 둘은 엄연히 서로 다른 클래스다. 

// 이렇게 다른 람다들을 하나의 컨테이너에 묶을 수 있는것이 function<반환형타입(들어가는 인자)> 컨테이너다. 

// auto f3 = [&]{s1.moveX}; 이거는 moveX함수의 코드를 복붙한게 아니라 람다가 호출되면 그때 moveX를 실제로 호출하는 방식이다. 
// 캡쳐 &은 람다 외부의 변수를 다 참조로 읽을 수 있게 하겠다는 것을 의미함. s1만 읽고 싶으면 s1&을 할 수도 있음. 
