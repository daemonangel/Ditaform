#include "XmlData.h"

// Linker -> System -> SubSystem" to Console for testing - was originally Windows

XmlData::XmlData()
    : QWidget()
{

}

/*struct map_simple_walker : pugi::xml_tree_walker //walks through the map file
{
	XmlData* xmlData;
	bool for_each(pugi::xml_node& node)
	{
		for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

		std::cout << ": name='" << node.name() << "', value='" << node.value() << "'\n";

		if (node.attribute("keys") != nullptr)
		{
			//add to _keysValues
			std::cout << ": keydef name='" << node.attribute("keys").value() << "'\n";
		}
		if (std::string(node.name()) == "keyword" && (node.parent().parent().parent().attribute("keys") != nullptr)) //node must be an ancestor of node with keys attribute
		{
			//add to _keysValues
			std::cout << ": keydef value='" << node.child_value() << "'\n";
		}

		if (std::string(node.name()) == "topicref")
		{
			std::cout << ": href='" << node.attribute("href").value() << "'\n";
			// save each href value to _keysList so i can open each one and traverse it
			xmlData->_topicHrefs.push_back(node.attribute("href").value());
		}

		return true; // continue traversal
	}
};*/

struct topic_simple_walker : pugi::xml_tree_walker //walks through the map file
{
	bool for_each(pugi::xml_node& node)
	{
		for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

		std::cout << ": name='" << node.name() << "', value='" << node.value() << "'\n";

		return true; // continue traversal
	}
};

void XmlData::ProcessData()
{
    //get the bookmap file.
    auto bookFile = "source/bm-sample-source.ditamap";
    pugi::xml_document bookmapFile;
    pugi::xml_parse_result resultBookmap = bookmapFile.load_file(bookFile);

    //get the map file. only supporting one map.
    auto mapHref = bookmapFile.child("bookmap").child("chapter").attribute("href").value();
    pugi::xml_document mapFile;
    auto fullMapPath = std::string("source/") + mapHref;
    pugi::xml_parse_result resultMap = mapFile.load_file(fullMapPath.c_str());

	//get the keydefs from the map
	auto keydefResult = mapFile.select_nodes("//keydef");
	for (auto& result : keydefResult)
	{
		_keysValues.emplace_back(result.node().attribute("keys").value(), result.node().child_value("keywords"));
	}

	//get the topicrefs from the map
	auto topicrefResult = mapFile.select_nodes("//topicref");
	for (auto& result : topicrefResult)
	{
		_topicHrefs.push_back(result.node().attribute("href").value());
	}

    std::cout << fullMapPath << std::endl;

    /*map_simple_walker mapWalker;
	mapWalker.xmlData = this;
    mapFile.traverse(mapWalker);*/

	int i = 0;
	_propsRows.push_back(propValueCollection());

	std::cout << "propsRows stores " << int(_propsRows.size()) << " items.\n";

	for (const auto& href : _topicHrefs) { // fancy ranged for-loop
		//get the topic file.
		pugi::xml_document topicFile;
		auto fullTopicPath = std::string("source/") + href;
		pugi::xml_parse_result resultTopic = topicFile.load_file(fullTopicPath.c_str());

		auto _propResult = topicFile.select_nodes("//*[@props]");
		for (auto& propResult : _propResult)
		{	
			auto p = propResult.node().attribute("props").value();

			//if row containing current props value exists
			if (_propsRows[i].propsName != p)
			{
				//make a new row
				_propsRows.push_back(propValueCollection());

				//if this is not the first row, move to the next index
				if (_propsRows[i].propsName != "") { i++; }

				//set propsName for this row
				_propsRows[i].propsName = p;
			}	

			//add the current node
			_propsRows[i].propsNode.push_back(propResult.node());

			//add all keyrefs inside this node
			//auto _keyrefResult = propResult.node().select_nodes("//*[@keyref]");

			//this only searches immediate children, need all descendants
			for (auto& propChild : propResult.node())
			{
				if (propChild.attribute("keyref"))
				{
					_propsRows[i]._keysList.push_back(propChild.attribute("keyref").value());
				}
			}

			std::cout << "prop value='" << propResult.node().attribute("props").value() << "'\n";
			std::cout << "    struct value='" << _propsRows[i].propsName << ", in row " << i << "'\n";
		}
		std::cout << "propsRows stores " << int(_propsRows.size()) << " items.\n";
		//topic_simple_walker topicWalker;
		//topicFile.traverse(topicWalker);
	}
    //bookmapFile.save_file("source/bm-sample-source1.ditamap");

}

