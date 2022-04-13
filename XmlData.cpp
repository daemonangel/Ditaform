#include "XmlData.h"
#include "DitaConvertTags.h"
#include "RegulatoryTemplate.h"

// Linker -> System -> SubSystem" to Console for testing - was originally Windows

//TODO future support for keydef to resource file
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
	RegulatoryTemplate _regTemp;
	pugi::xml_parse_result resultBookmap = bookmapFile.load_file(_regTemp.sourceBookmapFile.toStdString().c_str());

	//TODO support multiple maps. This is for later.
	
	//get the map file.
	std::string mapHref = bookmapFile.child("bookmap").child("chapter").attribute("href").value();
	auto& mapFile = xmlDocs.emplace_back();
	auto fullMapPath = QFileInfo(_regTemp.sourceBookmapFile).absolutePath() + "/" + mapHref.c_str();
	pugi::xml_parse_result resultMap = mapFile.load_file(_regTemp.sourceMapFile.toStdString().c_str());

	_keysValues = GetKeyDefsAndValues(mapFile);

	_topicHrefs = GetTopicHrefs(mapFile);

	std::cout << _regTemp.sourceMapFile.toStdString().c_str() << std::endl;

	processTopics();
}

void XmlData::processTopics()
{
	RegulatoryTemplate _regTemp;
	for (const auto& href : _topicHrefs) // fancy ranged for-loop
	{
		//get the topic file.
		auto& topicFile = xmlDocs.emplace_back();
		auto fullTopicPath = QFileInfo(_regTemp.sourceBookmapFile).absolutePath() + "/" + std::string(href).c_str();
		pugi::xml_parse_result resultTopic = topicFile.load_file(fullTopicPath.toStdString().c_str());
		DitaConvertTags::convert(topicFile);
		auto _propResult = topicFile.select_nodes(".//*[@props]");
		for (auto& propResult : _propResult)
		{
			auto propsNode = propResult.node();
			auto propName = propsNode.attribute("props").value();

			auto& propsRow = _propsRows[propName]; // make new or find existing (note that the & is VERY important)
			propsRow.propsName = propName; // we may be re-assigning the name but that's ok
			propsRow.propsNodes.push_back(propsNode);

			//add all keyrefs inside this node
			auto keyrefResult = propsNode.select_nodes(".//*[@auto_keyref]");
			for (auto& propChild : keyrefResult)
			{
				propsRow._keysList.push_back(propChild.node().attribute("keyref").value());
			}
		}
	}
}
