#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>

using namespace std;
using namespace xercesc;

int main(int argc, char* argv[])
{
    int rc = 0;
    // if (argc !=2 )
    // {
    //     cout <<<"Usage:"<endl;
    // }

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& e) {
        char* message = XMLString::transcode(e.getMessage());
        std::cerr << "Error during initialization! :"<<endl
                << xercesc::XMLString::transcode(e.getMessage()) << std::endl;
        XMLString::release(&message);
        return 1;
    }

    //optional
    xercesc::XercesDOMParser parser;
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser.setDoNamespaces(true);
    parser.setDoSchema(true);

    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser.setErrorHandler(errHandler);

    try {
        parser.parse("./myfile.xml");
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
    //XMLPlatformUtils::Terminate();//会引发coredump
    return 0;
}
