#include "LanguageDialog.h"
#include <QMessageBox>
QStringList LanguageDialog::fullLanguageList;
QStringList LanguageDialog::selectedLanguageList;

LanguageDialog::LanguageDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    if (fullLanguageList.isEmpty())
    {
        fullLanguageList.append("German");
        fullLanguageList.append("French");
        fullLanguageList.append("Italian");
        fullLanguageList.append("Japanese");
        fullLanguageList.append("Korean");
        fullLanguageList.append("Swedish");
        fullLanguageList.append("Simplified Chinese");
        fullLanguageList.append("Traditional Chinese");
        fullLanguageList.append("Thai");
        fullLanguageList.append("Croatian");
        fullLanguageList.append("Turkish");
        fullLanguageList.append("Hebrew");
        fullLanguageList.append("Arabic");
        fullLanguageList.append("Bulgarian");
        fullLanguageList.append("Hungarian");
        fullLanguageList.append("Spanish");
        fullLanguageList.append("Portuguese");
        fullLanguageList.append("Polish");
        fullLanguageList.append("Russian");
    }

    //TODO future: pull languages from a text file

    fullLanguageList.sort();
    addLanguages();
    createConnections();
}

void LanguageDialog::addLanguages()
{
    ui.language_list->addItems(fullLanguageList);

    QListWidgetItem* item = 0;
    for (int i = 0; i < ui.language_list->count(); ++i) {
        item = ui.language_list->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        //TODO if item is listed in saved bookmap, setCheckState = checked, else unchecked
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
    auto list = ui.language_list;
    for (int i = 0; i < list->count(); ++i)
    {
        QListWidgetItem* item = list->item(i);
        if (item->checkState() == Qt::Checked)
        {
            selectedLanguageList.append(item->text());
        }
    }
    QMessageBox::StandardButton ret;
    ret = QMessageBox::information(this, tr("Languages Updated"),
        tr("Make sure to save your changes."),
        QMessageBox::Ok);
    return accept();
}

QStringList LanguageDialog::savedLanguages()
{
    return selectedLanguageList;
}

LanguageDialog::~LanguageDialog()
{}
