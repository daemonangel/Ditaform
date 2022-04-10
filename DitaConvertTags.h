#pragma once
#include <functional>
#include <unordered_map>
class DitaConvertTags
{
public:
	using nodeEditingFunction = std::function<void(pugi::xml_node&)>;

public:
	static std::unordered_map<std::string, nodeEditingFunction> nodeEditingMap;

private:
	static void ph_node(pugi::xml_node& node);
	static void uicontrol_node(pugi::xml_node& node);
	static void title_node(pugi::xml_node& node);
	static void do_nothing([[maybe_unused]] pugi::xml_node& node);
	//static void default_tag(pugi::xml_node& node)

public:
	static void convert(pugi::xml_node& node);
};
