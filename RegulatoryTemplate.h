#pragma once

#include <QtWidgets/QMainWindow>
#include "XmlData.h"
#include "ui_RegulatoryTemplate.h"
#include "qwidget.h"

class RegulatoryTemplate : public QMainWindow
{
    Q_OBJECT

public:
    RegulatoryTemplate(QWidget *parent = Q_NULLPTR);
    QString bookFile;
    QString ditavalFile;
    QString mapFile;
    pugi::xml_document bookDoc;
    pugi::xml_document valDoc;
    pugi::xml_document mapDoc;

private slots:
    void enableDisableContent(bool checked);
    void prodnameEdit(const QString& metadata);
    void partnumEdit(const QString& metadata);
    void monthEdit(const QString& metadata);
    void yearEdit(const QString& metadata);
    void revisionEdit(const QString& metadata);
    void updateKeyref(const QString& metadata);
    void fileOpen();
    void loadSource();
    
    //void thingClicked(const QString& selection);
    //void thingClicked(int row);

private:
    Ui::RegulatoryTemplateClass ui;
    std::vector<std::string> lines;
    std::unique_ptr<XmlData> _xmlData;
};
