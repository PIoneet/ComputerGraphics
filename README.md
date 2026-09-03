# 2026 09 03

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

기존에는 없어서 중구난방이었던 코드들을 클래스로 만들어서 코드 간소화를 진행했습니다. 
friend 키워드로 전역함수를 만들어서 연산자 << 오버로딩으로 쉽게 출력이 가능하게 구현했습니다. 생성자를 이용해 TwoMatrix two;를 하면
바로 난수가 대입되게끔 구현했지만 tempMatrix 처럼 난수로 초기화하지 않는 경우도 있다 보니까 makeRadnom이라는 멤버 함수로 그 기능을 이전했습니다. 
크기가 4x4 행렬로 고정되어있기에 array를 사용했습니다. 



