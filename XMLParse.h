#pragma once
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>

using namespace xercesc;

class XMLParser
{

public:
    XMLParser(const string& xml);
    virtual ~XMLParser();
    void processXMLParse(const char* xmlString);

private:
    void handElement(const DOMDocument* doc);
    void handleSubElements(const DOMDocument* doc);
    void handleSubElement(const DOMNode* node);
    const char* m_xmlString;
    map<string, string> elemMap;
    vector<vector<string, string>> subElems;// handle submsg, if have different type submsg, need have different containers
};