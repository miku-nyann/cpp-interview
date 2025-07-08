#include <iostream>

class HeapOnly {
public:
    HeapOnly() {
        std::cout << "Construct" << std::endl;
    }

    HeapOnly(const HeapOnly& that) {
        std::cout << "Copy Construct" << std::endl;
    }

    HeapOnly& operator=(const HeapOnly& that) {
        std::cout << "Copy =" << std::endl;
        return *this;
    }

    void destory() {
        delete this;
    }

private:
    ~HeapOnly() {
        std::cout << "Destruct" << std::endl;
    }
};

int main() {
    HeapOnly* h1 = new HeapOnly();
    //HeapOnly h2(*h1);
    //HeapOnly h3 = *h1;
    h1->destory();
}
