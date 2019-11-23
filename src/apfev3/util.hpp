//
//  util.hpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_util_hpp
#define apfev3_util_hpp

#include <string>
#include <initializer_list>

namespace apfev3 {
const char* readFile(const char* const fname);

inline const char* readFile(const std::string fname) {
    return readFile(fname.c_str());
}

inline void invariant(bool cond, const char* message) {
    if (!cond) {
        throw std::logic_error(message);
    }
}

template<class T>
class SingleOwnerPtr {
public:
    SingleOwnerPtr(T* p = nullptr) {
        __p = p;
    }
    
    SingleOwnerPtr& operator=(const SingleOwnerPtr& from) {
        __p = from.__p;
        const_cast<SingleOwnerPtr&>(from).__p = nullptr;
        return *this;
    }
    
    bool isNull() const {
        return (nullptr == __p);
    }
    
    operator const T*() const {
        return __p;
    }
    
    operator T*() {
        return __p;
    }
    
    T* operator->() {
        return __p;
    }
    
    const T* operator->() const {
        return __p;
    }
    
    T& operator*() {
        return *__p;
    }
    
    const T& operator *() const {
        return *__p;
    }
    
    void destroy() {
        if (!isNull()) {
            delete __p;
            __p = nullptr;
        }
    }
    
    virtual ~SingleOwnerPtr() {
        destroy();
    }
    
private:
    T* __p;
};

// std::forward_list is too complicated and doesn't work w/ references correctly.
// So, RYO (Roll Your Own).
template<class T>
class SList {
public:
    explicit SList()
    : __head(nullptr),
    __tail(nullptr)
    {}
    
    explicit SList(const std::initializer_list<T>& from) : SList() {
        for (auto iter = from.begin(); iter != from.end(); iter++) {
            append(*iter);
        }
    }
    
    SList& append(const T& data) {
        Link* p = new Link(data);
        if (nullptr == __head) {
            __head = p;
            __tail = p;
        } else {
            __tail->next = p;
            __tail = p;
        }
        return *this;
    }
        
    SList& operator<<(const T& r) {
        return append(r);
    }
    
private:
    struct Link;
    
public:
    friend class Iterator;
    
    class Iterator {
    public:
        explicit Iterator(Link* head)
        : __curr(head)
        {}
        
        //allow default copy constructors
        
        bool hasMore() const {
            return (nullptr != __curr);
        }
        
        T next() {
            T rval = __curr->data;
            __curr = __curr->next;
            return rval;
        }
    private:
        Link* __curr;
    };
    
    Iterator iterator() const {
        return Iterator(__head);
    }
private:
    struct Link {
        explicit Link(const T& _data)
        : data(_data), next(nullptr)
        {}
        
        T data;
        Link* next;
    };
    
    Link* __head;
    Link* __tail;
};
}

#define INVARIANT(_expr) apfev3::invariant(_expr, "Invariant failed: " #_expr)

#endif /* apfev3_util_hpp */
