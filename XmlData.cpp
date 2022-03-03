#include "XmlData.h"

XmlData::XmlData()
    : QWidget()
{

}

void XmlData::ProcessData()
{
    //get the bookmap file
    pugi::xml_document bookmapFile;

    pugi::xml_parse_result result = bookmapFile.load_file("source/maps/bm-sample-source.ditamap");

    pugi::xml_node bookmap = bookmapFile.child("bookmap");

    //can also use xpath
    //get the map by finding the first child chapter. only supporting one map per bookmap for now
    auto chapterNode = bookmap.child("chapter");
    auto mapFile = chapterNode.attribute("href").value();

    //prodnameNode.text().set("a test project");

    bookmapFile.save_file("source/maps/bm-sample-source1.ditamap");


    //auto mapHref = pugi::xml_node::find_child(allow_remote_predicate()).attribute("Href").value() << std::endl;

    //search all descendant xml files of the .bookmap file for "props" attribute
        //for each props found:
        //  save the xml_node and props value to variables
        //  search inside the node for "keyref" attribute
        //  for each keyref found:
        //    save the xml_node and keyref value to the keysList
}

