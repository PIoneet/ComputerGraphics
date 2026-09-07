#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <windows.h>

using namespace std;

random_device rd4;
mt19937 gen4(rd4());

uniform_int_distribution<int> dis{ 1, 6 };

// ===== 보드 한 칸을 표현하는 구조체 =====
struct Cell {
    char letter{};        // 배치된 소문자 (또는 조커 '@' 같은 특수문자)
    bool revealed{ false }; // 현재 대문자로 확정되어 계속 보이는 상태인지

    bool matched{ false };
};


class Board {
private:
    vector<vector<Cell>> grid;
    int width{};    // 가로 (열 개수, a b c d ...)
    int height{};   // 세로 (행 개수, 1 2 3 ...)

public:
    Board() {}

    void setUp(int w, int h) {

        width = w;
        height = h;
        grid.assign(height, vector<Cell>(width));
        // 기존 grid의 메모리를 재사용해서 채워주므로 효율적이다. 

        int totalCell = width * height;
        int halfCell = totalCell / 2;

        // totalCell의 최대값은 36이겠네. 6x6

        vector<char> randLetter;
        randLetter.reserve(100);

        for (int i = 0; i < halfCell; ++i) {
            for (int j = 0; j < 2; ++j) {
                randLetter.push_back(char('a' + i));
            }
        }
        if (totalCell % 2 == 1) {
            // 홀수인 경우 조커 하나도 push
            randLetter.push_back('@');
        }

        shuffle(randLetter.begin(), randLetter.end(), gen4 );

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {

                grid[i][j].letter = randLetter[i * width + j];
            }
        }
        

        //일단 char('a' + i)의 형태로 a,b,c,d 등등을 Cell.letter = 이렇게 대입하는건 맞음. 
        // 그리고 어느 위치에다가 넣을지가 난수로 결정하는 부분이다. 
        // 난수가 막 10이 나왔다고 치면 i * width + row로 계산해서 넣어야 될려나? 

        // 아니야 그건 너무 복잡한 것 같고. for 루프로 grid를 순회하면서 내가 vector에
        // 저장해놓은 소문자들을 하나씩 대입하는 편이 더 좋겠다. 
        // shuffle 알고리즘을 사용할 수 있겠네. 


        // TODO: 아래 절차를 구현
        // 1. 전체 칸 수 = width * height
        // 2. 절반 개수만큼 서로 다른 소문자 준비 (a, b, c, ...)
        // 3. 홀수면 남는 한 칸은 조커로 배치, 짝수면 조커 없음
        // 4. 각 문자를 2개씩 무작위 위치에 배치 (mt19937 gen4 사용)
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    const vector<vector<Cell>>& getGrid() const { return grid; }
    vector<vector<Cell>>& getGrid() { return grid; }



