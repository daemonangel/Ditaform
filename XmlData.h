#pragma once
#include <QDialog>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <ui_RegulatoryTemplate.h>

class XmlData : public QDialog
{
	Q_OBJECT

public:
	XmlData(QWidget* parent = Q_NULLPTR);
	bool _action;
	std::string _contentPreview;
	std::vector<std::pair<pugi::xml_node, std::string>> _keysList;

private:
	QScrollArea *form;
	QCheckBox *action_checkbox;
	QLabel *keydef_label;
	QLineEdit *keydef_edit;
	QTextBrowser *contentPreview_browser;
};