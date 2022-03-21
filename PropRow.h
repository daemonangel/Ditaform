#pragma once

#include <QWidget>
#include "ui_PropRow.h"
#include "XmlData.h"
#include <QLabel>

class PropRow : public QWidget
{
	Q_OBJECT

public:
	PropRow(const propValueCollection& propsRow, QWidget *parent = Q_NULLPTR);
	~PropRow();

private:
	Ui::PropRow ui;
	const propValueCollection& _myPropsRow;
};
