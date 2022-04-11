#pragma once
#include <unordered_map>
#include <functional>

class DitaWellFormedHtml
{
public:
	using nodeEditingFunction = std::function<void(pugi::xml_node&)>;

public:
	static std::unordered_map<std::string, nodeEditingFunction> nodeEditingMap;

private:
	static pugi::xml_node li_node(pugi::xml_node& node);
	static pugi::xml_node add_break(pugi::xml_node& node);
	static pugi::xml_node do_nothing(pugi::xml_node& node);
	//pugi::xml_node default_tag(const pugi::xml_node& node)

public:
	static std::string makeWellFormed(pugi::xml_node& node);
};
