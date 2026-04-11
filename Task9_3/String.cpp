#include "String.h"
#include <cstring>

void String::copyFrom(const char* src) {
    len = strlen(src);
    data = new char[len + 1];
    strcpy(data, src);
}

String::String(const char* str) {
    copyFrom(str);
}

String::String(const std::string& str) {
    copyFrom(str.c_str());
}

String::String(const String& other) {
    copyFrom(other.data);
}

String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        copyFrom(other.data);
    }
    return *this;
}

String::~String() {
    delete[] data;
}

int String::length() const {
    return len;
}

bool String::contains(const std::string& key) const {
    if (key.empty()) return true;
    return strstr(data, key.c_str()) != nullptr;
}

bool String::operator==(const String& other) const {
    return strcmp(data, other.data) == 0;
}