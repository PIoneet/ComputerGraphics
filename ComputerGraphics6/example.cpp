#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;


struct Vertex {
    double x{}, y{}, z{};
};


struct TexCoord {
    double s{}, t{};
};


struct RawFace {
    vector<string> tokens; 
    int lineNo{};
};


bool isAllDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}


void readObjFile(const string& filename, vector<Vertex>& vertices,
    vector<TexCoord>& texcoords, vector<RawFace>& faces)
{
    ifstream fin(filename);
    if (!fin) {
        cout << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    string line;
    int lineNo = 0;

    while (getline(fin, line)) {
        ++lineNo;

        istringstream iss(line);
        string tok;
        if (!(iss >> tok)) continue;

        if (tok == "#") {
            continue; 
        }
        else if (tok == "v") {
            double x, y, z;
            if (!(iss >> x >> y >> z)) {
                cout << "[줄 " << lineNo << "] v 데이터 형식이 올바르지 않습니다: " << line << endl;
                continue;
            }
            vertices.push_back({ x, y, z });
        }
        else if (tok == "vt") {
            double s, t;
            if (!(iss >> s >> t)) {
                cout << "[줄 " << lineNo << "] vt 데이터 형식이 올바르지 않습니다: " << line << endl;
                continue;
            }
            texcoords.push_back({ s, t });
        }
        else if (tok == "f") {
            RawFace rf;
            rf.lineNo = lineNo;
            string ft;
            while (iss >> ft) {
                rf.tokens.push_back(ft);
            }
            faces.push_back(rf);
        }
        else {
           
            cout << "[줄 " << lineNo << "] 허용되지 않는 문자값입니다: \"" << tok << "\"" << endl;
        }
    }
}


bool parseFaceToken(const string& token, int& vIdx, int& vtIdx, bool& hasTex)
{
    size_t slashPos = token.find('/');

    string vPart = (slashPos == string::npos) ? token : token.substr(0, slashPos);
    string vtPart = (slashPos == string::npos) ? "" : token.substr(slashPos + 1);

    if (!isAllDigits(vPart)) {
        return false;
    }
    vIdx = stoi(vPart);

    hasTex = !vtPart.empty();
    if (hasTex) {
        if (!isAllDigits(vtPart)) {
            return false;
        }
        vtIdx = stoi(vtPart);
    }

    return true;
}


void processFace(const RawFace& rf, int faceNum, const vector<Vertex>& vertices,
    const vector<TexCoord>& texcoords)
{
 
    if (rf.tokens.size() != 3) {
        cout << "Face " << faceNum << " [줄 " << rf.lineNo << "]: 삼각형이 이루어지지 않습니다 (꼭짓점 개수 "
            << rf.tokens.size() << "개)" << endl;
        return;
    }

    int vIdx[3]{};
    int vtIdx[3]{};
    bool hasTex[3]{};


    for (int k = 0; k < 3; ++k) {
        if (!parseFaceToken(rf.tokens[k], vIdx[k], vtIdx[k], hasTex[k])) {
            cout << "Face " << faceNum << " [줄 " << rf.lineNo << "]: 허용되지 않는 인덱스 값 \""
                << rf.tokens[k] << "\"" << endl;
            return;
        }
    }


    for (int k = 0; k < 3; ++k) {
        if (vIdx[k] < 1 || vIdx[k] > (int)vertices.size()) {
            cout << "Face " << faceNum << " [줄 " << rf.lineNo << "]: 정점 인덱스가 범위를 벗어났습니다 ("
                << vIdx[k] << ")" << endl;
            return;
        }
        if (hasTex[k] && (vtIdx[k] < 1 || vtIdx[k] > (int)texcoords.size())) {
            cout << "Face " << faceNum << " [줄 " << rf.lineNo << "]: 텍스처 인덱스가 범위를 벗어났습니다 ("
                << vtIdx[k] << ")" << endl;
            return;
        }
    }


    if (vIdx[0] == vIdx[1] || vIdx[1] == vIdx[2] || vIdx[0] == vIdx[2]) {
        cout << "Face " << faceNum << " [줄 " << rf.lineNo << "]: 꼭짓점 인덱스가 중복되어 삼각형이 이루어지지 않습니다 ("
            << vIdx[0] << ", " << vIdx[1] << ", " << vIdx[2] << ")" << endl;
        return;
    }


    cout << "Face " << faceNum << " (" << vIdx[0] << ", " << vIdx[1] << ", " << vIdx[2] << "): vertex ";
    for (int k = 0; k < 3; ++k) {
        const Vertex& v = vertices[vIdx[k] - 1];  //인덱스는 시작이 1이니까 -1을 해줘서 뺴야 한다.
        cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
    }
    cout << endl;

    bool allHaveTex = hasTex[0] && hasTex[1] && hasTex[2];  // 텍스쳐 좌표가 없는 경우도 예외처리를 하는듯.
    if (allHaveTex) {
        cout << "                     texture ";
        for (int k = 0; k < 3; ++k) {
            const TexCoord& vt = texcoords[vtIdx[k] - 1];
            cout << "(" << vt.s << ", " << vt.t << ") ";
        }
        cout << endl;
    }
}


void checkDuplicateVertices(const vector<Vertex>& vertices)
{
    bool found = false;

    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (vertices[i].x == vertices[j].x &&
                vertices[i].y == vertices[j].y &&
                vertices[i].z == vertices[j].z) {
                cout << "Duplicate vertex value: index " << (i + 1) << " and " << (j + 1) << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "No duplicate vertex value" << endl;
    }
}

int main()
{
    string filename;
    cout << "input data file name: ";
    cin >> filename;

    vector<Vertex> vertices;
    vector<TexCoord> texcoords;
    vector<RawFace> faces;

    readObjFile(filename, vertices, texcoords, faces);

    cout << endl;

    int faceNum = 0;
    for (const RawFace& rf : faces) {
        ++faceNum;
        processFace(rf, faceNum, vertices, texcoords);
    }

    cout << endl;
    checkDuplicateVertices(vertices);

    return 0;
}
