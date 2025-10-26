#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>

class Complex {
private:
    double real_part;
    double imag_part;

public:
    // 构造函数
    Complex(double real = 0.0, double imag = 0.0) : real_part(real), imag_part(imag) {}
    
    // 获取实部
    double getReal() const {
        return real_part;
    }
    
    // 获取虚部
    double getImag() const {
        return imag_part;
    }
    
    // 计算模
    double magnitude() const {
        return sqrt(real_part * real_part + imag_part * imag_part);
    }
    
    // 重载 == 操作符
    bool operator==(const Complex& other) const {
        const double epsilon = 1e-9;
        return (abs(real_part - other.real_part) < epsilon) && 
               (abs(imag_part - other.imag_part) < epsilon);
    }
    
    // 重载 < 操作符，用于排序（首先按模排序，模相同时按实部排序）
    bool operator<(const Complex& other) const {
        const double epsilon = 1e-9;
        double this_mag = this->magnitude();
        double other_mag = other.magnitude();
        
        if (abs(this_mag - other_mag) < epsilon) {
            // 模相同时按实部排序
            return real_part < other.real_part;
        } else {
            // 按模排序
            return this_mag < other_mag;
        }
    }
    
    // 重载输出操作符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(2) 
           << c.real_part << (c.imag_part >= 0 ? "+" : "") << c.imag_part << "i";
        return os;
    }
};

// 生成随机复数向量
std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-10.0, 10.0);  // 实部和虚部范围[-10, 10]
    
    for (int i = 0; i < size; ++i) {
        // 为了确保有重复项，有时生成相同的复数
        if (i % 5 == 0) {  // 每5个中有一个重复的
            vec.push_back(Complex(1.5, 2.5));  // 人为添加一些重复
        } else {
            vec.push_back(Complex(dis(gen), dis(gen)));
        }
    }
    
    return vec;
}

// 置乱向量
void shuffleVector(std::vector<Complex>& vec) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(vec.begin(), vec.end(), gen);
}

// 查找复数（实部和虚部均相同）
int findComplex(const std::vector<Complex>& vec, const Complex& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return static_cast<int>(i);  // 返回索引
        }
    }
    return -1;  // 未找到
}

// 在指定位置插入复数
void insertComplex(std::vector<Complex>& vec, int index, const Complex& c) {
    if (index >= 0 && index <= static_cast<int>(vec.size())) {
        vec.insert(vec.begin() + index, c);
    }
}

// 删除指定位置的复数
bool removeComplex(std::vector<Complex>& vec, int index) {
    if (index >= 0 && index < static_cast<int>(vec.size())) {
        vec.erase(vec.begin() + index);
        return true;
    }
    return false;
}

// 删除第一个匹配的复数
bool removeComplexByValue(std::vector<Complex>& vec, const Complex& target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    }
    return false;
}

// 向量唯一化（移除重复项）
void uniqueVector(std::vector<Complex>& vec) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

// 起泡排序
void bubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (vec[j + 1] < vec[j]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序辅助函数
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 创建临时向量
    std::vector<Complex> leftVec(n1);
    std::vector<Complex> rightVec(n2);

    // 复制数据到临时向量
    for (int i = 0; i < n1; i++)
        leftVec[i] = vec[left + i];
    for (int j = 0; j < n2; j++)
        rightVec[j] = vec[mid + 1 + j];

    // 合并临时向量
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] < rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        } else {
            vec[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // 复制剩余元素
    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }
    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}

// 归并排序
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        merge(vec, left, mid, right);
    }
}

// 区间查找算法，找出模介于[m1, m2)的所有元素
std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const Complex& c : vec) {
        double mag = c.magnitude();
        if (mag >= m1 && mag < m2) {
            result.push_back(c);
        }
    }
    // 按照排序规则排序结果
    std::sort(result.begin(), result.end());
    return result;
}

