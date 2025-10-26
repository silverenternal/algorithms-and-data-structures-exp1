#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

/**
 * 使用单调栈解决柱状图中最大矩形面积问题
 * 时间复杂度: O(n)
 * 空间复杂度: O(n)
 */
int largestRectangleArea(vector<int>& heights) {
    if (heights.empty()) return 0;
    
    stack<int> st;  // 存储柱子的索引
    int maxArea = 0;
    
    // 在数组末尾添加一个高度为0的柱子，确保所有柱子都能被处理
    heights.push_back(0);
    
    for (int i = 0; i < heights.size(); i++) {
        // 当栈不为空且当前柱子高度小于栈顶柱子高度时
        while (!st.empty() && heights[i] < heights[st.top()]) {
            int h = heights[st.top()];  // 获取栈顶柱子的高度
            st.pop();
            
            // 计算以当前柱子为高度的最大矩形面积
            // 宽度是当前索引i减去新的栈顶索引再减1
            int w = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        st.push(i);  // 将当前索引压入栈
    }
    
    // 恢复原始数组（移除添加的0）
    heights.pop_back();
    
    return maxArea;
}

// 辅助函数：生成随机测试数据
vector<int> generateRandomHeights(int length, int maxHeight = 10000) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, maxHeight);
    
    vector<int> heights(length);
    for (int i = 0; i < length; i++) {
        heights[i] = dis(gen);
    }
    return heights;
}

// 辅助函数：打印数组
void printHeights(const vector<int>& heights) {
    cout << "[";
    for (size_t i = 0; i < heights.size(); i++) {
        cout << heights[i];
        if (i < heights.size() - 1) cout << ", ";
    }
    cout << "]";
}

int main() {
    cout << "柱状图中最大矩形面积问题 - 单调栈解法" << endl;
    cout << "====================================" << endl;
    
    // 一些测试用例
    cout << "基本测试用例：" << endl;
    
    vector<vector<int>> testCases = {
        {2, 1, 5, 6, 2, 3},
        {2, 4, 6, 8},
        {1, 1, 1, 1},
        {5},
        {0, 0, 0},
        {1, 2, 3, 4, 5, 4, 3, 2, 1}
    };
    
    for (size_t i = 0; i < testCases.size(); i++) {
        vector<int> heights = testCases[i];
        cout << "测试用例 " << i+1 << ": ";
        printHeights(heights);
        int result = largestRectangleArea(heights);
        cout << " -> 最大面积: " << result << endl;
    }
    
    cout << "\n随机测试用例 (10组 - 简化输出):" << endl;
    cout << "====================================" << endl;
    
    // 随机生成10组测试数据
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 1; i <= 10; i++) {
        // 随机生成数组长度 (1 to 1000) 和最大高度 (0 to 1000) - 减小数据规模以便查看
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> lenDis(1, 1000);
        uniform_int_distribution<> maxHDis(0, 1000);
        
        int length = lenDis(gen);
        int maxHeight = maxHDis(gen);
        
        vector<int> heights = generateRandomHeights(length, maxHeight);
        
        int result = largestRectangleArea(heights);
        cout << "随机测试 " << i << ": 长度=" << length << " -> 最大面积: " << result << endl;
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\n所有测试完成，总耗时: " << duration.count() << " ms" << endl;
    
    return 0;
}