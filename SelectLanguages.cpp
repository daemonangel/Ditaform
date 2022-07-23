#include "SelectLanguages.h"
#include <qpushbutton.h>

SelectLanguages::SelectLanguages(QWidget* parent) 
    : QDialog(parent)
{
    setWindowTitle("Select Languages");

    createListWidget();
    //createOtherWidgets();
    //createLayout();
    createConnections();
}

void SelectLanguages::createListWidget() {
    widget = new QListWidget;
    QStringList strList;
    strList << "German" << "French" << "Italian" << "Japanese"
        << "Korean" << "Swedish" << "Traditional Chinese" << "Turkish";

    widget->addItems(strList);

    QListWidgetItem* item = 0;
    for (int i = 0; i < widget->count(); ++i) {
        item = widget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    //buttonBox->addButton(QDialogButtonBox::Save);
    //buttonBox->addButton(QDialogButtonBox::Cancel);
}

void SelectLanguages::createConnections() {
    QObject::connect(widget, &QListWidget::currentItemChanged,
        this, &SelectLanguages::highlightChecked);
    //QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectLanguages::save);
    //QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectLanguages::close);
}

void SelectLanguages::highlightChecked(QListWidgetItem* item) {
    if (item->checkState() == Qt::Checked)
        item->setBackground(QColor("#ffffb2"));
    else
        item->setBackground(QColor("#ffffff"));
}

void SelectLanguages::save() {

}