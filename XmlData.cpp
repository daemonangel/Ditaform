#include "XmlData.h"

XmlData::XmlData()
    : QWidget()
{

}

//get the bookmap
pugi::xml_document bookmap;
pugi::xml_parse_result result = bookmap.load_file("../source/maps/bm-sample-source.ditamap");

//get the map by finding the first child chapter with a href attribute
pugi::xml_document map;
//auto mapHref = pugi::xml_node::find_child(allow_remote_predicate()).attribute("Href").value() << std::endl;

//search all descendant xml files of the .bookmap file for "props" attribute
    //for each props found:
    //  save the xml_node and props value to the keysList
    //  search inside the node for "keyref" attribute
    //  for each keyref found:
    //    save the xml_node and keyref value to the keysList