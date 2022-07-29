#pragma once
#include <QDialog>
#include "qlistwidget.h"
#include "qdialogbuttonbox.h"
#include "qgroupbox.h"

class SelectLanguages : public QDialog
{
	Q_OBJECT

public:
	SelectLanguages(QWidget* parent = 0);
public slots:
	void highlightChecked(QListWidgetItem* item);
	void save();
private:
    QListWidget* widget;
    QDialogButtonBox* buttonBox;

    void createListWidget();
    //void createOtherWidgets();
    //void createLayout();
    void createConnections();
};

