#include "pugixml.hpp"
#include "Xml.h"

pugi::xml_attribute Xml::CreateAttrib(pugi::xml_node parent, const std::string& name, const std::string& value)
{
	auto a = parent.append_attribute(name.c_str());
	a.set_value(value.c_str());
	return a;
}

pugi::xml_node Xml::CreateNode(pugi::xml_node parent, const std::string& name, const std::string& value)
{
	auto n = parent.append_child(name.c_str());
	if (value.size() > 0)
	{
		auto x = n.append_child(pugi::xml_node_type::node_pcdata);
		x.set_value(value.c_str());
	}
	return n;
}

void Xml::SetNodeText(pugi::xml_node node, const pugi::char_t* text)
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
