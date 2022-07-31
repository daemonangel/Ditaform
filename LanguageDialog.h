#pragma once

#include <QDialog>
#include <unordered_map>
#include "ui_LanguageDialog.h"

class LanguageDialog : public QDialog
{
	Q_OBJECT

public:
	LanguageDialog(const QStringList& selectedLanguages, QWidget *parent = nullptr);
	static QStringList savedLanguages();
	~LanguageDialog();

public slots:
	void highlightChecked(QListWidgetItem* item);

private:
	static QStringList fullLanguageList;
	static QStringList selectedLanguageList;
	Ui::LanguageDialogClass ui;
	void addLanguages(const QStringList& selectedLanguages);
	void loadLanguages();
	void createConnections();
	void save();
};

