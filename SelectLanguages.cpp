#include "SelectLanguages.h"
#include <qpushbutton.h>
#include <qlayout.h>
QStringList SelectLanguages::strList;

SelectLanguages::SelectLanguages(QWidget* parent) 
    : QDialog(parent)
{
    setWindowTitle("Select Languages");
    if (strList.isEmpty())
    {
        strList.append("German");
        strList.append("French");
        strList.append("Italian");
        strList.append("Japanese");
        strList.append("Korean");
        strList.append("Swedish");
        strList.append("Traditional Chinese");
        strList.append("Turkish");
    }
    
    //TODO: make the list a static const vector here and remove from creatListWidget()
    //TODO future: if the list is empty, read a text file to fill it in , push_back or append
    createListWidget();
    //createOtherWidgets();
    //createLayout();
    createConnections();
}

void SelectLanguages::createListWidget() {
    widget = new QListWidget(this); //remove

    widget->addItems(strList); //remove 
    auto centralWidget = new QWidget(this); //remove
    auto layout = new QVBoxLayout(centralWidget); //remove
    layout->addWidget(widget); //remove

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