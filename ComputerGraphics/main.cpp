#include <iostream>
#include <random>
#include <array>
#include <algorithm>
#include <fstream>

using namespace std;

random_device rd;

std::mt19937 gen(rd());

uniform_int_distribution<int> dis{ 0, 9 };

// 2개의 matrix를 가진 구조체를 만들어서 Print()도 만들고 재활용하는 방식으로 만들면
// 코드 훨씬 깔끔해질 것 같기는 한데. 


class TwoMatrix {

public:
	array<int, 16> matrix1{};
	array<int, 16> matrix2{};
	int row{ 4 };

	TwoMatrix() = default;

	void makeRandom()
	{
		for (int i = 0; i < row; ++i)
			for (int j = 0; j < row; ++j) {
				matrix1[i * row + j] = dis(gen);
				matrix2[i * row + j] = dis(gen);
			}
	}

	friend ostream& operator<<(ostream& os, const TwoMatrix& twoMatrix) {

		for (int i = 0; i < twoMatrix.row; ++i) {
			for (int j = 0; j < twoMatrix.row; ++j)
				os << twoMatrix.matrix1[i * twoMatrix.row + j] << " ";
			os << endl;
		}
			

		os << endl;

		for (int i = 0; i < twoMatrix.row; ++i) {
			for (int j = 0; j < twoMatrix.row; ++j)
				os << twoMatrix.matrix2[i * twoMatrix.row + j] << " ";
			os << endl;
		}
			
		os << endl;


		return os;
	}

};


void matrixMul(const TwoMatrix& two);
void matrixAdd(const TwoMatrix& two);
void matrixDraw(const TwoMatrix& two);
void matrixTranspos(const TwoMatrix& two);
void matrixMin(const TwoMatrix& two, bool& toggle);
void matrixMax(const TwoMatrix& two, bool& toggle);



int main() {

	TwoMatrix two;
	two.makeRandom();
	char input;
	bool toggleMin{ true };
	bool toggleMax{ true };

	// 두 난수 행렬 출력.
	cout << two; 
	
	do
	{
		cout << endl;
		cout << "명령을 입력해주세요: ";
		cin >> input;

		if (input == 'q') {
			cout << "프로그램 종료합니다." << endl;
			break;
		}
		else if (input == 'm') {
			matrixMul(two);
		}
		else if (input == 'a') {
			matrixAdd(two);
		}
		else if (input == 'd') {
			matrixDraw(two);
		}
		else if (input == 'r') {
			// 스트랭 책에서 이 부분만 한번 봐야될듯. 4x4 행렬식 구하는건 아직 안배웠음.
		}
		else if (input == 't') {
			matrixTranspos(two);
			// 전치행렬 각각 2개씩 행렬식 2개씩 출력. 
		}
		else if (input == 'e') {
			
			matrixMin( two, toggleMin);
			
			if (toggleMin)
				toggleMin = false;
			else
				toggleMin = true;
		}
		else if (input == 'f') {

		}
		else if (input == '+') {

		}
		else if (input == '-') {

		}
		else if (input == 's') {

		}




	} while (true);

}


void matrixMul(const TwoMatrix& two) 
{
	array<int, 16> tempMatrix{};
	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {

			for (int k = 0; k < two.row; ++k) {
				tempMatrix[i * two.row + j] +=
					two.matrix1[(i * two.row) + k] * two.matrix2[j + (two.row * k)];

			}
		}
	}

	for (int i = 0; i < two.row; ++i) {
		for (int j = 0; j < two.row; ++j) {

			cout << tempMatrix[i * two.row + j] << " ";
		}
		cout << endl;
	}


}

void matrixAdd(const TwoMatrix& two)
{
	array<int, 16> tempMatrix{};
	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {
			
			tempMatrix[i * two.row + j] =
				two.matrix1[i * two.row + j] + two.matrix2[i * two.row + j];
		}
	}

	for (int i = 0; i < two.row; ++i) {
		for (int j = 0; j < two.row; ++j) {

			cout << tempMatrix[i * two.row + j] << " ";
		}
		cout << endl;
	}
}


void matrixDraw(const TwoMatrix& two)
{
	array<int, 16> tempMatrix{};
	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {

			tempMatrix[i * two.row + j] =
				two.matrix1[i * two.row + j] - two.matrix2[i * two.row + j];
		}
	}

	for (int i = 0; i < two.row; ++i) {
		for (int j = 0; j < two.row; ++j) {

			cout << tempMatrix[i * two.row + j] << " ";
		}
		cout << endl;
	}

}


void matrixTranspos(const TwoMatrix& two)
{

	TwoMatrix tempMatrix;

	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {

			tempMatrix.matrix1[i * two.row + j] = two.matrix1[i + two.row * j];
		}
	}

	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {

			tempMatrix.matrix2[i * two.row + j] = two.matrix2[i + two.row * j];
		}
	}


	cout << tempMatrix;

}



void matrixMin(const TwoMatrix& two, bool &toggle)
{  // Toggle 형태로 구현하기
	
	TwoMatrix tempMatrix;

	if (toggle) {  // 행에서 최소를 뺴는 경우

		vector<int> least1;
		vector<int> least2;
		least1.reserve(100);
		least2.reserve(100);

		for (int i = 0; i < two.row; ++i) {
			least1.push_back( *min_element( &two.matrix1[ i * two.row ], &two.matrix1[i * two.row] + two.row) );
		
		}

		for (int i = 0; i < two.row; ++i) {
			least2.push_back(*min_element(&two.matrix2[i * two.row], &two.matrix2[i * two.row] + two.row));

		}

		for (int i = 0; i < two.row; ++i) {

			for (int j = 0; j < two.row; ++j) {

				tempMatrix.matrix1[i * two.row + j] = two.matrix1[i * two.row + j] - least1[i];
				tempMatrix.matrix2[i * two.row + j] = two.matrix2[i * two.row + j] - least2[i];

			}
		}

		cout << tempMatrix;

	}
	else {
		// 원래 행렬 그냥 출력
		cout << two;
	}
}


void matrixMax(const TwoMatrix& two, bool& toggle)
{
	TwoMatrix tempMatrix;

	if (toggle) {  // 행에서 최소를 뺴는 경우

		vector<int> max1;
		vector<int> max2;
		max1.reserve(100);
		max2.reserve(100);

		for (int i = 0; i < two.row; ++i) {


		}



		for (int i = 0; i < two.row; ++i) {

			for (int j = 0; j < two.row; ++j) {

				tempMatrix.matrix1[i * two.row + j] = two.matrix1[i * two.row + j] + max1[i];
				tempMatrix.matrix2[i * two.row + j] = two.matrix2[i * two.row + j] - max2[i];

			}
		}

		cout << tempMatrix;

	}
	else {
		// 원래 행렬 그냥 출력
		cout << two;
	}


}





