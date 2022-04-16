#pragma once

#include <QDialog>
#include "ui_SaveDialog.h"
#include "qstring.h"

class SaveDialog : public QDialog
{
	Q_OBJECT

public:
	SaveDialog(QWidget *parent = Q_NULLPTR);
	static std::pair<QString, QString> savedFiles();
	~SaveDialog();

private:
	Ui::SaveDialog ui;
	static QString bookmapFile;
	static QString ditavalFile;

private slots:
	void saveBookmap();
	void saveDitaval();
};
