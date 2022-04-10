#pragma once

#include <QWidget>
#include "ui_PropRow.h"
#include "XmlData.h"
#include <QLabel>
#include "RegulatoryTemplate.h"

class PropRow : public QWidget
{
	Q_OBJECT

public:
	PropRow(const propValueCollection& propsRow, QWidget *parent = Q_NULLPTR);
	~PropRow();

private slots:
	void updateKeyref();
	void updateDitaval();

private:
	const int* i;
	Ui::PropRow ui;
	const propValueCollection& _myPropsRow;
	std::unique_ptr<RegulatoryTemplate> _RegulatoryTemplate;
	
	void insertKeyrefInput(const pugi::xml_node&);
};
