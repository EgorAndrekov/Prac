#include "StringList.h"

StringList::StringList(const std::string& str) : String(str) {}

void StringList::addString(const String& s) {
    List<String>::add(s);
}

bool StringList::contains(const std::string& key) const {
    for (int i = 0; i < List<String>::length(); ++i) {
        const String& elem = List<String>::get(i);
        if (std::string(elem.c_str()) == key || elem.contains(key)) {
            return true;
        }
    }
    return false;
}