#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "pugixml.hpp"
#include <iostream>
#include "qstring.h"

//need a vector to hold all the structs?

struct propValueCollection //one for each prop
{
	using ditaval_node = pugi::xml_node;
	using topic_node = pugi::xml_node;

	std::string propsName;
	std::vector<ditaval_node> propsNodes; //list of nodes because a props name could be on multiple nodes
	std::vector<std::string> _keysList; //from topic: list of keyref values
	//TODO add checkbox value 
};

struct key_value_pair
{
	std::string key;
	std::string value;
};

struct data_node
{
	enum class RuleType
	{
		One,
		Any,
	};

	//relationship nodes for storing dependency info
	std::string parent;
	RuleType rule;
	std::vector<std::string> children;

	// check if `name` exists in `children`
	inline bool isChild(const std::string& name)
	{
		return find(children.begin(), children.end(), name) != children.end();
	}

	// check if anything in `names` exists in `children`
	inline bool isAnyChild(const std::vector<std::string>& names)
	{
		for (auto& name : names)
		{
			std::vector<std::string>::iterator it;

			it = find(children.begin(), children.end(), name);
			if (it != children.end())
			{
				return true;
			}
		}
		return false;
	}
};

class XmlData
{
public:
	XmlData();
	void processTopics();
	void addKeyrefs(const pugi::xml_node&, propValueCollection*);
	void addDataNodes(const pugi::xpath_node_set&);
	std::vector<key_value_pair> _keysValues; //from map: keys (keyref) value, keyword value
	std::vector<std::string> _topicHrefs; //from map: list of all the hrefs to topics
	std::vector<std::unique_ptr<propValueCollection>> _propsRows;
	std::vector<pugi::xml_document> xmlDocs;
	std::vector<data_node> _dataNodes;
};
