#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

struct Vertex {

	float x{};
	float y{};
	float z{};
};

struct TextCoorder {

	float u{};
	float v{};
};

struct TriangleFace {

	vector<string>& faceLine;
	int lineNumber{};

};


void readObjFile( ifstream& in, vector<Vertex>& vertices, vector<TextCoorder>& textCoorders ) {

	string temp;
	
	while (getline(in, temp)) {

		istringstream iss(temp);

		string firstWord;

		iss >> firstWord;

		
		if (firstWord == "#") {
			continue;
		}
		else if (firstWord == "v") {

			string word;
			vector<float> tempVertex;

			while (iss >> word) {
				float val = stoi(word);

				tempVertex.push_back(val);
			}

			vertices.push_back(Vertex{tempVertex[0], tempVertex[1], tempVertex[2]});
			

		}
		else if (firstWord == "vt") {

			string word;
			vector<float> tempCoord;

			while (iss >> word) {
				float val = stoi(word);

				tempCoord.push_back(val);
			}

			textCoorders.push_back(TextCoorder{ tempCoord[0], tempCoord[1] });

		}
		else {
			cout << "유효하지 않는 문자열입니다. " << endl;

		}
	


	}

	

}



int main()
{
	string fileName;
	vector<Vertex> vertices;
	vector<TextCoorder> textCoorders;

	vertices.reserve(100);
	textCoorders.reserve(100);

	cout << "파일 이름을 입력해주세요 : ";
	cin >> fileName;

	ifstream in{ fileName };

	if (!in.is_open()) {
		cout << " 파일을 열 수 없습니다. " << endl;
		return -1;
	}
	
	readObjFile( in, vertices, textCoorders);



}



// 전체적인 구조를 짜자면. 
/*
obj 파일을 읽어서 그 속에 들어있는 정점 데이터, 텍스쳐 좌표, 면 정보를 읽어오고 저장하는 것이 핵심이다. 

자료 구조를 이용해서 데이터를 자유롭게 담을 수 있고 f는 1/1 2/2 이런 형태로 작성되어있기에 이 문자열을
다시 잘라서 숫자로 표현해야 한다. 

다양한 예외처리가 존재한다. 
1. 정점 좌표끼리는 서로 달라야 한다. 
2. 정점.x 정점.y 정점.z는 서로 중복이 있어서는 안된다. 
3. 텍스쳐 좌표는 3개 전부 있어야 한다 하나의 좌표에서.
4. face의 정점을 나타내는 인덱스가 범위 바깥이면 그것도 예외처리 해야된다. 
5. 똑같이 정점도 3개가 아니어서 삼각형을 이루지 못한 경우도 예외처리해야됩니다. 











*/
