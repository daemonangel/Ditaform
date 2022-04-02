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

	//TODO support multiple maps. This is for later.
	
	//get the map file.
	auto mapHref = bookmapFile.child("bookmap").child("chapter").attribute("href").value();
	auto& mapFile = xmlDocs.emplace_back();
	auto fullMapPath = std::string("source/") + mapHref;
	pugi::xml_parse_result resultMap = mapFile.load_file(fullMapPath.c_str());

	_keysValues = GetKeyDefsAndValues(mapFile);

	_topicHrefs = GetTopicHrefs(mapFile);

	std::cout << fullMapPath << std::endl;

	processTopics();
}

void XmlData::processTopics()
{
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
		}
	}
}
