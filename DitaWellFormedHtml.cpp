#include <string>
#include <iostream>
#include <sstream>
#include "pugixml.hpp"
#include "Xml.h"
#include "DitaWellFormedHtml.h"

const bool continueTraversal = true;

pugi::xml_node DitaWellFormedHtml::li_node(pugi::xml_node& node)
{
	auto ulNode = Xml::CreateNode(node.parent(), "ul", "");
	ulNode.append_move(node);
	return ulNode;
}

pugi::xml_node DitaWellFormedHtml::h1_node(pugi::xml_node& node)
{
	Xml::CreateNode(node.parent(), "br", "");
	return node;
}

/*pugi::xml_node DitaWellFormedHtml::default_tag(pugi::xml_node& node)
{
	auto originalName =	std::string(node.name());
	node.set_name("div");
	Xml::CreateAttrib(node, "original-name", originalName);
	return node;
}*/

pugi::xml_node DitaWellFormedHtml::do_nothing(pugi::xml_node& node)
{
	// nothing to do
	return node;
}

std::unordered_map<std::string, DitaWellFormedHtml::nodeEditingFunction> DitaWellFormedHtml::nodeEditingMap =
{
	{"li", DitaWellFormedHtml::li_node},
	{"h1", DitaWellFormedHtml::h1_node},
};

struct make_well_formed_walker : pugi::xml_tree_walker
{
	virtual bool for_each(pugi::xml_node& node)
	{
		auto it = DitaWellFormedHtml::nodeEditingMap.find(node.name());
		if (it != DitaWellFormedHtml::nodeEditingMap.end() && !node.attribute("skip")) // found something
		{
			it->second(node);
		}
		else if (it == DitaWellFormedHtml::nodeEditingMap.end()) // found nothing 
		{
			//default_tag(node);
		}


		return continueTraversal;
	}
};

std::string DitaWellFormedHtml::makeWellFormed(pugi::xml_node& node)
{
	//pugi::xml_document newDoc;
	//newDoc.append_copy(node);

	auto it = DitaWellFormedHtml::nodeEditingMap.find(node.name());
	if (it != DitaWellFormedHtml::nodeEditingMap.end() && !node.attribute("skip")) // found something
	{
		it->second(node);
	}
	else if (it == DitaWellFormedHtml::nodeEditingMap.end()) // found nothing 
	{
		//default_tag(node);
	}

	std::stringstream ss;
	node.print(ss);

	std::cout << "Well-formed xml:\n\n" << ss.str();
	return ss.str();
}
