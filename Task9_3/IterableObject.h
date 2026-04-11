#ifndef ITERABLEOBJECT_H
#define ITERABLEOBJECT_H

#include <string>

class IterableObject {
public:
    virtual ~IterableObject() {}
    virtual int length() const = 0;
    virtual bool contains(const std::string& key) const = 0;
};

#endif