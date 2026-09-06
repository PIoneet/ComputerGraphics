#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <cctype>
#include <windows.h>
using namespace std;

random_device rd4;
mt19937 gen4(rd4());

// ===== 보드 한 칸을 표현하는 구조체 =====
struct Cell {
    char letter{};          // 배치된 소문자 (조커는 '@')
    bool revealed{ false }; // 지금 화면에 글자가 보이고 있는지 (임시 표시 포함)
    bool matched{ false };  // 완전히 맞춰져서 영구적으로 열린 상태인지
};

class Board {
private:
    vector<vector<Cell>> grid;
    int width{};    // 가로 (열 개수, a b c d ...)
    int height{};   // 세로 (행 개수, 1 2 3 ...)

public:
    Board() {}

    void setup(int w, int h) {
        width = w;
        height = h;
        grid.assign(height, vector<Cell>(width));

        int totalCells = width * height;
        int pairCount = totalCells / 2;        // 필요한 서로 다른 문자 개수
        bool hasJoker = (totalCells % 2 != 0);

        vector<char> letters;
        for (int i = 0; i < pairCount; ++i) {
            letters.push_back(static_cast<char>('a' + i));
            letters.push_back(static_cast<char>('a' + i));
        }
        if (hasJoker) {
            letters.push_back('@'); // 조커: 어느 카드와도 매치됨
        }

        shuffle(letters.begin(), letters.end(), gen4); // 무작위 배치

        int idx = 0;
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                grid[r][c].letter = letters[idx++];
                grid[r][c].revealed = false;
                grid[r][c].matched = false;
            }
        }
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    const vector<vector<Cell>>& getGrid() const { return grid; }
    vector<vector<Cell>>& getGrid() { return grid; }

    bool allMatched() const {
        for (const auto& row : grid)
            for (const auto& cell : row)
                if (!cell.matched) return false;
        return true;
    }

    // ---- 보드 출력 (a b c d.. 열 헤더 + 1 2 3.. 행 번호 포함) ----
    void print() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        cout << "  ";
        for (int c = 0; c < width; ++c) {
            cout << char('a' + c) << " ";
        }
        cout << endl;

        for (int r = 0; r < height; ++r) {
            cout << (r + 1) << " ";
            for (int c = 0; c < width; ++c) {
                const Cell& cell = grid[r][c];
                if (cell.revealed) {
                    if (cell.matched)
                        SetConsoleTextAttribute(hConsole, 10); // 초록: 확정 매치
                    else
                        SetConsoleTextAttribute(hConsole, 14); // 노랑: 임시 표시
                    cout << static_cast<char>(toupper(cell.letter));
                    SetConsoleTextAttribute(hConsole, 7);      // 기본색 복귀
                    cout << " ";
                }
                else {
                    cout << "* ";
                }
            }
            cout << endl;
        }
    }
};

// ===== 함수 선언 =====
void resetGame(Board& board, int w, int h);
void showHint(Board& board);
pair<int, int> parseInput(const string& input);
bool isValidPos(const Board& board, pair<int, int> pos);
bool tryMatch(Board& board, pair<int, int> pos1, pair<int, int> pos2, int& score);

int main() {
    Board board;
    int w, h;

    do {
        cout << "가로 세로 크기 입력 (3~6): ";
        cin >> w >> h;
        if (w < 3 || w > 6 || h < 3 || h > 6)
            cout << "범위를 벗어났습니다. 3~6 사이로 다시 입력하세요." << endl;
    } while (w < 3 || w > 6 || h < 3 || h > 6);

    board.setup(w, h);
    board.print();

    int score = 0;
    int triesLeft = 10;   // 횟수/배점 기준: 매치 시도 10번, 성공 시 +10점

    do {
        cout << endl << "남은 시도: " << triesLeft << ", 점수: " << score << endl;
        cout << "명령을 입력해주세요 (예: a1 c3, r=리셋, h=힌트, q=종료): ";
        string input;
        cin >> input;

        if (input == "q") {
            cout << "게임 종료" << endl;
            break;
        }
        else if (input == "r") {
            resetGame(board, w, h);
            score = 0;
            triesLeft = 10;
        }
        else if (input == "h") {
            showHint(board);
        }
        else {
            string input2;
            cin >> input2;

            pair<int, int> pos1 = parseInput(input);
            pair<int, int> pos2 = parseInput(input2);

            if (!isValidPos(board, pos1) || !isValidPos(board, pos2) || pos1 == pos2) {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
                continue;
            }

            tryMatch(board, pos1, pos2, score);
            board.print();
            --triesLeft;

            if (board.allMatched()) {
                cout << endl << "모든 카드를 맞췄습니다!" << endl;
                break;
            }
            if (triesLeft <= 0) {
                cout << endl << "횟수를 모두 사용했습니다. 게임을 종료합니다." << endl;
                break;
            }
        }

    } while (true);

    cout << "최종 점수: " << score << endl;
    return 0;
}

// ===== 함수 구현 =====

void resetGame(Board& board, int w, int h)
{
    board.setup(w, h);
    board.print();
}

void showHint(Board& board)
{
    auto& grid = board.getGrid();
    int height = board.getHeight();
    int width = board.getWidth();

    vector<vector<bool>> original(height, vector<bool>(width));
    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            original[r][c] = grid[r][c].revealed;

    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            grid[r][c].revealed = true;

    board.print();
    Sleep(1500); // 1.5초 동안 전체 공개

    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            grid[r][c].revealed = original[r][c];

    board.print();
}

pair<int, int> parseInput(const string& input)
{
    // "a1" -> col = 'a'-'a' = 0, row = 1-1 = 0
    if (input.size() < 2) return { -1, -1 };
    int col = input[0] - 'a';
    int row = stoi(input.substr(1)) - 1;
    return { row, col };
}

bool isValidPos(const Board& board, pair<int, int> pos)
{
    return pos.first >= 0 && pos.first < board.getHeight()
        && pos.second >= 0 && pos.second < board.getWidth();
}

bool tryMatch(Board& board, pair<int, int> pos1, pair<int, int> pos2, int& score)
{
    auto& grid = board.getGrid();
    Cell& cell1 = grid[pos1.first][pos1.second];
    Cell& cell2 = grid[pos2.first][pos2.second];

    if (cell1.matched || cell2.matched) {
        cout << "이미 맞춰진 칸입니다." << endl;
        return false;
    }

    // 1. 두 칸을 잠깐 보여줌
    cell1.revealed = true;
    cell2.revealed = true;
    board.print();

    bool isJoker1 = (cell1.letter == '@');
    bool isJoker2 = (cell2.letter == '@');

    if (isJoker1 || isJoker2) {
        // 2. 조커가 포함되면 -> 두 칸 모두 자동으로 열림(매치 확정)
        cout << "조커! 두 칸 모두 열립니다." << endl;
        cell1.matched = true;
        cell2.matched = true;
        cell1.revealed = true;
        cell2.revealed = true;
        score += 10;
        return true;
    }

    if (cell1.letter == cell2.letter) {
        // 3. 문자가 같으면 -> 대문자로 확정
        cout << "일치! 맞췄습니다." << endl;
        cell1.matched = true;
        cell2.matched = true;
        score += 10;
        return true;
    }
    else {
        // 4. 다르면 -> 다시 가려짐
        cout << "불일치." << endl;
        cell1.revealed = false;
        cell2.revealed = false;
        return false;
    }
}