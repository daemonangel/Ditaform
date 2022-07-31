#include "LanguageDialog.h"
#include <QMessageBox>
#include <qfile.h>
#include <qtextstream.h>

QStringList LanguageDialog::fullLanguageList;
QStringList LanguageDialog::selectedLanguageList;

LanguageDialog::LanguageDialog(const QStringList& selectedLanguages, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    if (fullLanguageList.isEmpty())
    {
        loadLanguages();
    }

    addLanguages(selectedLanguages);
    createConnections();
}

void LanguageDialog::loadLanguages()
{
    if (QFile inputFile("source/languages.txt"); inputFile.exists())
    {
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                fullLanguageList.append(line);
            }
            inputFile.close();
            fullLanguageList.sort();
        }
    }
    else
    {
       QMessageBox::information(this, tr("Language List"),
            tr("languages.txt cannot be found. Check if it exists in the source folder."),
            QMessageBox::Ok);
        ui.save_button->setEnabled(false);
        ui.language_list->addItem("[languages.txt not found]");
    }
}

void LanguageDialog::addLanguages(const QStringList& selectedLanguages)
{
    ui.language_list->addItems(fullLanguageList);

    QListWidgetItem* item = 0;
    for (int i = 0; i < ui.language_list->count(); ++i) {
        item = ui.language_list->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        //if selectedLanguages.contains
        //TODO if item is listed in saved bookmap, setCheckState = checked, else unchecked
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

    return accept();
}

QStringList LanguageDialog::savedLanguages()
{
    return selectedLanguageList;
}

LanguageDialog::~LanguageDialog()
{}
