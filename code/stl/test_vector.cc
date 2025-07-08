#include "vector.h"

#include <iostream>

struct VecElement {
    VecElement(int val) : _val(val) {
        std::cout << "construct " << _val << std::endl;
    }

    ~VecElement() {
        std::cout << "destruct " << _val << std::endl;
    }

    int _val;
};

int main() {
    vector<VecElement> v;
#if 0
    v.push_back(2);
    v.push_back(4);
    v.push_back(6);
#else 
    v.emplace_back(2);
    v.emplace_back(4);
    v.emplace_back(6);
#endif 
    v.resize(8, 8);
    v.resize(4, 8);
}
