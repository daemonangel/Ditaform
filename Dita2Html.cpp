#include "pugixml.hpp"
#include "Dita2Html.h"
#include <iostream>
#include <unordered_map>
#include <functional>
#include <sstream>

const bool continueTraversal = true;

using nodeEditingFunction = std::function<void(pugi::xml_node&)>;

pugi::xml_attribute CreateAttrib(pugi::xml_node& parent, const std::string& name, const std::string& value)
{
	auto a = parent.append_attribute(name.c_str());
	a.set_value(value.c_str());
	return a;
}

pugi::xml_node CreateNode(pugi::xml_node& parent, const std::string& name, const std::string& value)
{
	auto n = parent.append_child(name.c_str());
	if (value.size() > 0)
	{
		auto x = n.append_child(pugi::xml_node_type::node_pcdata);
		x.set_value(value.c_str());
	}
	return n;
}

void WrapNode(pugi::xml_node& node, const std::string& name)
{
	auto originalName = std::string(node.name());
	node.set_name(name.c_str());

	auto n = node.append_child(originalName.c_str());
	auto a = n.append_attribute("skip");
	a.set_value("yes");

	for (auto& child : node)
	{
		n.append_move(child);
	}
}

void SetNodeText(pugi::xml_node& node, const pugi::char_t* text)
{
	for (auto& n : node.children())
	{
		if (n.type() == pugi::xml_node_type::node_pcdata)
		{
			n.set_value(text);
			return;
		}
	}

	auto x = node.append_child(pugi::xml_node_type::node_pcdata);
	x.set_value(text);
}

void ph_node(pugi::xml_node& node)
{
	auto text = node.attribute("keyref").value();
	node.set_name("font");
	CreateAttrib(node, "color", "red");
	node.attribute("keyref").set_name("data-keyref");
	SetNodeText(node, text);
}

void uicontrol_node(pugi::xml_node& node)
{
	node.set_name("font-weight");
	CreateAttrib(node, "weight", "bold");
}

void li_node(pugi::xml_node& node)
{
	WrapNode(node, "ul");
}

void title_node(pugi::xml_node& node)
{
	node.set_name("h1");
	
}

/*void default_tag(pugi::xml_node& node)
{
	auto originalName =	std::string(node.name());
	node.set_name("div");
	CreateAttrib(node, "original-name", originalName);
}*/

void do_nothing([[maybe_unused]] pugi::xml_node& node)
{
	// nothing to do
}

std::unordered_map<std::string, nodeEditingFunction> nodeEditingMap =
{
    {"ph", ph_node},
    {"uicontrol", uicontrol_node},
    {"li", li_node},
    {"title", title_node},
};

struct simple_walker : pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node)
    {
        auto it = nodeEditingMap.find(node.name());
        if (it != nodeEditingMap.end() && !node.attribute("skip")) // found something
        {
#if _DEBUG
			std::stringstream ss;
			node.print(ss);
			std::cout << "Found entry for " << it->first << ":\n\tbefore: " << ss.str() << std::endl;
			ss = {};
#endif
            it->second(node);
#if _DEBUG
			node.print(ss);
			std::cout << "\tAfter: " << ss.str() << std::endl;
#endif
        }
		else if(it == nodeEditingMap.end()) // found nothing 
		{
			//default_tag(node);
		}

        
        return continueTraversal;
    }
};

void Dita2Html::convertDitaTags(pugi::xml_node& node)
{
    simple_walker walker;
    node.traverse(walker);
	std::stringstream ss;
	node.print(ss);

	std::cout << "Final xm:\n\n" << ss.str();
}

std::string Dita2Html::makeWellformedHtml([[maybe_unused]] pugi::xml_node& node)
{
    return{};
}