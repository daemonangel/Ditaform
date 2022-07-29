#include "LanguageDialog.h"
QStringList LanguageDialog::strList;

LanguageDialog::LanguageDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    if (strList.isEmpty())
    {
        strList.append("German");
        strList.append("French");
        strList.append("Italian");
        strList.append("Japanese");
        strList.append("Korean");
        strList.append("Swedish");
        strList.append("Simplified Chinese");
        strList.append("Traditional Chinese");
        strList.append("Thai");
        strList.append("Croatian");
        strList.append("Turkish");
        strList.append("Hebrew");
        strList.append("Arabic");
        strList.append("Bulgarian");
        strList.append("Hungarian");
        strList.append("Spanish");
        strList.append("Portuguese");
        strList.append("Polish");
        strList.append("Russian");
    }

    //TODO future: pull languages from a text file

    strList.sort();
    addLanguages();
    createConnections();
}

void LanguageDialog::addLanguages()
{
    ui.language_list->addItems(strList);

    QListWidgetItem* item = 0;
    for (int i = 0; i < ui.language_list->count(); ++i) {
        item = ui.language_list->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void LanguageDialog::createConnections() {
    QObject::connect(ui.language_list, &QListWidget::itemChanged,
        this, &LanguageDialog::highlightChecked);
    QObject::connect(ui.save_button, &QPushButton::clicked, this, &LanguageDialog::save);
}

void LanguageDialog::highlightChecked(QListWidgetItem* item) {
    if (item->checkState() == Qt::Checked)
        item->setBackground(QColor("#ffffb2"));
    else
        item->setBackground(QColor("#ffffff"));
}

void LanguageDialog::save()
{
    //TODO future: saving creates a set of bookmaps with the correct language codes


}

LanguageDialog::~LanguageDialog()
{}
