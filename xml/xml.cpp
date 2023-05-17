#include "xml.h"
#include "Parser.h"
using namespace wzh::xml;
 
#include <fstream>
#include <sstream>
using namespace std;
 
Value::Value() {
}
 
Value::Value(bool value) {
    *this = value;
}
 
Value::Value(int value) {
    *this = value;
}
 
Value::Value(double value) {
    *this = value;
}
 
Value::Value(const char * value) : m_value(value) {
}
 
Value::Value(const string & value) : m_value(value) {
}
 
Value::~Value() {
}
 
Value & Value::operator = (bool value) {
    m_value = value ? "true" : "false";
    return *this;
}
 
Value & Value::operator = (int value) {
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}
 
Value & Value::operator = (double value) {
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}
 
Value & Value::operator = (const char * value) {
    m_value = value;
    return *this;
}
 
Value & Value::operator = (const string & value) {
    m_value = value;
    return *this;
}
 
Value & Value::operator = (const Value & value) {
    m_value = value.m_value;
    return *this;
}
 
bool Value::operator == (const Value & other) {
    return m_value == other.m_value;
}
 
bool Value::operator != (const Value & other) {
    return !(m_value == other.m_value);
}
 
Value::operator bool() {
    if (m_value == "true")
        return true;
    else if (m_value == "false")
        return false;
    return false;
}
 
Value::operator int() {
    return std::atoi(m_value.c_str());
}
 
Value::operator double() {
    return std::atof(m_value.c_str());
}
 
Value::operator string() {
    return m_value;
}
 
XML::XML() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {
 
}
 
XML::XML(const char * name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {
    m_name = new string(name);
}
 
XML::XML(const string & name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {
    m_name = new string(name);
}
 
XML::XML(const XML & other) {
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
}
 
string XML::name() const {
    if(m_name == nullptr) {
        return "";
    }
    return *m_name; 
}
 
void XML::setName(const string & name) {
    if(m_name != nullptr) {
        delete m_name;
        m_name = nullptr;
    }
    m_name = new string(name);
}
 
string XML::text() const {
    if(m_text == nullptr) {
        return "";
    }
    return *m_text;
}
                
void XML::setText(const string & text) {
    if(m_text != nullptr) {
        delete m_text;
        m_text = nullptr;
    }
    m_text = new string(text);
}
 
Value & XML::attr(const string & key) {
    if(m_attrs == nullptr) {
        m_attrs = new std::map<string, Value>();
    }
    return (*m_attrs)[key];
}
                
void XML::setAttr(const string & key, const Value & val) {
    if(m_attrs == nullptr) {
        m_attrs = new std::map<string, Value>();
    }
    (*m_attrs)[key] = val;
}
 
string XML::str() const {
    if(m_name == nullptr) {
        throw std::logic_error("element name is empty");
    }
 
    stringstream ss;
    ss << "<";
    ss << *m_name;
    if(m_attrs != nullptr) {
        for(auto it = m_attrs->begin(); it != m_attrs->end(); it++) {
            ss << " " << it->first << "=\"" << (string)it->second << "\"";
        }
    }
    ss << ">";
    if(m_child != nullptr) {
        for(auto it = m_child->begin(); it != m_child->end(); it++) {
            ss << it->str();
        }
    }
    if(m_text != nullptr) {
        ss << *m_text;
    }
    ss << "</" << *m_name << ">";
    return ss.str();
}
 
void XML::clear() {
    if(m_name != nullptr) {
        delete m_name;
        m_name = nullptr;
    }
    if(m_text != nullptr) {
        delete m_text;
        m_text = nullptr;
    }
    if(m_attrs != nullptr) {
        delete m_attrs;
        m_attrs = nullptr;
    }
    if(m_child != nullptr) {
        for(auto it = m_child->begin(); it != m_child->end(); it++) {
            it -> clear();
        }
        delete m_child;
        m_child = nullptr;
    }
}
 
void XML::append(const XML & child) {
    if(m_child == nullptr) {
        m_child = new std::list<XML>();
    }
    m_child->push_back(child);
}
 
XML & XML::operator [] (int index) {
    if(index < 0) {
        throw std::logic_error("index less than zero");
    }
    if(m_child == nullptr) {
        m_child = new std::list<XML>();
    }
    int size = m_child->size();
    if(index >= 0 && index < size) {
        auto it = m_child->begin();
        for(int i=0; i<index; i++)
            it++;
        return *it;
    }
    if (index >= size) {
        for(int i=size; i<=index; i++)
            m_child->push_back(XML());
    }
    return m_child->back();
}
 
XML & XML::operator [] (const char * name) {
    return (*this)[string(name)];
}
 
XML & XML::operator [] (const string & name) {
    if(m_child == nullptr) {
        m_child = new std::list<XML>();
    }
    for(auto it = m_child->begin(); it != m_child->end(); it++) {
        if(it->name() == name) {
            return *it;
        }
    }
    m_child->push_back(XML(name));
    return m_child->back();
}
 
XML & XML::operator = (const XML & other) {
    clear();
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
    return *this;
}
 
void XML::remove(int index) {
    if(m_child == nullptr) {
        return;
    }
    int size = m_child->size();
    if(index < 0 || index >= size) {
        throw std::logic_error("index out of range");
        return;
    }
    auto it = m_child->begin();
    for(int i=0; i<index; i++) {
        it++;
    }
    it->clear();
    m_child->erase(it);
}
 
void XML::remove(const char * name) {
    remove(string(name));
}
 
void XML::remove(const string & name) {
    if(m_child == nullptr) {
        return;
    }
    for(auto it = m_child->begin(); it != m_child->end();) {
        if(it->name() == name) {
            it->clear();
            it = m_child->erase(it);
        }
        else {
            it++;
        }
    }
}
 
bool XML::load(const string & filename)
{
    Parser p;
    if (!p.load_file(filename))
    {
        return false;
    }
    *this = p.parse();
    return true;
}
 
bool XML::save(const string & filename)
{
    ofstream fout(filename);
    if (fout.fail())
    {
        return false;
    }
    fout << str();
    fout.close();
    return true;
}
 
bool XML::parse(const string & str)
{
    Parser p;
    if (!p.load_string(str))
    {
        return false;
    }
    *this = p.parse();
    return true;
}
