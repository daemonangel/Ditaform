#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>
#include "qtextbrowser.h"
#include <iostream>

//need a vector to hold all the structs?

struct propValueCollection //one for each prop
{
	using ditaval_node = pugi::xml_node;
	using topic_node = pugi::xml_node;

	std::string propsName;
	std::vector<ditaval_node> propsNode; //list of nodes because a props name could be on multiple nodes
	std::vector<std::string> _keysList; //from topic: list of keyref values
};

class XmlData : public QWidget
{
	Q_OBJECT

public:
	XmlData();
	void ProcessData();
	QTextBrowser* contentPreview_browser;
	int propsRow;
	std::vector<std::pair<std::string, std::string>> _keysValues; //from map: keys (keyref) value, keyword value
	std::vector<std::string> _topicHrefs; //from map: list of all the hrefs to topics
	std::vector<propValueCollection> _propsRows;
};