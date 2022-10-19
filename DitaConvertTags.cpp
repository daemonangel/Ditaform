#include "pugixml.hpp"
#include <iostream>
#include <sstream>
#include "Xml.h"
#include "DitaConvertTags.h"
#include "RegulatoryTemplate.h"

#define DitaConvertPrint false

const bool continueTraversal = true;

void DitaConvertTags::ph_node(pugi::xml_node& node)
{
	std::string parent = node.parent().name();
	if (node.attribute("keyref"))
	{
		auto text = node.attribute("keyref").value();
		node.set_name("font");
		Xml::CreateAttrib(node, "color", "red");
		node.attribute("keyref").set_name("data-keyref");
		Xml::SetNodeText(node, text);
	}
	else if (parent == "h1")
	{
		node.set_name("h1");
	}
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

void DitaConvertTags::div_tag(pugi::xml_node& node)
{
	node.set_name("div");
}

void DitaConvertTags::p_tag(pugi::xml_node& node)
{
	node.set_name("p");
}

void DitaConvertTags::step_node(pugi::xml_node& node)
{
	node.set_name("li");
}

void DitaConvertTags::steps_node(pugi::xml_node& node)
{
	node.set_name("ol");
}

void DitaConvertTags::image_node(pugi::xml_node& node)
{
	node.set_name("img");
	node.attribute("href").set_name("src");
	//look for images in the same folder as the source bookmap
	RegulatoryTemplate rg;
	auto fullPath = "graphics/" + QFileInfo(node.attribute("src").value()).fileName();
	node.attribute("src").set_value(fullPath.toStdString().c_str());
	//std::cout << fullPath.toStdString().c_str() << std::endl;
}

void DitaConvertTags::table_node(pugi::xml_node& node)
{
	node.set_name("table");
	Xml::CreateAttrib(node, "border", "1");
	Xml::CreateAttrib(node, "cellpadding", "3");
	Xml::CreateAttrib(node, "cellspacing", "0");
}

void DitaConvertTags::tgroup_node(pugi::xml_node& node)
{
	node.set_name("tbody");
}

void DitaConvertTags::row_node(pugi::xml_node& node)
{
	node.set_name("tr");
}

void DitaConvertTags::entry_node(pugi::xml_node& node)
{
	node.set_name("td");
}

void DitaConvertTags::note_node(pugi::xml_node& node)
{
	node.set_name("p");
	//add admonitions in BOLD: before the node text
}

void DitaConvertTags::comment_node(pugi::xml_node& node)
{
	node.set_name("p");
	if (std::string(node.attribute("outputclass").value()) == "translation")
	{
		Xml::CreateAttrib(node, "style", "background-color:cyan;");
	}
	else
	{
		Xml::CreateAttrib(node, "style", "color:red;background-color:yellow;");
	}
}

void DitaConvertTags::data_node(pugi::xml_node& node)
{
	node.set_name("span");
	std::string selectionType = node.attribute("datatype").value();
	std::string choices = node.attribute("value").value();
	auto text = "Select " + selectionType + " of the following: " + choices;
	Xml::SetNodeText(node, text.c_str());
	Xml::CreateAttrib(node, "style", "color:indigo;background-color:lavender;");
	node.parent().insert_child_before("br", node);
	node.parent().insert_child_after("br", node);
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
    //miscellaneous
	{"ph", DitaConvertTags::ph_node},
    {"uicontrol", DitaConvertTags::uicontrol_node},
    {"title", DitaConvertTags::title_node},
    {"image", DitaConvertTags::image_node},
    {"note", DitaConvertTags::note_node},
    {"draft-comment", DitaConvertTags::comment_node},
    {"data", DitaConvertTags::data_node},

	//ordered list
    {"step", DitaConvertTags::step_node},
    {"steps", DitaConvertTags::steps_node},

	//topics
    {"concept", DitaConvertTags::div_tag},
    {"task", DitaConvertTags::div_tag},
    {"reference", DitaConvertTags::div_tag},
    {"topic", DitaConvertTags::div_tag},
    {"figure", DitaConvertTags::div_tag},

	//blocks
    {"shortdesc", DitaConvertTags::p_tag},
    {"info", DitaConvertTags::p_tag},
    {"cmd", DitaConvertTags::p_tag},

	//tables
    {"table", DitaConvertTags::table_node},
    {"tgroup", DitaConvertTags::tgroup_node},
    {"row", DitaConvertTags::row_node},
    {"entry", DitaConvertTags::entry_node},
	{"simpletable", DitaConvertTags::table_node},
	{"sthead", DitaConvertTags::row_node},
	{"strow", DitaConvertTags::row_node},
	{"stentry", DitaConvertTags::entry_node},
};

struct convert_tags_walker : pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node)
    {
        auto it = DitaConvertTags::nodeEditingMap.find(node.name());
        if (it != DitaConvertTags::nodeEditingMap.end()) // found something
        {
#if _DEBUG && DitaConvertPrint
			std::stringstream ss;
			node.print(ss);
			//std::cout << "Found entry for " << it->first << ":\n\tbefore: " << ss.str() << std::endl;
			ss = {};
#endif
            it->second(node);
#if _DEBUG && DitaConvertPrint
			node.print(ss);
			//std::cout << "\tAfter: " << ss.str() << std::endl;
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
