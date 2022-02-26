#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>

class XmlData : public QWidget
{
	Q_OBJECT

public:
	XmlData();
	std::vector<std::pair<pugi::xml_node, std::string>> _keysList;
	QString _prop;
	QTextBrowser* contentPreview_browser;
};