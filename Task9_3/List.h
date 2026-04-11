#ifndef LIST_H
#define LIST_H

#include "IterableObject.h"
#include <string>
#include <sstream>

template<typename T>
class List : virtual public IterableObject {
private:
    T* arr;
    int cap;
    int sz;
    void resize(int newCap);
public:
    List(int initialCapacity = 4);
    List(const List& other) = delete;
    List& operator=(const List& other) = delete;
    ~List();

    void add(const T& value);
    T get(int index) const;
    int size() const;
    int length() const override;
    bool contains(const std::string& key) const override;
};

template<typename T>
T parseFromString(const std::string& s);

template<>
int parseFromString<int>(const std::string& s);

template<>
double parseFromString<double>(const std::string& s);

template<>
std::string parseFromString<std::string>(const std::string& s);

class String;
template<>
String parseFromString<String>(const std::string& s);

#endif