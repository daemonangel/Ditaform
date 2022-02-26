#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>

class FormatData : public QWidget
{
	Q_OBJECT

public:
	FormatData();

	QGroupBox* contentUi_group(std::vector<std::pair<pugi::xml_node, std::string>> keysList);
	void contentPreview();

	QTextBrowser* contentPreview_browser;
};