    void print() const {
        cout << "  ";
        for (int c = 0; c < width; ++c) {
            cout << char('a' + c) << " ";
        }
        cout << endl;


        for (int r = 0; r < height; ++r) {
            cout << (r + 1) << " ";
            for (int c = 0; c < width; ++c) {
                if (grid[r][c].revealed) {
                    
                    char res = (char)toupper(grid[r][c].letter);

                    if(res == 'A' || res == 'A' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                    else if (res == 'B' || res == 'B' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                    else if (res == 'C' || res == 'C' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                    else if (res == 'D' || res == 'D' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                    else if (res == 'E' || res == 'E' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
                    else if (res == 'F' || res == 'F' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                    else if (res == 'G' || res == 'G' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    else if (res == 'H' || res == 'H' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    else if (res == 'I' || res == 'I' + 9)
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    
                    cout << res << " ";

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    cout << "* ";
                }
            }
            cout << endl;
        }
    }
};





void printBoard(Board& board, int w, int h);
void resetGame(Board& board, int w, int h);        
void showHint(Board& board);                      
pair<int, int> parseInput(const string& input);      
bool tryMatch(Board& board, pair<int, int> pos1, pair<int, int> pos2, int& score); // 매칭 판정

int main() {
    Board board;
    int w, h;

    cout << "가로 세로 크기 입력 (3~6): ";
    cin >> w >> h;
    
    if ( (w < 3 || w > 6) || (h < 3 || h > 6))
        return 1;
    
    board.setUp(w, h);
    board.print();

    int score = 0;
    int triesLeft = 10;   

    do {
        cout << endl << "명령을 입력해주세요 ( a1 c3, r=리셋, h=힌트, q=종료): ";
        string input;
        cin >> input;

        if (triesLeft == 0) {

            cout << "시도 가능 횟수가 0입니다. " << endl;
            break;
        }

        if (input == "q") {
            cout << "게임 종료" << endl;
            break;
        }
        else if (input == "r") {
            score = 0;
            triesLeft = 10;
            resetGame(board, w, h);
        }
        else if (input == "h") {
            showHint(board);
        }
        else {

            string input2;
            cin >> input2;

            triesLeft -= 1;

            pair<int, int> pos1 = parseInput(input);
            pair<int, int> pos2 = parseInput(input2);

            if (board.getGrid()[pos1.first][pos1.second].matched == true || board.getGrid()[pos2.first][pos2.second].matched == true)
                continue;

            tryMatch(board, pos1, pos2, score);
            board.print();
        }

    } while (true);

    cout << "최종 점수: " << score << endl;
    return 0;
}


void printBoard(Board& board, int w, int h)
{
    auto& boardGrid = board.getGrid();

    for (int i = 0; i < h; ++i) {

        for (int j = 0; j < w; ++j) {

            boardGrid[i][j].revealed = true;
        }
    }

    board.print();

    for (int i = 0; i < h; ++i) {

        for (int j = 0; j < w; ++j) {

            boardGrid[i][j].revealed = false;
        }
    }
}


void resetGame(Board& board, int w, int h)
{
    board.setUp(w, h);
    board.print();
}

void showHint(Board& board)
{
    // TODO: 전체 칸의 letter를 잠깐 보여줬다가(revealed=true로 전부 바꿔서 출력)
    //       다시 원래 상태(각 칸의 원래 revealed 값)로 복원해서 출력

    int w = board.getWidth();
    int h = board.getHeight();

    printBoard(board, w, h);  // 잠깐 revealed = true 해서 보여주고 다시 false로 바꿈.

    Sleep(1000);

    system("cls");

    cout << endl;
    board.print();
}


pair<int, int> parseInput(const string& input)
{
    // TODO: "a1" 같은 입력을 파싱
    // input[0] : 열 문자 (a, b, c...) -> col = input[0] - 'a'
    // input[1] 이후 : 행 번호 문자열 -> row = stoi(...) - 1
    
    
    int col = input[0] - 'a';
    
    int row = stoi(input.substr(1)) - 1;

    // substr(1)은 문자열에서 1번 인덱스 부터 끝까지 잘라내서 반환하는 함수입니다. 
    return { row, col };
}



bool tryMatch(Board& board, pair<int, int> pos1, pair<int, int> pos2, int& score)
{
    // TODO:
    // 1. 두 칸의 letter를 잠깐 보여줌
    // 2. 조커가 포함되어 있으면 -> 나머지 카드 자동으로 열림 처리
    // 3. 두 문자가 같으면 -> 대문자로 확정(revealed=true 유지), score 증가
    // 4. 다르면 -> 다시 가려짐(revealed=false), 문자 색상 다르게 표시했다가 복귀
    auto& pos1Cell = board.getGrid()[pos1.first][pos1.second];
    auto& pos2Cell = board.getGrid()[pos2.first][pos2.second];

    pos1Cell.revealed = true;
    pos2Cell.revealed = true;

    int w = board.getWidth();
    int h = board.getHeight();

    board.print();

    Sleep(1000);

    if (pos1Cell.letter == pos2Cell.letter) {  
        score += 5;
        pos1Cell.matched = true;
        pos2Cell.matched = true;

        system("cls");
    }
    else if (pos1Cell.letter == '@' || pos2Cell.letter == '@') {  // 조커일떄

        char temp;


        if (pos1Cell.letter != '@') {
            temp = pos1Cell.letter;
            
        }
        else {
            temp = pos2Cell.letter;
        }

        pos1Cell.matched = true;
        pos2Cell.matched = true;
            

        auto& tempCell = board.getGrid();

        for (int i = 0; i < h; ++i) {

            for (int j = 0; j < w; ++j) {

                if (tempCell[i][j].letter == temp) {
                    tempCell[i][j].revealed = true;
                    tempCell[i][j].matched = true;
                }
                    

            }
        }

        score += 10;

        system("cls");

    }
    else {

        pos1Cell.revealed = false;
        pos2Cell.revealed = false;

        system("cls");

    }

    return false;
}
