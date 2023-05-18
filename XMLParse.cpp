#include <iostream>
#include "XMLParse.h"


using namespace std;
using namespace xercesc;


XMLParser::XMLParser(const string& xml)
    : m_xmlString(xml.c_str())
{
    
}

XMLParser:: ~XMLParser() 
{
    //TODO
}

XMLParser::processXMLParse()
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& e) {
        char* message = XMLString::transcode(e.getMessage());
        std::cerr << "Error during initialization! :"<<endl
                << xercesc::XMLString::transcode(e.getMessage()) << std::endl;
        XMLString::release(&message);
        //return 1;
    }

    //optional
    xercesc::XercesDOMParser parser;
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser.setDoNamespaces(true);
    parser.setDoSchema(true);

    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser.setErrorHandler(errHandler);

    try {
        parser.parse(xmlString);
        //DOMDocument* xmlDoc = parser->getDocument();
        DOMDocument* xmlDoc = parser->adoptDocument();
        handleSubElements(xmlDoc);
        handSubElement
    } 
    catch (const xercesc::XMLException& e) {
        std::cerr << "Error parsing XML file: "
                << xercesc::XMLString::transcode(e.getMessage()) << std::endl;
        rc = -1;
    }
    catch (const DOMException& e) {
        std::cerr << "DOMException during parsing XML file at line "
                  << e.getLineNumber() << ", column "
                  << e.getColumnNumber() << ": "
                  << xercesc::XMLString::transcode(e.getMessage())
                  << std::endl;
        rc = -1;
    }
    catch (const SAXParseException& e) {
        std::cerr << "SAXParseException during parsing XML file at line "
                  << e.getLineNumber() << ", column "
                  << e.getColumnNumber() << ": "
                  << xercesc::XMLString::transcode(e.getMessage())
                  << std::endl;
        rc = -1;
    }
    catch (...){
        cout<< "Unexpected Exception During Parsing"<<endl;
    }

    if (rc == 0)
        cout << "XML File :" << "parese OK" << std::endl;

    delete errHandler;

}

XMLParser::handElement(const DOMDocument* doc)
{
    DOMElement*  rootNode = doc->getDocumentElement();
    std::cout << "rootNode Name  : " << toCh(rootNode->getNodeName()) << std::endl;

    if (rootNode->haselemNodes())
    {
        DOMNodeList* elemList = rootNode->getElemNodes();
        for (XMLSize_t j = 0; j < elemList->getLength(); j++ ) {
        {
            DOMNode* elemNode = elemList->item(j);
            if (elemNode->getNodeType() == DOMNode::ENTITY_NODE)
            {
                const char* nodeName = XMLString::transcode(elemNode->getNodeName());// compile issue?
                XMLString::trim(nodeName);
                if (nodeName == ENTITLEMENT_LIST)
                    continue;//submsg handle
                char* nodeValue = XMLString::transcode(elemNode->getTextContent());
                XMLString::trim(nodeValue);
                elemMap.insert(pair<string, string>(nodeName, nodeValue));
                XMLString::release(nodeName);
                XMLString::release(nodeValue);
            }
        }
    }
}

XMLParser::handleSubElements(const DOMDocument* doc)
{
    DOMNodeList* elemList = doc->getElementsByTagName(toXMLCh("ENTITLEMENT_DATA"));// 从哪个节点开始循环就用哪个节点
    for ( XMLSize_t i = 0; i < elemList->getLength(); i++ ) 
    {
        std::cout << "Entilement(" << i << ") ";
        handleSubElement(elemList->item(i));
    }
}

XMLParser::handleSubElement(const DOMNode* node)
{
    if (!rootNode->haselemNodes())
        return;

    DOMNodeList* elemList = rootNode->getElemNodes();
    vector<string, string> subMsg;
    for (XMLSize_t j = 0; j < elemList->getLength(); j++ ) {
    {
        DOMNode* elemNode = elemList->item(j);
        if (elemNode->getNodeType() == DOMNode::ENTITY_NODE)
        {
            const char* nodeName = XMLString::transcode(elemNode->getNodeName());// compile issue?
            XMLString::trim(nodeName);
            char* nodeValue = XMLString::transcode(elemNode->getTextContent());
            XMLString::trim(nodeValue);
            subMsg.emplace_back(nodeName, nodeValue);
            XMLString::release(nodeName);
            XMLString::release(nodeValue);
        }
    }
    subElems.push_back(subMsg);
}

XMLParser::~XMLParser()
{
}
