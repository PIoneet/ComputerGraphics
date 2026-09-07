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
void matrixSub(const TwoMatrix& two);
void matrixDet(const TwoMatrix& two);
void matrixTranspos(const TwoMatrix& two);
void matrixMin(const TwoMatrix& two, bool& toggle);
void matrixMax(const TwoMatrix& two, bool& toggle);
void matrixModuel(TwoMatrix& two, char c);  // 원본 수정할 예정



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
			matrixSub(two);
		}
		else if (input == 'r') {
			// 스트랭 책에서 이 부분만 한번 봐야될듯. 4x4 행렬식 구하는건 아직 안배웠음.
			matrixDet(two);
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

			matrixMax(two, toggleMax);

			if (toggleMax)
				toggleMax = false;
			else
				toggleMax = true;
		}
		else if (input == '+') {

			matrixModuel(two, input);
		
		}
		else if (input == '-') {
			
			matrixModuel(two, input);
		
		}
		else if (input == 's') {
			two.makeRandom();
			cout << two;

			toggleMin = true;
			toggleMax = true;
		}




	} while (true);

}


int determinant(vector<vector<int>> mat, int n)
{
	if (n == 1) return mat[0][0];
	if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	int det = 0;
	int sign = 1;

	for (int col = 0; col < n; ++col) {
		// 0번 행과 col번 열을 제외한 (n-1)x(n-1) 소행렬 만들기
		vector<vector<int>> minor(n - 1, vector<int>(n - 1));
		for (int i = 1; i < n; ++i) { // 0번 행을 제외하고
			int mc = 0;
			for (int j = 0; j < n; ++j) {
				if (j == col) continue;
				minor[i - 1][mc++] = mat[i][j];
			}
		}
		det += sign * mat[0][col] * determinant(minor, n - 1);
		sign = -sign;
	}
	return det;
}



vector<vector<int>> toGrid(const array<int, 16>& m, int n)
{
	vector<vector<int>> grid(n, vector<int>(n));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			grid[i][j] = m[i * n + j];
	return grid;
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


void matrixSub(const TwoMatrix& two)
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




void matrixDet(const TwoMatrix& two)
{
	auto grid1 = toGrid(two.matrix1, two.row);
	auto grid2 = toGrid(two.matrix2, two.row);

	cout << "행렬1의 행렬식: " << determinant(grid1, two.row) << endl;
	cout << "행렬2의 행렬식: " << determinant(grid2, two.row) << endl;
}







void matrixTranspos(const TwoMatrix& two)
{
	TwoMatrix tempMatrix;

	for (int i = 0; i < two.row; ++i)
		for (int j = 0; j < two.row; ++j)
			tempMatrix.matrix1[i * two.row + j] = two.matrix1[i + two.row * j];

	for (int i = 0; i < two.row; ++i)
		for (int j = 0; j < two.row; ++j)
			tempMatrix.matrix2[i * two.row + j] = two.matrix2[i + two.row * j];

	cout << tempMatrix;


	// 추가된 부분
	auto grid1 = toGrid(tempMatrix.matrix1, tempMatrix.row);
	auto grid2 = toGrid(tempMatrix.matrix2, tempMatrix.row);
	cout << "전치행렬1의 행렬식: " << determinant(grid1, tempMatrix.row) << endl;
	cout << "전치행렬2의 행렬식: " << determinant(grid2, tempMatrix.row) << endl;
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

			int maxVal1 = two.matrix1[i];
			int maxVal2 = two.matrix2[i];

			for (int j = 1; j < two.row; ++j) {

				maxVal1 = max(maxVal1, two.matrix1[i + two.row * j]);
				maxVal2 = max(maxVal2, two.matrix2[i + two.row * j]);

			}

			max1.push_back(maxVal1);
			max2.push_back(maxVal2);
		}

		for (int i = 0; i < two.row; ++i) {

			for (int j = 0; j < two.row; ++j) {

				tempMatrix.matrix1[i + two.row * j] = two.matrix1[i + two.row * j] + max1[i];
				tempMatrix.matrix2[i + two.row * j] = two.matrix2[i + two.row * j] + max2[i];

			}
		}

		cout << tempMatrix;

	}
	else {
		// 원래 행렬 그냥 출력
		cout << two;
	}

}


void matrixModuel(TwoMatrix& two, char c) 
{

	for (int i = 0; i < two.row; ++i) {

		for (int j = 0; j < two.row; ++j) {

			two.matrix1[i * two.row + j] = 
				( ( two.matrix1[i * two.row + j] +( (c == '+') ? 1 : -1) ) + 10) % 10;
	
			
			two.matrix2[i * two.row + j] = 
				( ( two.matrix2[i * two.row + j] + ( (c == '+') ? 1 : -1) )+ 10) % 10;
		
		}
	}

	cout << two;

}




