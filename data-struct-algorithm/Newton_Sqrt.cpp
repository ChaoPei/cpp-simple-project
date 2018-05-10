/**
 * 牛顿迭代法求n的开方
 * 
 * 设计y=x^2-n函数，利用牛顿迭代求函数的根
 */

#include <iostream>

#define EPS 0.000001

int main() {
    float n;
    std::cin >> n;
    float x0 = n;
    while(true) {
        float x1 = x0 / 2 + n / x0 / 2;
        std::cout << x1 << std::endl;
        if((x1 - x0) < EPS && (x1 - x0) > -EPS) {
            std::cout << "result: " << x1 << std::endl;
            break;
        }
        x0 = x1;
    }
    return 0;
}