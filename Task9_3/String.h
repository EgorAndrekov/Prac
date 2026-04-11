#ifndef STRING_H
#define STRING_H

#include "IterableObject.h"
#include <string>

class String : virtual public IterableObject {
private:
    char* data;
    int len;
    void copyFrom(const char* src);
public:
    String(const char* str = "");
    String(const std::string& str);
    String(const String& other);
    String& operator=(const String& other);
    ~String();

    int length() const override;
    bool contains(const std::string& key) const override;
    const char* c_str() const { return data; }
    
    bool operator==(const String& other) const;
};

#endif