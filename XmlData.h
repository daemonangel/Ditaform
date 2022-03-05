#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>
#include "qtextbrowser.h"

struct propValueCollection
{
	using ditaval_node = pugi::xml_node;
	using topic_node = pugi::xml_node;

	std::string propsName;
	std::vector<ditaval_node> propsNode;
	std::vector<std::pair<topic_node, std::string>> _keysList; // node and name for each key found inside a props
};

class XmlData : public QWidget
{
	Q_OBJECT

public:
	XmlData();
	void ProcessData();
	QTextBrowser* contentPreview_browser;
};