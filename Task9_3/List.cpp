#include "List.h"
#include "String.h"

template<>
int parseFromString<int>(const std::string& s) {
    return std::stoi(s);
}

template<>
double parseFromString<double>(const std::string& s) {
    return std::stod(s);
}

template<>
std::string parseFromString<std::string>(const std::string& s) {
    return s;
}

template<>
String parseFromString<String>(const std::string& s) {
    return String(s);
}

template<typename T>
List<T>::List(int initialCapacity) : cap(initialCapacity), sz(0) {
    arr = new T[cap];
}

template<typename T>
List<T>::~List() {
    delete[] arr;
}

template<typename T>
void List<T>::resize(int newCap) {
    T* newArr = new T[newCap];
    for (int i = 0; i < sz; ++i) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    cap = newCap;
}

template<typename T>
void List<T>::add(const T& value) {
    if (sz == cap) {
        resize(cap * 2);
    }
    arr[sz++] = value;
}

template<typename T>
T List<T>::get(int index) const {
    if (index < 0 || index >= sz) {
        throw "Index out of range";
    }
    return arr[index];
}

template<typename T>
int List<T>::size() const {
    return sz;
}

template<typename T>
int List<T>::length() const {
    return sz;
}

template<typename T>
bool List<T>::contains(const std::string& key) const {
    T target;
    try {
        target = parseFromString<T>(key);
    } catch (...) {
        return false;
    }
    for (int i = 0; i < sz; ++i) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

template class List<int>;
template class List<double>;
template class List<std::string>;
template class List<String>;