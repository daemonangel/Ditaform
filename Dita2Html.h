#pragma once
class Dita2Html
{
public:
	static void convertDitaTags(pugi::xml_node& node);
	static std::string makeWellformedHtml(pugi::xml_node& node);
};