// 打印向量
void printVector(const std::vector<Complex>& vec, const std::string& title) {
    std::cout << title << ": ";
    for (const Complex& c : vec) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== 复数向量操作测试 ===" << std::endl;
    
    // (1) 测试无序向量的置乱、查找、插入、删除和唯一化操作
    std::cout << "\n(1) 测试无序向量的置乱、查找、插入、删除和唯一化操作:" << std::endl;
    
    // 生成随机复数向量
    std::vector<Complex> vec = generateRandomComplexVector(10);
    printVector(vec, "原始向量");
    
    // 测试置乱
    shuffleVector(vec);
    printVector(vec, "置乱后向量");
    
    // 测试查找
    Complex target(1.5, 2.5);
    int index = findComplex(vec, target);
    std::cout << "查找 " << target << " 的位置: " << index << std::endl;
    
    // 测试插入
    Complex newComplex(3.0, 4.0);
    insertComplex(vec, 2, newComplex);
    printVector(vec, "在位置2插入 " + std::string("3.00+4.00i") + " 后");
    
    // 测试删除
    removeComplex(vec, 0);  // 删除位置0的元素
    printVector(vec, "删除位置0的元素后");
    
    // 测试唯一化
    std::vector<Complex> vecWithDups = generateRandomComplexVector(12);
    printVector(vecWithDups, "含重复元素的向量");
    uniqueVector(vecWithDups);
    printVector(vecWithDups, "唯一化后的向量");
    
    // (2) 测试排序和效率比较
    std::cout << "\n(2) 测试排序和效率比较:" << std::endl;
    
    // 准备不同顺序的向量
    std::vector<Complex> originalVec = generateRandomComplexVector(1000);
    
    // 顺序向量
    std::vector<Complex> orderedVec = originalVec;
    std::sort(orderedVec.begin(), orderedVec.end());
    
    // 逆序向量
    std::vector<Complex> reverseVec = orderedVec;
    std::reverse(reverseVec.begin(), reverseVec.end());
    
    // 随机向量
    std::vector<Complex> randomVec = originalVec;
    
    // 测试起泡排序
    std::vector<Complex> testVec;
    
    // 测试顺序向量的起泡排序
    testVec = orderedVec;
    clock_t start = clock();
    bubbleSort(testVec);
    clock_t end = clock();
    double bubbleOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "顺序向量起泡排序时间: " << std::fixed << std::setprecision(6) << bubbleOrderedTime << "s" << std::endl;
    
    // 测试逆序向量的起泡排序
    testVec = reverseVec;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    double bubbleReverseTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "逆序向量起泡排序时间: " << std::fixed << std::setprecision(6) << bubbleReverseTime << "s" << std::endl;
    
    // 测试随机向量的起泡排序
    testVec = randomVec;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    double bubbleRandomTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "随机向量起泡排序时间: " << std::fixed << std::setprecision(6) << bubbleRandomTime << "s" << std::endl;
    
    // 测试归并排序
    // 测试顺序向量的归并排序
    testVec = orderedVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    double mergeOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "顺序向量归并排序时间: " << std::fixed << std::setprecision(6) << mergeOrderedTime << "s" << std::endl;
    
    // 测试逆序向量的归并排序
    testVec = reverseVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    double mergeReverseTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "逆序向量归并排序时间: " << std::fixed << std::setprecision(6) << mergeReverseTime << "s" << std::endl;
    
    // 测试随机向量的归并排序
    testVec = randomVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    double mergeRandomTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "随机向量归并排序时间: " << std::fixed << std::setprecision(6) << mergeRandomTime << "s" << std::endl;
    
    // (3) 测试区间查找算法
    std::cout << "\n(3) 测试区间查找算法:" << std::endl;
    
    std::vector<Complex> searchVec = generateRandomComplexVector(20);
    printVector(searchVec, "用于区间查找的向量");
    
    // 对向量进行排序，以便测试区间查找
    std::sort(searchVec.begin(), searchVec.end());
    printVector(searchVec, "排序后的向量");
    
    double m1 = 2.0, m2 = 5.0;
    std::vector<Complex> rangeResult = rangeSearch(searchVec, m1, m2);
    printVector(rangeResult, "模在[" + std::to_string(m1) + "," + std::to_string(m2) + ")之间的元素");
    
    std::cout << "\n效率比较总结:" << std::endl;
    std::cout << "起泡排序 - 顺序:" << std::fixed << std::setprecision(6) << bubbleOrderedTime 
              << "s, 逆序:" << bubbleReverseTime << "s, 随机:" << bubbleRandomTime << "s" << std::endl;
    std::cout << "归并排序 - 顺序:" << std::fixed << std::setprecision(6) << mergeOrderedTime 
              << "s, 逆序:" << mergeReverseTime << "s, 随机:" << mergeRandomTime << "s" << std::endl;
    
    return 0;
}