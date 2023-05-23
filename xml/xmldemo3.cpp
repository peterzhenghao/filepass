#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace xercesc;

#define STUDENTS_TAG  "Students"
#define STUDENT_TAG   "Student"
#define NAME_TAG      "Name"
#define AGE_TAG       "Age"
#define SEX_TAG       "Sex"


<?xml version="1.0" encoding="utf-8"?>
<Students>
    <Student>
        <Name>Marco</Name>
        <Age>19</Age>
        <Sex>男</Sex>
    </Student>
    <Student>
        <Name>Mary</Name>
        <Age>20</Age>
        <Sex>女</Sex>
    </Student>
    <Student>
        <Name>Jack</Name>
        <Age>21</Age>
        <Sex>男</Sex>
    </Student>
</Students>

string str1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?><Students><Student><Name>Marco</Name><Age>19</Age><Sex>男</Sex></Student><Student><Name>Mary</Name><Age>20</Age><Sex>女</Sex></Student><Student><Name>Jack</Name><Age>21</Age><Sex>男</Sex></Student></Students>";

struct Student {
    string  Name;
    int     Age;
    string  Sex;
};

enum ErrorCode
{
    LACK_BEGIN = 1,     // 缺少<Students>标记
    LACK_NAME  = 2,     // 缺少姓名
    LACK_AGE   = 3,     // 缺少年龄
    LACK_SEX   = 4      // 缺少性别
};

DOMElement* findStudentsTag(DOMElement* root)
{
    if (root == 0)
        return 0;
    if (XMLString::compareString(XMLString::transcode(root->getTagName()),STUDENTS_TAG) == 0)
        return root;
    else
        return findStudentsTag(root->getNextElementSibling());
}

int visitDom2GetStudents(DOMElement* root,vector<Student>& students)
{
    DOMElement* stustag = findStudentsTag(root);
    if (stustag == 0)
        return LACK_BEGIN;
    DOMNodeList* stulist = root->getElementsByTagName(XMLString::transcode(STUDENT_TAG));
    size_t length = stulist->getLength();
    for (size_t index = 0; index < length; ++index)
    {
        DOMElement* elems = dynamic_cast<DOMElement*>(stulist->item(index));
        DOMElement* name = elems->getFirstElementChild();
        Student stu;
        if (name == 0)
            return LACK_NAME;
        if (XMLString::compareString(XMLString::transcode(name->getTagName()),NAME_TAG) == 0)
        {
            DOMNode* n = dynamic_cast<DOMNode*>(name);
            stu.Name = static_cast<string>(XMLString::transcode(n->getTextContent()));
        }
        else
            return LACK_NAME;

        DOMElement* age = name->getNextElementSibling();
        if (age == 0)
            return LACK_AGE;
        if (XMLString::compareString(XMLString::transcode(age->getTagName()),AGE_TAG) == 0)
        {
            DOMNode* n = dynamic_cast<DOMNode*>(age);
            stu.Age = atoi(XMLString::transcode(n->getTextContent()));
        }
        else
            return LACK_AGE;
        DOMElement* sex = age->getNextElementSibling();
        if (sex == 0)
        {
            return LACK_SEX;
        }
        if (XMLString::compareString(XMLString::transcode(sex->getTagName()),SEX_TAG) == 0)
        {
            DOMNode* n = dynamic_cast<DOMNode*>(sex);
            stu.Sex = static_cast<string>(XMLString::transcode(n->getTextContent()));
        }
        else
            return LACK_SEX;
        students.emplace_back(stu);
    }
    return 0;
}

int main()
{
    try 
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch)
    {
        std::cerr << XMLString::transcode(toCatch.getMessage());
        return -1;
    }

    XercesDOMParser* parser = new XercesDOMParser();
    ErrorHandler* errHandler = new HandlerBase();
    parser->setErrorHandler(errHandler);
    try
    {
        MemBufInputSource input(reinterpret_cast<const XMLByte*>(str1.c_str(), (XMLSize_t)str1.length(), "custom_xml_string"));
        //parser->parse("test.xml");
    }
    catch (const XMLException& toCatch)
    {
        std::cerr << XMLString::transcode(toCatch.getMessage());
        return -1;
    }
    catch (const DOMException& toCatch)
    {
        std::cerr << XMLString::transcode(toCatch.getMessage());
        return -1;
    }
    catch (const SAXException& toCatch)
    {
        std::cerr << XMLString::transcode(toCatch.getMessage());
        return -1;
    }

    DOMDocument* doc = parser->getDocument();
    DOMElement* root = doc->getDocumentElement();
    vector<Student> students;
    int ret = visitDom2GetStudents(root,students);
    if (ret != 0)
        std::cerr << "Parse error\n";
    else
    {
        for (auto stu :students)
        {
            std::cout << "Name: " << stu.Name << "\n"
                  << " Age: " << stu.Age  << "\n"
                  << " Sex: " << stu.Sex  << "\n";
        }
    }
    doc->release();
    XMLPlatformUtils::Terminate();
    return 0;
}