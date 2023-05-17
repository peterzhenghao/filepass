#pragma once
 
#include <string>
#include <list>
#include <map>
using namespace std;
 
namespace wzh {
    namespace xml {
 
        class Value {
            // 数值转化类
            public:
                Value();
                Value(bool value);
                Value(int value);
                Value(double value);
                Value(const char * value);
                Value(const string & value);
                ~Value();
 
                Value & operator = (bool value);
                Value & operator = (int value);
                Value & operator = (double value);
                Value & operator = (const char * value);
                Value & operator = (const string & value);
                Value & operator = (const Value & value);
 
                bool operator == (const Value & other);
                bool operator != (const Value & other);
 
                operator bool();
                operator int();
                operator double();
                operator string();
 
            private:
                string m_value;
        };
 
        class XML {
 
            public:
                XML();
                XML(const char * name);
                XML(const string & name);
                XML(const XML & other);
 
                string name() const; // 获取节点名称
                void setName(const string & name); // 设置节点名称
 
                string text() const; // 获取节点内容
                void setText(const string & text); // 设置节点内容
 
                Value & attr(const string & key); // 获取节点属性
                void setAttr(const string & key, const Value & val); // 设置节点属性
 
                string str() const; // 返回属性序列化结果
 
                void clear(); // 释放内存
 
                void append(const XML & child); // 添加子节点
 
                XML & operator [] (int index); // 以数组下标方式获取子节点
                XML & operator [] (const char * name); // 通过节点名称获取子节点(支持C形式)
                XML & operator [] (const string & name); // 通过节点名称获取子节点(支持C++形式)
 
                XML & operator = (const XML & other);
 
                void remove(int index); // 以数组下标方式删除子节点
                void remove(const char * name); // 通过节点名称删除子节点(支持C形式)
                void remove(const string & name); // 通过节点名称删除子节点(支持C++形式)
 
                typedef std::list<XML>::iterator iterator;
 
                iterator begin() {
                    return m_child->begin();
                }
 
                iterator end() {
                    return m_child->end();
                }
 
                iterator erase(iterator it) {
                    it->clear();
                    return m_child->erase(it);
                }
 
                int size() const {
                    return m_child->size();
                }
 
                bool load(const string & filename);
 
                bool save(const string & filename);
                
                bool parse(const string & str);
 
 
            private:
                // 使用指针加快数据的读取以及存储开销
                string* m_name; // 节点名称
                string* m_text; // 节点内容
                std::map<string, Value>* m_attrs; // 节点属性
                std::list<XML>* m_child; // 子节点
        };
 
    }
}