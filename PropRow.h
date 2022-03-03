#pragma once

#include <QWidget>
#include "ui_PropRow.h"

class PropRow : public QWidget
{
	Q_OBJECT

public:
	PropRow(QWidget *parent = Q_NULLPTR);
	~PropRow();

private:
	Ui::PropRow ui;
};
