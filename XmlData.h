#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "pugixml.hpp"
#include <iostream>

//need a vector to hold all the structs?

struct propValueCollection //one for each prop
{
	using ditaval_node = pugi::xml_node;
	using topic_node = pugi::xml_node;

	std::string propsName;
	std::vector<ditaval_node> propsNodes; //list of nodes because a props name could be on multiple nodes
	std::vector<std::string> _keysList; //from topic: list of keyref values
};

struct key_value_pair
{
	std::string key;
	std::string value;
};

class XmlData
{
public:
	XmlData();
	std::vector<key_value_pair> _keysValues; //from map: keys (keyref) value, keyword value
	std::vector<std::string> _topicHrefs; //from map: list of all the hrefs to topics
	std::unordered_map<std::string, propValueCollection> _propsRows;
	std::vector<pugi::xml_document> xmlDocs;
};
