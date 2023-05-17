#pragma once
 
#include "xml.h"
#include <string>
using namespace std;
 
namespace wzh {
    namespace xml{
 
        class Parser {
 
            public:
                Parser();
 
                bool load_file(const string & file);
                bool load_string(const string & str);
 
                XML parse(); // 解析方法
 
            private:
                void skip_white_space(); // 忽略空白符
 
                bool parse_declaration(); // 解析声明
 
                bool parse_comment(); // 解析注释
 
                XML parse_element(); // 解析主函数
 
                string parse_element_name(); // 解析节点名称
 
                string parse_element_text(); // 解析节点文本
 
                string parse_element_attr_key(); // 解析节点属性
 
                string parse_element_attr_val(); // 解析节点属性值
 
            private:
                string m_str;
                int m_idx;
 
        };
 
    }
}