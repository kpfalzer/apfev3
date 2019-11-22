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

}

#define INVARIANT(_expr) apfev3::invariant(_expr, "Invariant failed: " #_expr)

#endif /* apfev3_util_hpp */
