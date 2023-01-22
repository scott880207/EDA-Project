#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Edge結構體用於存儲圖中的邊
struct Edge {
    string src, dest;
};

// Graph結構體用於存儲圖的鄰接表
class Graph {

public:
    void addEdge(string src, string dest) {
        edges.push_back({src, dest});
    }
    void addVertex(string v) {
        vertexMap[v] = v;
    }
    bool isVertexIn(string v) {
        return vertexMap.find(v) == vertexMap.end();
    }
    void test() {
        int i = 0;
        for (auto e : edges) cout << e.src << "->" << e.dest << endl;
    }
    void createAdjMatrix() {
        // 初始化鄰接矩陣
        int size = vertexMap.size();
        adjMatrix.resize(size, vector<int>(size, 0));
        // 遍歷所有邊，並在鄰接矩陣中設置值
        for (auto e : edges) {
            int srcIdx = getIndex(e.src);
            int destIdx = getIndex(e.dest);
            adjMatrix[srcIdx][destIdx] = 1;
        }
    }

    int getIndex(string v) {
        auto it = vertexMap.find(v);
        if (it != vertexMap.end()) {
            return distance(vertexMap.begin(), it);
        }
        return -1;
    }

private:
    // unordered_map用於存儲頂點
    unordered_map<string, string> vertexMap;
    // 儲存邊的陣列
    vector<Edge> edges;
    // 儲存鄰接矩陣
    vector<vector<int>> adjMatrix;

};

// 讀取BLIF文件並建立圖
void readBLIF(string fileName, Graph &g) {
    // 打開BLIF文件
    ifstream file(fileName);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            string first_token;
            string node;
            vector<string> nodes;

            stringstream ss(line);
            ss >> first_token;

            if (first_token == ".model") {
                // do something with .model line
            } else if (first_token == ".inputs") {
                string input;
                while (ss >> input) {
                    g.addVertex(input);
                }
            } else if (first_token == ".outputs") {
                string output;
                while (ss >> output) {
                    g.addVertex(output);
                }
            } else if (first_token == ".names") {
                while (ss >> node) {
                    nodes.push_back(node);
                }
                // 將這些點加入g.vertexMap中
                for (string node : nodes) {
                    if(g.isVertexIn(node)) {
                        g.addVertex(node);
                    }
                }
                // 第一個點開始遍歷，並將每個點與最後一個點加入Edge
                for (int i = 0; i < nodes.size() - 1; i++) {
                    g.addEdge(nodes[i], nodes.back());
                }
            } else if (first_token == ".end") {
                break;
            } else {}

            //g.test();
        }
        file.close();
    } else {
        cout << "Unable to open file" << endl;
    }
}

int main() {
    Graph g;
    readBLIF("sample01.blif", g);
    return 0;
}

