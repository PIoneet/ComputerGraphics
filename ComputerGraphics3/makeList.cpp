#include <iostream>
#include <array>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

// ===== 점 하나를 표현하는 구조체 =====
struct Point {
    int x{};
    int y{};
    int z{};
    bool used{ false };   
};



class PointList {
private:
    array<Point, 10> points{};
    int top{ -1 };   
   

public:
    PointList() {}


    const array<Point, 10>& getPoints() const { return points; }
    array<Point, 10>& getPoints() { return points; }

    int getTop() const { return top; }
 

    void setTop(int topVal) { top = topVal; }
 

    void setPoints( int index, int x, int y, int z )
    {
        points[index].x = x;
        points[index].y = y;
        points[index].z = z;
        points[index].used = true;
    }


    bool isEmpty()
    {
        bool empty{ true };

        for (int i = 0; i < 10; ++i) {
            
            if (points[i].used == true)
                empty = false;
        }
        return empty;
    }

    bool isFull()
    {
        bool full{ true };

        for (int i = 0; i < 10; ++i) {

            if (points[i].used == false)
                full = false;
        }

        return full;
    }




    friend ostream& operator<<(ostream& os, const PointList& list) {
        for (int i = 9; i >= 0; --i) {  
            os << i << " ";
            if (list.points[i].used) {
                os << list.points[i].x << " " << list.points[i].y << " " << list.points[i].z;
            }
            os << "\n";
        }
        return os;
    }
};




void pushTop(PointList& list, int x, int y, int z);      
void popTop(PointList& list);                             
void pushBottom(PointList& list, int x, int y, int z);   
void popBottom(PointList& list);                          
void printCount(const PointList& list);                   
void shiftDown(PointList& list);                            
void clearList(PointList& list);                            
void sortByDistance(const PointList& list, bool& toggleSort);
void findMinMaxDistance(const PointList& list);              

int main() {
    PointList list;
    bool toggleSort{ true };

    cout << list;

    do {
        cout << endl << "명령어 입력: ";
        char cmd;
        cin >> cmd;

        if (cmd == 'q') {
            cout << "프로그램 종료" << endl;
            break;
        }
        else if (cmd == '+') {
            int x, y, z;
            cin >> x >> y >> z;
            pushTop(list, x, y, z);
        }
        else if (cmd == '-') {
            popTop(list);
        }
        else if (cmd == 'e') {
            int x, y, z;
            cin >> x >> y >> z;
            pushBottom(list, x, y, z);
        }
        else if (cmd == 'd') {
            popBottom(list);
        }
        else if (cmd == 'a') {
            printCount(list);
        }
        else if (cmd == 'b') {
            shiftDown(list);
        }
        else if (cmd == 'c') {
            clearList(list);
        }
        else if (cmd == 'f') {
            sortByDistance(list, toggleSort);
        }
        else if (cmd == 'g') {
            findMinMaxDistance(list);
        }

        if(cmd != 'f')
            cout << list;   // f일떄만 제외하고 

    } while (true);

    return 0;
}



void pushTop(PointList& list, int x, int y, int z)
{
   
    int top = list.getTop();
    
    if ( list.isFull() ) {
        return;
    }
    
    top = (top + 1) % 10;

    list.setTop(top);

    list.setPoints(top, x, y, z);
}

void popTop(PointList& list)
{
 
    int top = list.getTop();

    if (list.isEmpty()) {
        cout << "List is Empty" << endl << endl;
        return;
    }

    list.getPoints()[top].used = false;

    if (top == 0 && !list.isEmpty()) {
        top = 9;
    }
    else {
        top -= 1;
    }

    list.setTop(top);
    

}

void pushBottom(PointList& list, int x, int y, int z)
{
 

    int top = list.getTop();
    


    if (list.isFull())  //꽉 찼다면 탈출하기
        return;


    if (list.getPoints()[0].used == false) {
        if (list.isEmpty()) {
            top += 1;
            list.setTop(top);
        }


        /*  이렇게 구현하면 문제가 생기는게 d를 여러번하고 e를 하면 bottom이 엉뚱한 위치에 존재하게 된다.
        else if (bottom != 0) {
            bottom -= 1;
            list.setBottom(bottom);
        }*/
            
           
        list.setPoints(0, x, y, z);
        
    }
    else{
        ++top;  // 데이터 추가하니까 top 증가
        list.setTop(top);

        auto firstIter = list.getPoints().begin();
        auto endIter = firstIter + top;

        rotate(firstIter, endIter, endIter + 1);

        list.setPoints(0, x, y, z);
        

    }

}

