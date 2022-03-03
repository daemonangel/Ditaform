#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>

struct propValueCollection
{
	std::string propsName;
	pugi::xml_node propsNode;
	std::vector<std::pair<pugi::xml_node, std::string>> _keysList; // node and name for each key found inside a props
};

class XmlData : public QWidget
{
	Q_OBJECT

public:
	XmlData();
	void ProcessData();
	QTextBrowser* contentPreview_browser;
};