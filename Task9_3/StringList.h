#ifndef STRINGLIST_H
#define STRINGLIST_H

#include "String.h"
#include "List.h"

class StringList : public String, public List<String> {
public:
    StringList(const std::string& str = "");
    void addString(const String& s);
    bool contains(const std::string& key) const override;
    int length() const override { return List<String>::length(); }
};

#endif