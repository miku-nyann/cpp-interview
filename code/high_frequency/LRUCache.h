#pragma once 

#include <unordered_map>

class LRUCache {
    struct LRUNode {
        LRUNode(int key = 0, int value = 0) 
            : _prev(nullptr), _next(nullptr), _key(key), _value(value) {}
        LRUNode*    _prev;
        LRUNode*    _next;
        int         _key;
        int         _value;
    };

    using Lookup = std::unordered_map<int, LRUNode*>;

public:
    explicit LRUCache(int capacity) 
        : _capacity(capacity), _size(0) {
        _list._prev = &_list;
        _list._next = &_list;
    }

    ~LRUCache() {
        while (!empty())
            pop();
    }

    int get(int key) {
        auto it = _lookup.find(key);
        if (it == _lookup.end())
            return -1;
        LRUNode* node = it->second;
        erase(node);
        append(node);
        return node->_value;
    }

    void put(int key, int value) {
        LRUNode* node;
        auto it = _lookup.find(key);
        if (it != _lookup.end()) {
            node = it->second;
            node->_value = value;
            erase(node);
        } else {
            node = new LRUNode(key, value);
            if (_size == _capacity)
                pop();
            _lookup.emplace(key, node);
        }
        append(node);
    }

    bool empty() const { return _list._next == &_list; }

private:
    void append(LRUNode* node) {
        node->_next = _list._next;
        node->_prev = &_list;
        node->_next->_prev = node;
        node->_prev->_next = node;
        ++_size;
    }

    void erase(LRUNode* node) {
        node->_next->_prev = node->_prev;
        node->_prev->_next = node->_next;
        node->_next = nullptr;
        node->_prev = nullptr;
        --_size;
    }

    void pop() {
        LRUNode* n = _list._prev;
        erase(n);
        _lookup.erase(n->_key);
        delete n;
    }

private:
    Lookup      _lookup;
    LRUNode     _list;
    int         _capacity;
    int         _size;
};
