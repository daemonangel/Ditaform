#include "pugixml.hpp"
#include <iostream>
#include <sstream>
#include "Xml.h"
#include "DitaConvertTags.h"

#define DitaConvertPrint true

const bool continueTraversal = true;

void DitaConvertTags::ph_node(pugi::xml_node& node)
{
	auto text = node.attribute("keyref").value();
	node.set_name("font");
	Xml::CreateAttrib(node, "color", "red");
	node.attribute("keyref").set_name("data-keyref");
	Xml::SetNodeText(node, text);
}

void DitaConvertTags::uicontrol_node(pugi::xml_node& node)
{
	node.set_name("font-weight");
	Xml::CreateAttrib(node, "weight", "bold");
}

void DitaConvertTags::title_node(pugi::xml_node& node)
{
	node.set_name("h1");
	
}

/*void DitaConvertTags::default_tag(pugi::xml_node& node)
{
	auto originalName =	std::string(node.name());
	node.set_name("div");
	CreateAttrib(node, "original-name", originalName);
}*/

void DitaConvertTags::do_nothing([[maybe_unused]] pugi::xml_node& node)
{
	// nothing to do
}

std::unordered_map<std::string, DitaConvertTags::nodeEditingFunction> DitaConvertTags::nodeEditingMap =
{
    {"ph", DitaConvertTags::ph_node},
    {"uicontrol", DitaConvertTags::uicontrol_node},
    {"title", DitaConvertTags::title_node},
};

struct convert_tags_walker : pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node)
    {
        auto it = DitaConvertTags::nodeEditingMap.find(node.name());
        if (it != DitaConvertTags::nodeEditingMap.end() && !node.attribute("skip")) // found something
        {
#if _DEBUG && DitaConvertPrint
			std::stringstream ss;
			node.print(ss);
			std::cout << "Found entry for " << it->first << ":\n\tbefore: " << ss.str() << std::endl;
			ss = {};
#endif
            it->second(node);
#if _DEBUG && DitaConvertPrint
			node.print(ss);
			std::cout << "\tAfter: " << ss.str() << std::endl;
#endif
        }
		else if(it == DitaConvertTags::nodeEditingMap.end()) // found nothing 
		{
			//default_tag(node);
		}

        
        return continueTraversal;
    }
};

void DitaConvertTags::convert(pugi::xml_node& node)
{
	convert_tags_walker walker;
    node.traverse(walker);
	std::stringstream ss;
	node.print(ss);

	std::cout << "Converted xml:\n\n" << ss.str();
}
