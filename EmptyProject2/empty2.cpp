#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <array>


/*1. Vertex 구조체 구현
* 2. TextCoorder 구현
* 3. 삼각형 face 구조체 구현
* 4. 파일 읽어서 저장하는 부분 구현
*  - 4.1 파일 못 읽었을떄 예외처리 구현
* 
* 5. 각 getline해서 줄의 앞 string을 보고 if else문 처리하기
* - 5.1 "f"일뗴 1/1 2/2 3/3을 처리하기
* - 5.2 iss >> firstWord 공백일떄 continue 처리했음. 
* 
* 6. 삼각형 면의 개수만큼 삼각형의 좌표값과 텍스쳐 좌표값을 출력해야 한다. 
* - 6.1 이 삼각형이 몇번 face 삼각형인지도 미리 저장해야됨. 
* - 6.2 하나의 면에서 꼭짓점이 3개가 아닌경우.  
* - 6.3 삼각형의 꼭짓점이 중복된 경우 
* - 6.4 인덱스 범위 외의 인덱스로 삼각형, 텍스쳐 구성될떄
* - 6.5 허용되지 않는 문자값 포함되있을떄
* - 6.6 f의 각 인덱스들을 확인해서 삼각형이 이루어지지 않을떄 에러메시지 출력하기.
* - 6.7 텍스쳐 좌표 인덱스가 없는 경우도 처리해야 된다. 
* - 6.8 출력할떄 face 1 (2,3,4) 처럼 몇번째 면의 삼각형이고 정점 좌표 인덱스 출력해야 한다. (1이 시작)
* 

*/


// 주석 작성의 핵심은 내가 어느 길로 나아가야 하는지 알려주는 척도일 뿐이다. 
// 너무 새새하게 작성할 필요는 없다. 적당히 큼지막하게 적고. 막상 구현해보면서 복잡한 것들은
//다시 주석으로 돌아서 더 새새하게 과정을 잘라서 작성해주면 된다. 
//그렇게 하나하나 달성해가는 재미를 느끼면 된다. 

// 뭔가 딱 과제 보면 머릿속에서 설계도가 자동으로 그려져서 한번 슥 보고 막힘없이 코드 쓰는 걸 동경하지도 말고
// 그걸 자기 자신에게 강요도 하지 마라. 지금은 그렇지 않은게 당연한거고 배워가는 과정임을 명시하자. 


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

	array<int, 3> faceVertex;
	array<int, 3> faceCoord;
	// 이 faceLine에 들어있는 각 줄의 인덱스 정보를 가져와서 출력해야된다.
	// 그럼 vecotr<TriangleFace> 타입의 자료구조가 있어야 될 것 같은데?
	int lineFace{};

};


int readObjFile(ifstream& in)
{
	string fileName;

	cout << "파일 이름을 입력해주세요 : ";
	cin >> fileName;

	in.open(fileName);

	if (!in.is_open()) {
		return 0;
	}

	return 1;
}


void parseFace(string word, vector<Vertex>& vertices, vector<TextCoorder>& textCoorders, vector<TriangleFace>& lineTriangle, 
	array<int, 3>& vIdx, array<int, 3>& vtIdx, int idx, int& lineFaceNum)
{
	// 지금 word에는 1/2 뭐 이런 예시값이 들어있음.
	// 지금은 1, 2가 string값이라서 stoi()해줘서 저장하면 될듯. 
	// 현재 상황은 v,vt 이렇게 정점 텍스쳐가 다 저장되있는 상황임. 그걸 활용하는 단계이고. 


	size_t point = word.find("/");

	vIdx[idx] = stoi( (point == string::npos) ? word : word.substr(0, point) );
	string temp = (point == string::npos) ? "" : word.substr( point + 1 ) ;

	vtIdx[idx] = -1;  // 예외 체크하고 ""이라면 -1저장. 이후 -1이면 텍스쳐 좌표 출력 안하는 방향으로 가면 될듯. 
	if (!temp.empty())
		vtIdx[idx] = stoi(temp);

		
	if (idx == 2) {
		++lineFaceNum;
		lineTriangle.push_back( TriangleFace{ vIdx, vtIdx, lineFaceNum } );
	}

}


