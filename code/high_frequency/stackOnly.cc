#include <iostream>
#include <memory>

class StackOnly {
public:
    StackOnly(int v) : _v(v) {}

    void v() {
        std::cout << _v << std::endl;
    }

private:
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
    void* operator new(size_t, void*) = delete; // malloc + placement new

    int _v;
};

int main() {
    StackOnly s1(11);
    s1.v();

    //StackOnly* s2 = new StackOnly(11);

    //StackOnly* tmp3 = (StackOnly*)malloc(sizeof(StackOnly));
    //StackOnly* s3 = new (tmp3) StackOnly(11);

    StackOnly* tmp4 = (StackOnly*)malloc(sizeof(StackOnly));
    tmp4->v();
    //StackOnly* s4 = std::construct_at(tmp4, 11);
    StackOnly* s4 = ::new (tmp4) StackOnly(11);
    s4->v();
}