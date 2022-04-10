#pragma once
class Xml
{
public:
	static pugi::xml_attribute CreateAttrib(pugi::xml_node parent, const std::string& name, const std::string& value);
	static pugi::xml_node CreateNode(pugi::xml_node parent, const std::string& name, const std::string& value);
	static void SetNodeText(pugi::xml_node node, const pugi::char_t* text);
};