void popBottom(PointList& list)
{
    int bottom{};

    if (list.isEmpty()) {
        cout << "List is Empty" << endl << endl;
        return;
    }

    // 매번 루프로 가장 낮은 인덱스인 bottom을 찾는 로직
    for (int i = 9; i >= 0; --i) {
        if (list.getPoints()[i].used == true)
            bottom = i;
    }

    list.getPoints()[bottom].used = false;

    if (list.isEmpty()) {  // pop해서 비어지면 top도 다시 -1되야 한다.
        list.setTop(-1);
    }
        


}

void printCount(const PointList& list)
{
    // TODO: used == true인 칸 개수 세서 출력

    // STL count_if로 대체 가능하다.
    int pointCnt{};

    for (int i = 0; i < 10; ++i) {
        if (list.getPoints()[i].used == true)
            ++pointCnt;
    }

    cout << endl;
    cout << "Point count: " << pointCnt << endl;

}

void shiftDown(PointList& list)
{
    // TODO: 0->9, 1->0, ..., 9->8 로 전체 밀기 (순환)

    int top = list.getTop();

    if (list.isEmpty()) {
        cout << "List is Empty" << endl;
        return;
    }

    auto firstIter = list.getPoints().begin();
    auto endIter = list.getPoints().end();

    rotate(firstIter, firstIter + 1 , endIter);
    top = ((top - 1) + 10) % 10;
    list.setTop(top);


}

void clearList(PointList& list)
{
    for (int i = 0; i < 10; ++i) {

        list.getPoints()[i].used = false;
    }

    list.setTop(-1);
}


void sortByDistance(const PointList& list, bool& toggleSort)
{
    if (toggleSort) {
  

        vector< pair<Point, float> > dist;
        dist.reserve(100);

        for (int i = 0; i < 10; ++i) {

            if (list.getPoints()[i].used == true) {
                int x = list.getPoints()[i].x;
                int y = list.getPoints()[i].y;
                int z = list.getPoints()[i].z;
                dist.push_back( { list.getPoints()[i], sqrt(x * x + y * y + z * z) } );
            }
        }

        sort(dist.begin(), dist.end(), [](const pair<Point, float>& a, const pair<Point, float>& b) {
            return a.second < b.second;
            });

        for (int i = 0; i < dist.size(); ++i) {

            cout << i << " ";

            cout << '(' << dist[i].first.x << ' ' << dist[i].first.y << ' ' << dist[i].first.z << ')';

            cout << " " << dist[i].second << "\n";
        }

        toggleSort = false;
    }
    else {
        
        cout << list;

        toggleSort = true;
    }
}




void findMinMaxDistance(const PointList& list)
{
    vector<Point> pts;
    for (int i = 0; i < 10; ++i) {
        if (list.getPoints()[i].used) {
            pts.push_back(list.getPoints()[i]);
        }
    }

    int n = static_cast<int>(pts.size());
    if (n < 2) {
        cout << "점이 2개 미만이라 조합을 만들 수 없습니다." << endl;
        return;
    }

    float maxDist = 0, minDist = 0;
    Point maxA{}, maxB{}, minA{}, minB{};
    bool first = true;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int dx = pts[i].x - pts[j].x;
            int dy = pts[i].y - pts[j].y;
            int dz = pts[i].z - pts[j].z;
            float d = sqrt(static_cast<float>(dx * dx + dy * dy + dz * dz));

            cout << '(' << pts[i].x << ' ' << pts[i].y << ' ' << pts[i].z << ") - ("
                << pts[j].x << ' ' << pts[j].y << ' ' << pts[j].z << ") : 거리 "
                << d << '\n';

            if (first || d > maxDist) {
                maxDist = d;
                maxA = pts[i];
                maxB = pts[j];
            }
            if (first || d < minDist) {
                minDist = d;
                minA = pts[i];
                minB = pts[j];
            }
            first = false;
        }
    }

    cout << endl;
    cout << "가장 먼 두 점: (" << maxA.x << ' ' << maxA.y << ' ' << maxA.z
        << ") - (" << maxB.x << ' ' << maxB.y << ' ' << maxB.z
        << "), 거리: " << maxDist << endl;

    cout << "가장 가까운 두 점: (" << minA.x << ' ' << minA.y << ' ' << minA.z
        << ") - (" << minB.x << ' ' << minB.y << ' ' << minB.z
        << "), 거리: " << minDist << endl;
}