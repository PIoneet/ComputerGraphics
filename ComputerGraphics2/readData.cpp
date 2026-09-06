#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include <windows.h>
#include <random>
#include <algorithm>


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
void reverseOutput(const TextData& textData, bool& switchReverse);
void changeSpaceToStar(const TextData& textData, bool& switchSpace);
void reverseWords(const TextData& textData, bool& switchReverseWords);
void changeLetter(const TextData& textData, bool& switchLetter);
void lineWrap(const TextData& textData, bool& switchLine);
void inputColor(const TextData& textData, string& word);
void moveLine(TextData& textData);



int main() {

	TextData textData;

	string fileName;

	bool switchCase{ true };
	bool switchUpperColor{ true };
	bool switchReverse{ true };
	bool switchSpace{true};
	bool switchReverseWords{ true };
	bool switchLetter{ true };
	bool switchLine{ true };


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
			reverseOutput(textData, switchReverse);
		}
		else if (input == 'e') {
			// e는 원본 수정하는건지? , *을 계속 남기는건지 물어봐야됨.
			changeSpaceToStar(textData, switchSpace);
		}
		else if (input == 'f') {
			// e로 추가된 *를 기준으로 구분하기를 원하는건지 물어보자. 
			reverseWords(textData, switchReverseWords);
		}
		else if (input == 'g') {
			// 대소문자 구분하는 형태로 구현했음. A바꾸면 a는 바꾸지 않음.
			changeLetter(textData, switchLetter);
		}
		else if (input == 'h') {
			lineWrap(textData, switchLine );
		}
		else if (input == 'i') {
			string word;
			cin >> word;

			inputColor(textData, word);

		}
		else if (input == 'j') {
			moveLine(textData);
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
	if (switchUpperColor) {

		int cntColor{};
		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			stringstream lineStream(textData.getStrData()[i]);
			string temp;

			while (lineStream >> temp) {
				char firstChar = temp[0];

				if (isupper(firstChar)) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), dis(gen));
					cout << temp << " ";
					++cntColor;
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					cout << temp << " ";
				}
				
			}
			
			cout << endl;

		}
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << endl;
		cout << "Number of Changed Colors: " << cntColor << endl;
		
		switchUpperColor = false;
	}
	else {
		cout << textData;
		switchUpperColor = true;
	}
}



void reverseOutput(const TextData& textData, bool& switchReverse)
{

	if (switchReverse) {
		
		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			string temp = textData.getStrData()[i];
			
			reverse(temp.begin(), temp.end());
			
			cout << temp << endl;
		}

		
		switchReverse = false;
	}
	else {
		cout << textData;
		switchReverse = true;
	}

}


void changeSpaceToStar(const TextData& textData, bool& switchSpace)
{
	TextData tempData;

	if (switchSpace) {

		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			stringstream lineStream(textData.getStrData()[i]);
			string temp;
			string result;

			bool first{ true };
			while (lineStream >> temp) {
				
				if (!first) result += "*";
				result += temp;
				first = false;
			
			}

			tempData.getStrData().push_back(result);
		
		}

		cout << tempData << endl;
		switchSpace = false;
	}
	else {
		cout << textData;
		switchSpace = true;
	}

}


void reverseWords(const TextData& textData, bool& switchReverseWords)
{
	if (switchReverseWords) {

		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			string temp = textData.getStrData()[i];

			for (int j = 0; j < temp.size(); ++j) {
				if (temp[j] == '*')
					temp[j] = ' ';
			}

			stringstream tempLine(temp);
			vector<string> words;
			string tempWord;
			bool first{ true };
			words.reserve(100);

			while (tempLine >> tempWord) {
				words.push_back(tempWord);
			}

			for (auto it = words.rbegin(); it != words.rend(); ++it) {
				if (!first) cout << *it << " ";
				else cout << *it;

				first = false;
			}

			cout << endl;
		}
		switchReverseWords = false;
	}
	else {
		cout << textData;
		switchReverseWords = true;
	}

}


void changeLetter(const TextData& textData, bool& switchLetter)
{
	TextData tempData;

	if (switchLetter) {
		
		char letter, input;
		cout << "Letter you want to change: ";
		cin >> letter;
		cout << "Put Input: ";
		cin >> input;

		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			stringstream lineStream(textData.getStrData()[i]);
			string temp;
			string result;
			bool first{ true };


			while (lineStream >> temp) {
				
				for (int j = 0; j < temp.size(); ++j) {
					if (temp[j] == letter)
						temp[j] = input;
				}

				if (!first) result += " ";
				first = false;

				result += temp;

			}

			tempData.getStrData().push_back(result);

		}

		cout << tempData << endl;
		switchLetter = false;
	}
	else {
		cout << textData;
		switchLetter = true;
	}

}



void lineWrap(const TextData& textData, bool& switchLine)
{

	if (switchLine) {

		int lineCount = textData.getLine();

		for (int i = 0; i < lineCount; ++i) {
			
			stringstream lineStream(textData.getStrData()[i]);
			string temp;
	

			while (lineStream >> temp) {
				
				int j{};
				for (; j < temp.size(); ++j) {
			
					cout << temp[j];

					if (isdigit(temp[j])) {
						
						cout << "\n";
					}
				}

				if( !isdigit(temp[j-1]) )
					cout << " ";
			}

			cout << "\n";

		}

		switchLine = false;
	}
	else {
		cout << textData;
		switchLine = true;
	}
}




void inputColor(const TextData& textData, string& word)
{
	for (int i = 0; i < word.size(); ++i) {
		if (isupper(word[i]))
			word[i] = tolower(word[i]);
		// 소문자로 통일하기
	}

	int cntColor{};
	int lineCount = textData.getLine();

	for (int i = 0; i < lineCount; ++i) {
		stringstream lineStream(textData.getStrData()[i]);
		string temp;
		bool first{ true };

		while (lineStream >> temp) {
			
			string original = temp;

			for (int j = 0; j < temp.size(); ++j) {
				if (isupper(temp[j]))
					temp[j] = tolower(temp[j]);
			}

			if (word == temp) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), dis(gen)); 
				++cntColor;
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7 );
			}
				
			if (!first) original += " ";
			first = false;

			cout << original;

		}

		cout << endl;

	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << endl;
	cout << "Number of Changed Colors: " << cntColor << endl;

}



void moveLine(TextData& textData)
{	
	
	auto begin = textData.getStrData().begin();
	auto end = textData.getStrData().end();
		
	rotate(begin, end - 1, end);

	

	cout << textData;


}