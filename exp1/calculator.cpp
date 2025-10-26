#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <functional>
#include <map>
#include <iomanip>

#define N_OPTR 9 //运算符总数
typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

//运算符字符
const char operChar[N_OPTR] = {'+', '-', '*', '/', '^', '!', '(', ')', '\0'};

//运算符优先等级表 [栈顶] [当前]
const char pri[N_OPTR][N_OPTR] = {
    /*              |-------------- 当前运算符 --------------| */
    /*              +    -    *    /    ^    !    (    )   \0 */
    /* -- + */    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* | - */     {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* 栈 * */    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* 顶 / */    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* 运 ^ */    {'>', '>', '>', '>', '>', '<', '<', '>', '>'},
    /* 算 ! */    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
    /* 符 ( */    {'<', '<', '<', '<', '<', '<', '<', '=', ' '},
    /* | ) */     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    /* -- \0 */   {'<', '<', '<', '<', '<', '<', '<', ' ', '='}
};

// 将字符转换为运算符枚举
Operator char2optr(char c) {
    for (int i = 0; i < N_OPTR; i++) {
        if (c == operChar[i]) {
            return (Operator)i;
        }
    }
    return EOE; // 不是运算符则返回终止符
}

// 栈模板类
template<typename T>
class Stack {
private:
    std::vector<T> data;
    
public:
    // 构造函数
    Stack() {}
    
    // 获取栈大小
    int size() const {
        return data.size();
    }
    
    // 判断栈是否为空
    bool empty() const {
        return data.empty();
    }
    
    // 获取栈顶元素
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    // 入栈
    void push(const T& element) {
        data.push_back(element);
    }
    
    // 出栈
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        data.pop_back();
    }
    
    // 清空栈
    void clear() {
        data.clear();
    }
};

// 阶乘函数
double factorial(double n) {
    if (n < 0 || n != (int)n) {
        throw std::runtime_error("Factorial only defined for non-negative integers");
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    double result = 1;
    for (int i = 2; i <= (int)n; i++) {
        result *= i;
    }
    return result;
}

// 执行二元运算
double calculate(double a, Operator op, double b) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV: 
            if (b == 0) {
                throw std::runtime_error("Division by zero");
            }
            return a / b;
        case POW: return pow(a, b);
        default:
            throw std::runtime_error("Invalid binary operation");
    }
}

// 执行一元运算（阶乘）
double calculate(Operator op, double a) {
    if (op == FAC) {
        return factorial(a);
    }
    throw std::runtime_error("Invalid unary operation");
}

// 判断是否为数字字符
bool isDigit(char c) {
    return std::isdigit(c) || c == '.';
}

// 获取运算符优先级
char getPriority(Operator op1, Operator op2) {
    return pri[op1][op2];
}

// 从字符串中提取下一个数字
std::pair<double, int> getNextNumber(const std::string& expr, int start) {
    std::string numStr = "";
    int i = start;
    
    // 处理负号
    if (i < expr.length() && expr[i] == '-' && 
        (i == 1 || expr[i-1] == '(' || expr[i-1] == '+' || expr[i-1] == '-' || 
         expr[i-1] == '*' || expr[i-1] == '/' || expr[i-1] == '^')) {
        numStr += expr[i];
        i++;
    }
    
    // 收集数字字符（包括小数点）
    while (i < expr.length() && (isDigit(expr[i]) || expr[i] == '.')) {
        numStr += expr[i];
        i++;
    }
    
    if (numStr.empty()) {
        throw std::runtime_error("Invalid number format");
    }
    
    try {
        double num = std::stod(numStr);
        return std::make_pair(num, i);
    } catch (...) {
        throw std::runtime_error("Invalid number format");
    }
}

