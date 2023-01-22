#include "graph.h"

// C 中的有向圖實現
int main(void)
{
    // 包含圖邊的輸入數組(如上圖)
    // 數組中的 (x, y) 對錶示從 x 到 y 的一條邊
    struct Edge edges[] =
    {
        {0, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 2}, {4, 5}, {5, 4}
    };
 
    // 計算總邊數
    int E = sizeof(edges)/sizeof(edges[0]);
 
    // 從給定的邊構造一個圖
    struct Graph *graph = createGraph(6, edges, E);
 
    // 打印圖的鄰接表表示的函數
    printGraph(graph);
 
    return 0;
}


