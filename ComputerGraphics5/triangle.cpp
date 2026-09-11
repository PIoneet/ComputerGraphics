#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <random>
#include <algorithm> 


using namespace std;


random_device rd4;
mt19937 gen4(rd4());

uniform_int_distribution<int> dis{ 1, 6 };


struct Shape {
    int x{};      
    int y{};      
    int w{};      
    int h{};      
};

class Board {
private:
    int width{};
    int height{};
    int growCount{};   
    int shrinkCount{};  

public:
    Board() {}

    void setUp(int w, int h) {
        width = w;
        height = h;
        growCount = 0;
        shrinkCount = 0;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }



    bool grow() {
        if (growCount < 10) {
            ++width;
            ++height;

            ++growCount;
            return true;
        }

        return false;
    }

    
    bool shrink(Shape& s1, Shape& s2) {

        if (shrinkCount < 20) {
            --width;
            --height;

            ++shrinkCount;

            // [수정] 줄어든 보드 기준으로 두 도형 위치를 안쪽으로 당김
            if (s1.x + s1.w > width)  s1.x = max(0, width - s1.w);
            if (s1.y + s1.h > height) s1.y = max(0, height - s1.h);
            if (s2.x + s2.w > width)  s2.x = max(0, width - s2.w);
            if (s2.y + s2.h > height) s2.y = max(0, height - s2.h);

            return true;
        }

        return false;
    }


    void print(const Shape& s1, const Shape& s2) const {

        for (int i = 0; i < height; ++i) {

            for (int j = 0; j < width; ++j) {
            

                if ((j >= s1.x && j <= s1.x + s1.w - 1) && (i >= s1.y && i <= s1.y + s1.h - 1)
                    && (j >= s2.x && j <= s2.x + s2.w - 1) && (i >= s2.y && i <= s2.y + s2.h - 1)) {

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                    cout << '#' << " ";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else if ((j >= s1.x && j <= s1.x + s1.w - 1) && (i >= s1.y && i <= s1.y + s1.h - 1)) {
                    cout << '0' << " ";
                }
                else if ((j >= s2.x && j <= s2.x + s2.w - 1) && (i >= s2.y && i <= s2.y + s2.h - 1)) {
                    cout << 'x' << " ";
                }
                else {
                    cout << '.' << " ";
                }

            }
            cout << endl;
        }

    }
};


// ===== 함수 프로토타입 =====
void moveShapeX(Shape& s, const Board& board, int dir); // dir: +1 우측, -1 좌측, wrap-around
void moveShapeY(Shape& s, const Board& board, int dir); // dir: +1 아래, -1 위, wrap-around
void scaleShape(Shape& s, int dw, int dh);              // dw, dh: 각 축 증감량 (+1/-1/0 조합으로 s/i/j/a 명령 전부 처리 가능)
int  computeArea(const Shape& s);
bool checkOverlap(const Shape& s1, const Shape& s2, int x, int y); // 특정 칸(x,y)이 두 도형 모두에 포함되는지
void readShapeInput(Shape& s1, Shape& s2); // 두 도형 좌표를 사용자로부터 입력받아 채움 (초기 입력, r 리셋에서 재사용)

int main() {
    int boardW = 30, boardH = 30;
    Board board;
    board.setUp(boardW, boardH);

    Shape shape1{}, shape2{};
    readShapeInput(shape1, shape2);

    board.print(shape1, shape2);

    do {
        cout << endl << "명령을 입력해주세요: ";
        string cmd;
        cin >> cmd;

        if (cmd == "q") {
            cout << "프로그램 종료" << endl;
            break;
        }
        else if (cmd == "r") {
            board.setUp(boardW, boardH);
            readShapeInput(shape1, shape2);
        }
        else if (cmd == "c") {
            board.grow();
        }
        else if (cmd == "d") {
            board.shrink(shape1, shape2); // [수정] shrink가 도형 참조를 받도록 시그니처 변경
        }
        else if (cmd == "b") {
            cout << "도형1 면적: " << shape1.w << "x" << shape1.h << "=" << computeArea(shape1) << endl;
            cout << "도형2 면적: " << shape2.w << "x" << shape2.h << "=" << computeArea(shape2) << endl;
        }
        else {
            // 도형1 명령어
            if (cmd == "x") moveShapeX(shape1, board, +1);
            else if (cmd == "X") moveShapeX(shape1, board, -1);
            else if (cmd == "y") moveShapeY(shape1, board, +1);
            else if (cmd == "Y") moveShapeY(shape1, board, -1);
            else if (cmd == "s") scaleShape(shape1, -1, -1);
            else if (cmd == "S") scaleShape(shape1, +1, +1);
            else if (cmd == "i") scaleShape(shape1, +1, 0);
            else if (cmd == "I") scaleShape(shape1, -1, 0);
            else if (cmd == "j") scaleShape(shape1, 0, +1);
            else if (cmd == "J") scaleShape(shape1, 0, -1);
            else if (cmd == "a") scaleShape(shape1, +1, -1);
            else if (cmd == "A") scaleShape(shape1, -1, +1);
            // 도형2 명령어 (도형1과 겹치지 않게 뒤에 "2"를 붙인 두 글자 명령으로 구분)
            else if (cmd == "x2") moveShapeX(shape2, board, +1);
            else if (cmd == "X2") moveShapeX(shape2, board, -1);
            else if (cmd == "y2") moveShapeY(shape2, board, +1);
            else if (cmd == "Y2") moveShapeY(shape2, board, -1);
            else if (cmd == "s2") scaleShape(shape2, -1, -1);
            else if (cmd == "S2") scaleShape(shape2, +1, +1);
            else if (cmd == "i2") scaleShape(shape2, +1, 0);
            else if (cmd == "I2") scaleShape(shape2, -1, 0);
            else if (cmd == "j2") scaleShape(shape2, 0, +1);
            else if (cmd == "J2") scaleShape(shape2, 0, -1);
            else if (cmd == "a2") scaleShape(shape2, +1, -1);
            else if (cmd == "A2") scaleShape(shape2, -1, +1);
        }

        board.print(shape1, shape2);

    } while (true);

    return 0;
}


void moveShapeX(Shape& s, const Board& board, int dir)
{
    int width = board.getWidth();
    s.x = ((s.x + dir) % width + width) % width;
}



void moveShapeY(Shape& s, const Board& board, int dir)
{
    int height = board.getHeight();
    s.y = ((s.y + dir) % height + height) % height;
}



void scaleShape(Shape& s, int dw, int dh)
{
    const int MIN_SIZE = 1;

    if (s.w + dw >= MIN_SIZE) {
        s.w += dw;
    }

    if (s.h + dh >= MIN_SIZE) {
        s.h += dh;
    }
}



int computeArea(const Shape& s)
{
    return s.w * s.h;
}



bool checkOverlap(const Shape& s1, const Shape& s2, int x, int y)
{
    bool inS1 = (x >= s1.x && x <= s1.x + s1.w - 1) && (y >= s1.y && y <= s1.y + s1.h - 1);
    bool inS2 = (x >= s2.x && x <= s2.x + s2.w - 1) && (y >= s2.y && y <= s2.y + s2.h - 1);
    return inS1 && inS2;
}



void readShapeInput(Shape& s1, Shape& s2)
{
    int x, y, w, h;
    int x2, y2, w2, h2;

    cout << "input coord value1: ";
    cin >> x >> y >> w >> h;
    cout << "input coord value2: ";
    cin >> x2 >> y2 >> w2 >> h2;

    s1 = Shape{ x, y, w, h };
    s2 = Shape{ x2, y2, w2, h2 };
}