// 字符串计算器主函数（基础运算）
double evaluateBasicExpression(const std::string& expression) {
    if (expression.empty()) {
        return 0;
    }
    
    // 添加起始符和终止符
    std::string expr = '\0' + expression + '\0';
    
    Stack<double> operandStack; // 操作数栈
    Stack<Operator> operatorStack; // 运算符栈
    operatorStack.push(char2optr('\0')); // 先压入起始符
    
    int i = 1; // 从第一个实际字符开始（跳过起始符）
    
    while (i < expr.length()) {
        if (isDigit(expr[i]) || 
            (expr[i] == '-' && (i == 1 || expr[i-1] == '\0' || expr[i-1] == '(' || 
                                expr[i-1] == '+' || expr[i-1] == '-' || 
                                expr[i-1] == '*' || expr[i-1] == '/' || expr[i-1] == '^'))) {
            // 是数字（包括负数）
            auto numInfo = getNextNumber(expr, i);
            operandStack.push(numInfo.first);
            i = numInfo.second;
        } else {
            // 是运算符
            Operator currOp = char2optr(expr[i]);
            
            if (currOp == EOE) {
                // 终止符，跳出循环
                break;
            }
            
            switch (getPriority(operatorStack.top(), currOp)) {
                case '<': // 栈顶运算符优先级低，入栈
                    operatorStack.push(currOp);
                    i++;
                    break;
                    
                case '=': // 优先级相等，脱括号
                    operatorStack.pop(); // 弹出左括号或匹配的运算符
                    i++;
                    break;
                    
                case '>': // 栈顶运算符优先级高，计算
                {
                    Operator op = operatorStack.top();
                    operatorStack.pop();
                    
                    if (op == FAC) {
                        // 阶乘是一元运算
                        if (operandStack.empty()) {
                            throw std::runtime_error("Invalid factorial operation");
                        }
                        double operand = operandStack.top();
                        operandStack.pop();
                        double result = calculate(op, operand);
                        operandStack.push(result);
                    } else {
                        // 二元运算
                        if (operandStack.size() < 2) {
                            throw std::runtime_error("Invalid expression: not enough operands");
                        }
                        double b = operandStack.top();
                        operandStack.pop();
                        double a = operandStack.top();
                        operandStack.pop();
                        double result = calculate(a, op, b);
                        operandStack.push(result);
                    }
                    break;
                }
                
                default:
                    throw std::runtime_error("Invalid priority relation");
            }
        }
    }
    
    // 处理栈中剩余的运算
    while (operatorStack.top() != char2optr('\0')) {
        Operator op = operatorStack.top();
        operatorStack.pop();
        
        if (op == FAC) {
            // 阶乘是一元运算
            if (operandStack.empty()) {
                throw std::runtime_error("Invalid factorial operation");
            }
            double operand = operandStack.top();
            operandStack.pop();
            double result = calculate(op, operand);
            operandStack.push(result);
        } else {
            // 二元运算
            if (operandStack.size() < 2) {
                throw std::runtime_error("Invalid expression: not enough operands");
            }
            double b = operandStack.top();
            operandStack.pop();
            double a = operandStack.top();
            operandStack.pop();
            double result = calculate(a, op, b);
            operandStack.push(result);
        }
    }
    
    if (operandStack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }
    
    return operandStack.top();
}

// 前向声明
double evaluateExtendedExpression(const std::string& expression);

// 更完善的函数表达式解析器
class FunctionParser {
public:
    static double evaluateFunction(const std::string& func_name, double arg) {
        if (func_name == "sin") {
            return sin(arg * M_PI / 180); // 假设输入为度数
        } else if (func_name == "cos") {
            return cos(arg * M_PI / 180); // 假设输入为度数
        } else if (func_name == "tan") {
            return tan(arg * M_PI / 180); // 假设输入为度数
        } else if (func_name == "log") {
            if (arg <= 0) throw std::runtime_error("Log of non-positive number");
            return log10(arg);
        } else if (func_name == "ln") {
            if (arg <= 0) throw std::runtime_error("Ln of non-positive number");
            return log(arg);
        } else if (func_name == "sqrt") {
            if (arg < 0) throw std::runtime_error("Square root of negative number");
            return sqrt(arg);
        } else if (func_name == "abs") {
            return abs(arg);
        } else {
            throw std::runtime_error("Unknown function: " + func_name);
        }
    }
    
