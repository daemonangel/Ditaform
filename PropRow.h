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
	bool isIncluded() const;
	const std::string& propRowName() const;
	~PropRow();

private slots:
	void updateKeyref();
	void updateDitavalAndCheckboxes();

signals:
	void updateKeyrefs(const QString&, const QString&);
	void updateCheckboxes(const QString&);

private:
	const int* i;
	Ui::PropRow ui;
	const propValueCollection& _myPropsRow;
	std::unique_ptr<RegulatoryTemplate> _RegulatoryTemplate;
	
	void insertKeyrefInput(const pugi::xml_node&);
};
