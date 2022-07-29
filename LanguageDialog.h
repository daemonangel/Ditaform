#pragma once

#include <QDialog>
#include "ui_LanguageDialog.h"

class LanguageDialog : public QDialog
{
	Q_OBJECT

public:
	LanguageDialog(QWidget *parent = nullptr);
	~LanguageDialog();

public slots:
	void highlightChecked(QListWidgetItem* item);

private:
	static QStringList strList;
	Ui::LanguageDialogClass ui;
	void addLanguages();
	void createConnections();
	void save();
};