void dealWithFile( ifstream& in, vector<Vertex>& vertices, vector<TextCoorder>& textCoorders, vector<TriangleFace>& lineTriangle ) 
{

	string line;
	int lineFaceNum{};

	while (getline(in, line )) {  //매줄 처리하기 없으면 탈출

		istringstream iss(line);

		string firstWord;

		array<int, 3> vIdx;
		array<int, 3> vtIdx;
		int idx{};
		

		if (!(iss >> firstWord) ) {
			continue;
		}
		
		
		if (firstWord == "#") {
			continue;
		}
		else if (firstWord == "v") {

			string word;
			vector<float> tempVertex;
			tempVertex.reserve(5);

			while (iss >> word) {
				float val = stof(word);

				tempVertex.push_back(val);
			}

			vertices.push_back(Vertex{tempVertex[0], tempVertex[1], tempVertex[2]});
			

		}
		else if (firstWord == "vt") {

			string word;
			vector<float> tempCoord;

			tempCoord.reserve(5);

			while (iss >> word) {
				float val = stof(word);

				tempCoord.push_back(val);
			}

			textCoorders.push_back(TextCoorder{ tempCoord[0], tempCoord[1] });

		}
		else if (firstWord == "f") {
			// 이 1/2 2/3 3/4를 처리하는 것이 관건이다. 
			// 이게 일단 정점 버퍼의 1,2,3을 가져오고 텍스쳐 좌표의 2,3,4인덱스를 가져오라는거기는해.
			// 혹시 만약 1 2 3 이런 구조라면 정점 버퍼 인덱스를 애기하는거고 텍스쳐 좌표 인덱스는 없는 것이다. 
			string word;

			while (iss >> word) {
				parseFace(word, vertices, textCoorders, lineTriangle, vIdx, vtIdx, idx, lineFaceNum );
				++idx;
			}

		}
		else {
			cout << "유효하지 않는 문자열입니다. " << endl;

		}

	}
}

void printTriangleFace(const vector<Vertex>& vertices, const vector<TextCoorder>& textCoorders, const vector<TriangleFace>& lineTriangle)
{
	
	int size = lineTriangle.size();



	for (int i = 0; i < size; ++i) {
		int vIdx[3];
		

		int vtIdx[3];

		vIdx[0] = lineTriangle[i].faceVertex[0];
		vIdx[1] = lineTriangle[i].faceVertex[1];
		vIdx[2] = lineTriangle[i].faceVertex[2];
	
		vtIdx[0] = lineTriangle[i].faceCoord[0];
		vtIdx[1] = lineTriangle[i].faceCoord[1];
		vtIdx[2] = lineTriangle[i].faceCoord[2];
		
		int verticesSize = vertices.size()-1;

		if (vIdx[0] == vIdx[1] || vIdx[1] == vIdx[2] || vIdx[2] == vIdx[0] ) {

			cout << "삼각형 꼭짓점이 중복되고 있습니다. " << endl;
			return;

		}
		else if (vIdx[0] - 1 < 0 || vIdx[0] - 1 > verticesSize || vIdx[1] - 1 < 0 || vIdx[1] - 1 > verticesSize || vIdx[2] - 1 < 0 || vIdx[2] - 1 > verticesSize) {

			cout << "정점 버퍼의 인덱스 범위를 넘어섰습니다. " << endl;
			return;
		}
			 

		cout << "Face" << " " << lineTriangle[i].lineFace << " " << "(" << vIdx[0] << ", " << vIdx[1] << ", " << vIdx[2] << "): "
			<< "vertex " << "(" << vertices[vIdx[0] - 1].x << ", " << vertices[vIdx[0] - 1].y << ", " << vertices[vIdx[0] - 1].z << ") "
			<< "(" << vertices[vIdx[1] - 1].x << ", " << vertices[vIdx[1] - 1].y << ", " << vertices[vIdx[1] - 1].z << ") "
			<< "(" << vertices[vIdx[2] - 1].x << ", " << vertices[vIdx[2] - 1].y << ", " << vertices[vIdx[2] - 1].z << ") ";

		cout << endl;

		if (vtIdx[0] != -1 || vtIdx[1] != -1 || vtIdx[2] != -1) {

			cout << "texture " << "(" << textCoorders[vtIdx[0] - 1].u << ", " << textCoorders[vtIdx[0] - 1].v << ") "
				<< "(" << textCoorders[vtIdx[1] - 1].u << ", " << textCoorders[vtIdx[1] - 1].v << ") "
				<< "(" << textCoorders[vtIdx[2] - 1].u << ", " << textCoorders[vtIdx[2] - 1].v << ") ";

		}

		cout << endl << endl;

	}
}


int main()
{

	vector<Vertex> vertices;
	vector<TextCoorder> textCoorders;
	vector<TriangleFace> lineTriangle;

	vertices.reserve(100);
	textCoorders.reserve(100);
	lineTriangle.reserve(100);

	ifstream in;
	
	while (!readObjFile(in)) {  
		// 0을 반환하고 뒤집으면 true가 되는듯. 
		// 그니까 0은 false고 !하면 true가 되는거네. 
		system("cls");
	}

	dealWithFile(in, vertices, textCoorders, lineTriangle );   // 여기서 파일 제데로 읽고 자료구조에 저장한다. 

	printTriangleFace(vertices, textCoorders, lineTriangle);


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