    // 解析包含函数的表达式
    static std::string parseAndReplaceFunctions(const std::string& expression) {
        std::string result = expression;
        bool changed = true;
        
        while (changed) {
            changed = false;
            
            // 查找并替换函数调用（按函数名长度降序，避免短名称干扰长名称）
            std::vector<std::string> functions = {"sin", "cos", "tan", "log", "ln", "sqrt", "abs"};
            
            for (const std::string& func : functions) {
                size_t pos = 0;
                while ((pos = result.find(func, pos)) != std::string::npos) {
                    // 检查函数名后是否跟着左括号
                    size_t func_end = pos + func.length();
                    if (func_end < result.length() && result[func_end] == '(') {
                        size_t start_pos = pos;
                        size_t paren_count = 0;
                        size_t i = func_end + 1; // 跳过左括号
                        
                        // 找到匹配的右括号，处理嵌套括号
                        while (i < result.length()) {
                            if (result[i] == '(') {
                                paren_count++;
                            } else if (result[i] == ')') {
                                if (paren_count == 0) {
                                    // 找到匹配的右括号
                                    std::string arg_str = result.substr(func_end + 1, i - func_end - 1);
                                    try {
                                        // 递归解析参数表达式
                                        double arg_value = evaluateExtendedExpression(arg_str);
                                        double func_result = evaluateFunction(func, arg_value);
                                        
                                        // 替换整个函数调用
                                        std::string replacement = std::to_string(func_result);
                                        result.replace(start_pos, i - start_pos + 1, replacement);
                                        
                                        changed = true;
                                        pos = start_pos + replacement.length();
                                        break; // 从新位置继续搜索
                                    } catch (const std::exception& e) {
                                        // 如果参数解析失败，移到下一个可能的位置
                                        pos = i + 1;
                                        break;
                                    }
                                } else {
                                    paren_count--;
                                }
                            }
                            i++;
                        }
                        
                        if (i >= result.length()) {
                            pos = func_end + 1; // 没有找到匹配括号，移到下一个位置
                        }
                    } else {
                        pos++; // 函数名后没有左括号，移到下一个位置
                    }
                }
            }
        }
        
        return result;
    }
};

// 扩展版计算器，支持复杂函数
double evaluateExtendedExpression(const std::string& expression) {
    // 先解析并替换函数调用
    std::string processed_expr = FunctionParser::parseAndReplaceFunctions(expression);
    
    // 然后用基础计算器计算结果
    return evaluateBasicExpression(processed_expr);
}

// 测试函数
void runTests() {
    std::cout << "=== 字符串计算器测试 ===" << std::endl;
    
    std::vector<std::string> testCases = {
        "2+3",           // 基础加法
        "2*3+5",         // 混合运算
        "(2+3)*5",       // 括号运算
        "2^3",           // 幂运算
        "5!",            // 阶乘
        "2+3*4",         // 运算优先级
        "(2+3)*4-5",     // 复杂表达式
        "((2+3)*4-5)/2", // 嵌套括号
        "2+-3",          // 负数
        "-5+3",          // 表达式以负号开始
    };
    
    std::cout << "\n基础运算测试：" << std::endl;
    for (const auto& test : testCases) {
        try {
            double result = evaluateBasicExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    // 错误情况测试
    std::cout << "\n错误处理测试：" << std::endl;
    std::vector<std::string> errorCases = {
        "2++3",          // 连续运算符
        "2)",            // 缺少左括号
        "(2+3",          // 缺少右括号
        "5!",            // 有效阶乘
    };
    
    for (const auto& test : errorCases) {
        try {
            double result = evaluateBasicExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    // 复杂函数测试
    std::cout << "\n复杂函数测试：" << std::endl;
    std::vector<std::string> functionTests = {
        "sin(30)",      // sin(30°) = 0.5
        "cos(60)",      // cos(60°) = 0.5
        "tan(45)",      // tan(45°) = 1
        "log(100)",     // log(100) = 2
        "ln(2.718)",    // ln(e) ≈ 1
        "sqrt(16)",     // sqrt(16) = 4
        "abs(-5)",      // abs(-5) = 5
    };
    
    for (const auto& test : functionTests) {
        try {
            double result = evaluateExtendedExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    // 混合运算测试
    std::cout << "\n混合运算测试：" << std::endl;
    std::vector<std::string> mixedTests = {
        "2+sin(30)",    // 2 + 0.5 = 2.5
        "sqrt(16)*2",   // 4 * 2 = 8
    };
    
    for (const auto& test : mixedTests) {
        try {
            double result = evaluateExtendedExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
}

int main() {
    runTests();
    
    // 交互式测试
    std::cout << "\n=== 交互式计算器 ===" << std::endl;
    std::cout << "输入表达式进行计算（输入'quit'退出）：" << std::endl;
    
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        try {
            double result = evaluateExtendedExpression(input);
            std::cout << "= " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "错误: " << e.what() << std::endl;
        }
    }
    
    return 0;
}