#pragma once

#include <QDialog>
#include "ui_LoadDialog.h"
#include "qstring.h"

class LoadDialog : public QDialog
{
	Q_OBJECT

public:
	LoadDialog(QWidget *parent = Q_NULLPTR);
	static std::pair<QString, QString> openedFiles();
	~LoadDialog();

private:
	Ui::LoadDialog ui;
	static QString bookmapFile;
	static QString ditavalFile;

private slots:
	void openBookmap();
	void openDitaval();
};
