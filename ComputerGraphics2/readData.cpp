#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include <windows.h>
#include <random>


using namespace std;

random_device rd;

std::mt19937 gen(rd());

uniform_int_distribution<int> dis{ 1, 6 };




class TextData {

private:
	vector<string> strData;
	int line;
	

public:
	TextData() : line{10} {
		strData.reserve(100);
	}

	int getLine() const
	{
		return line;
	}
	// 위처럼 만들면 const 객체에서도 non-const 객체에서도 호출 가능.

	const vector<string>& getStrData() const
	{
		return strData;
	}

	vector<string>& getStrData()
	{
		return strData;
	}


	friend ostream& operator<<(ostream& os, const TextData& textData) 
	{
		for (int i = 0; i < textData.strData.size(); ++i) {			

			os << textData.strData[i] << "\n";
		}
		return os;

	}
};


void changeCase(const TextData& textData, bool& switchCase);
void wordOutput(const TextData& textData);
void setUpperColor(const TextData& textData, bool& switchUpperColor);


int main() {

	TextData textData;

	string fileName;

	bool switchCase{ true };
	bool switchUpperColor{ true };

	cout << "input data file name: ";
	cin >> fileName;

	ifstream in{ fileName };

	if (!in.is_open())
		return 1;

	for (int i = 0; i < textData.getLine(); ++i) {
		string temp;
		getline(in, temp);

		textData.getStrData().push_back(temp);

	}

	cout << textData << endl;


	do {
		cout << endl << endl;

		char input;
		cout << "Input the command: ";
		cin >> input;

		cout << endl;

		if (input == 'q') {
			cout << "Program closed" << endl;
			break;
		}
		else if (input == 'a') {
			changeCase(textData, switchCase);

		}
		else if (input == 'b') {
			wordOutput(textData);

		}
		else if (input == 'c') {
			setUpperColor(textData, switchUpperColor);
		}
		else if (input == 'd') {

		}
		else if (input == 'e') {

		}
		else if (input == 'f') {

		}
		else if (input == 'g') {

		}
		else if (input == 'h') {

		}
		else if (input == 'i') {

		}
		else if (input == 'j') {

		}


	} while (true);


	// isstringstream 은 문자열을 읽는 커서가 존재하는데 그게 eof에 도달하면 false를 반환한다.

}


void changeCase(const TextData& textData, bool& switchCase)
{
	TextData tempData;

	if (switchCase) {

		int count = textData.getLine();

		for (int i = 0; i < count; ++i) {
			stringstream lineStream( textData.getStrData()[i] );
			string temp;
			string result;

			while (lineStream >> temp) {

				for (int j = 0; j < temp.size(); ++j) {

					if (isupper(temp[j]))
						temp[j] = tolower(temp[j]);
					else if (islower(temp[j]))
						temp[j] = toupper(temp[j]);
				
				}
				result += temp + " ";

			}
			tempData.getStrData().push_back(result);
		}

		cout << tempData;
		switchCase = false;
	}
	else {
		cout << textData;
		switchCase = true;
	}
	
}
// 인자로 const TextData로 받으면 TextData 클래스의 const 멤버 함수만 호출가능하다는 애기다. 
// 멤버 함수에 const가 붙어있으면 그 함수에서 멤버 변수를 변경하지 않겠다는 약속이다. 
// vector<string>& 은 원본 수정 가능한 값을 반환하는거라서 const 멤버 함수랑 상충된다. 
// const vector<string>&을 반환해야됨.


void wordOutput(const TextData& textData) 
{
	int count = textData.getLine();

	for (int i = 0; i < count; ++i) {
		stringstream lineStream(textData.getStrData()[i]);
		string temp;
		int wordNum{};

		while (lineStream >> temp) {
			++wordNum;
		}

		cout << textData.getStrData()[i] << " " << wordNum << endl;
		
	}
}

void setUpperColor(const TextData& textData, bool& switchUpperColor)
{
	TextData tempData;

	if (switchUpperColor) {

		int count = textData.getLine();

		for (int i = 0; i < count; ++i) {
			stringstream lineStream(textData.getStrData()[i]);
			string temp;

			while (lineStream >> temp) {
				char firstChar = temp[0];

				if (isupper(firstChar)) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), dis(gen));
					cout << temp << " ";
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					cout << temp << " ";
				}
				

			}
			
			cout << endl;

		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		switchUpperColor = false;
	}
	else {
		cout << textData;
		switchUpperColor = true;
	}

}