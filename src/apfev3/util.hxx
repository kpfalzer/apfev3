//
//  util.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_util_hxx
#define apfev3_util_hxx

#include <stdexcept>
#include <ostream>
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

std::string& replaceAll(std::string& s, const std::string& from, const std::string& to);

template<class T>
class SingleOwnerPtr {
public:
    SingleOwnerPtr(T* p = nullptr) {
        __p = p;
    }
    
    SingleOwnerPtr(const SingleOwnerPtr& from) {
        __p = from.__p;
        const_cast<SingleOwnerPtr&>(from).__p = nullptr;
    }
    
    SingleOwnerPtr& operator=(const SingleOwnerPtr& from) {
        __p = from.__p;
        const_cast<SingleOwnerPtr&>(from).__p = nullptr;
        return *this;
    }
    
    bool isValid() const {
        return !isNull();
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
    
    virtual std::ostream& operator<<(std::ostream& os) const {
        os << this->operator*();
        return os;
    }
    
private:
    T* __p;
};

template<class T>
std::ostream&
operator<<(std::ostream& os, const SingleOwnerPtr<T>& ele) {
    return ele.operator<<(os);
}

struct _SList {
    _SList() : _length(0) {}
    
    static void open(const std::string& s);
    static void close(const std::string& s);
    static void sep(const std::string& s);
    
    static const std::string& open() {return _open;}
    static const std::string& close() {return _close;}
    static const std::string& sep() {return _sep;}

    size_t length() const {
        return _length;
    }
    
    bool isEmpty() const {
        return (0 == length());
    }
protected:
    static std::string _open, _close, _sep;
    size_t _length;
};

// std::forward_list implementation is too complicated and didn't work w/ references.
// So, RYO (Roll Your Own).
template<class T>
class SList : public _SList {
public:
    explicit SList()
    : __head(nullptr),
    __tail(nullptr)
    {}
    
    explicit SList(const std::initializer_list<T>& from) : SList() {
        for (auto iter = from.begin(); iter != from.end(); iter++) {
            append(*iter);
            _length += 1;
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
        _length += 1;
        return *this;
    }
        
    SList& operator<<(const T& r) {
        return append(r);
    }
    
    virtual ~SList() {
        for (Link* curr = __head; nullptr != curr; ) {
            Link* next = curr->next;
            delete curr;
            curr = next;
        }
        _length = 0;
    }
    
    std::ostream& operator<<(std::ostream& os) const {
        os << _open;
        for (Link* curr = __head; nullptr != curr; curr = curr->next) {
            if (__head != curr) os << _sep;
            os << curr->data;
        }
        os << _close;
        return os;
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
        
        T& next() {
            T& rval = __curr->data;
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

template<class T>
std::ostream&
operator<<(std::ostream& os, const SList<T>& ele) {
    return ele.operator<<(os);
}

}

#define INVARIANT(_expr) apfev3::invariant(_expr, "Invariant failed: " #_expr)

#endif /* apfev3_util_hxx */
