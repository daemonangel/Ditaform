#include "XmlData.h"

// Linker -> System -> SubSystem" to Console for testing - was originally Windows

std::vector<key_value_pair> GetKeyDefsAndValues(const pugi::xml_document& mapFile)
{
	std::vector<key_value_pair> keysAndValues;

	auto keydefResult = mapFile.select_nodes("//keydef");
	for (auto& result : keydefResult)
	{
		keysAndValues.emplace_back(result.node().attribute("keys").value(), result.node().child_value("keywords"));
	}
	
	return keysAndValues;
}

std::vector<std::string> GetTopicHrefs(const pugi::xml_document& mapFile)
{
	std::vector<std::string> topicHrefs;

	auto topicrefResult = mapFile.select_nodes("//topicref");
	for (auto& result : topicrefResult)
	{
		topicHrefs.push_back(result.node().attribute("href").value());
	}

	return topicHrefs;
}

XmlData::XmlData()
{
	//get the bookmap file.
	auto& bookmapFile = xmlDocs.emplace_back();
	pugi::xml_parse_result resultBookmap = bookmapFile.load_file(sourceBookmapFile);

	//get the map file. only supporting one map.
	//TODO support multiple maps - maybe do this after first release
	auto mapHref = bookmapFile.child("bookmap").child("chapter").attribute("href").value();
	auto& mapFile = xmlDocs.emplace_back();
	auto fullMapPath = std::string("source/") + mapHref;
	pugi::xml_parse_result resultMap = mapFile.load_file(fullMapPath.c_str());

	_keysValues = GetKeyDefsAndValues(mapFile);

	_topicHrefs = GetTopicHrefs(mapFile);

	std::cout << fullMapPath << std::endl;

	std::cout << "propsRows stores " << int(_propsRows.size()) << " items.\n";

	for (const auto& href : _topicHrefs) // fancy ranged for-loop
	{
		//get the topic file.
		auto& topicFile = xmlDocs.emplace_back();
		auto fullTopicPath = std::string("source/") + href;
		pugi::xml_parse_result resultTopic = topicFile.load_file(fullTopicPath.c_str());

		auto _propResult = topicFile.select_nodes("//*[@props]");
		for (auto& propResult : _propResult)
		{
			auto propsNode = propResult.node();
			auto propName = propsNode.attribute("props").value();

			auto& propsRow = _propsRows[propName]; // make new or find existing (note that the & is VERY important)
			propsRow.propsName = propName; // we may be re-assigning the name but that's ok
			propsRow.propsNodes.push_back(propsNode);

			//add all keyrefs inside this node
			auto keyrefResult = propsNode.select_nodes(".//*[@keyref]");
			for (auto& propChild : keyrefResult)
			{
				if (propChild.node().attribute("keyref"))
				{
					propsRow._keysList.push_back(propChild.node().attribute("keyref").value());
				}
			}
			//TODO need a way to deal with multiple instances of the same keyref in the form. maybe when u edit one, it automatically puts the same text in all of them?

			//std::cout << "prop value='" << propsNode.attribute("props").value() << "'\n";
			//std::cout << "    struct value='" << _propsRows[i].propsName << ", in row " << i << "'\n";
		}
		std::cout << "propsRows stores " << int(_propsRows.size()) << " items.\n";
	}
}
