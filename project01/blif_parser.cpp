#include <unordered_map>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <utility>
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
        int srcIdx = getIndex(src);
        int destIdx = getIndex(dest);
        edges.push_back({src, dest});
    }

    void addVertex(string v) {
        vertexMap[v] = vertexMap.size();
    }

    bool isVertexIn(string v) {
        return vertexMap.find(v) != vertexMap.end();
    }
    void createAdjMatrix() {
        // 初始化鄰接矩陣
        int size = vertexMap.size();
        adjMatrix.resize(size, vector<int>(size, 0));
        // 遍歷所有邊，並在鄰接矩陣中設置值
        for (auto e : edges) {
            int srcIdx = getIndex(e.src);
            int destIdx = getIndex(e.dest);
            cout << "srcIdx: " << e.src << srcIdx << " destIdx: " << e.dest << destIdx << endl;
            adjMatrix[srcIdx][destIdx] = 1;
        }
    }
    void printAdjacent(string v) {
        int idx = getIndex(v);
        if (idx == -1) {
            cout << "node "<< v <<" does not exist" << endl;
            return;
        }
        cout << "Predecessors: ";
        vector<string> predecessors;
        for (int i = 0; i < adjMatrix.size(); i++) {
            if (adjMatrix[i][idx] == 1) {
                predecessors.push_back(getVertex(i));
            }
        }
        if (predecessors.empty()) {
            cout << "-";
        } else {
            copy(predecessors.begin(), predecessors.end() - 1, ostream_iterator<string>(cout, ", "));
            cout << predecessors.back();
        }
        cout << endl;
        cout << "Successors: ";
        vector<string> successors;
        for (int i = 0; i < adjMatrix.size(); i++) {
            if (adjMatrix[idx][i] == 1) {
                successors.push_back(getVertex(i));
            }
        }
        if (successors.empty()) {
            cout << "-";
        } else {
            copy(successors.begin(), successors.end() - 1, ostream_iterator<string>(cout, ", "));
            cout << successors.back();
        }
        cout << endl;
    }
    void adjacencyQuery() {
    string input;
    while (true) {
        cout << "Please input a node: ";
        cin >> input;
        if (input == "0") break;
        if (!isVertexIn(input)) {
            cout << "node " << input << " does not exist" << endl;
        } else {
            printAdjacent(input);
        }
    }
    }
    int getIndex(string v) {
        auto it = vertexMap.find(v);
        return it->second;
    }

    string getVertex(int idx) {
        for (auto const& [key, value] : vertexMap) {
            if (value == idx) {
                return key;
            }
        }
        return "";
    }

    void test() {
        for (auto e : edges) cout << e.src << "->" << e.dest << endl;
        for (auto v : vertexMap) cout << v.first << " index is " << v.second << endl;
    }

private:
    // unordered_map用於存儲頂點
    unordered_map<string, int> vertexMap;
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
                    if(!g.isVertexIn(node)) {
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
    g.test();
    g.createAdjMatrix();
    g.adjacencyQuery();
    return 0;
}

