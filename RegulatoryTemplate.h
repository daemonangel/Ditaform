#pragma once

#include <QtWidgets/QMainWindow>
#include "XmlData.h"
#include "ui_RegulatoryTemplate.h"
#include "qwidget.h"

extern pugi::xml_document mapDoc;
extern pugi::xml_document valDoc;

class RegulatoryTemplate : public QMainWindow
{
    Q_OBJECT
    
public:
    RegulatoryTemplate(QWidget *parent = Q_NULLPTR);

private slots:
    void enableDisableContent(bool checked);
    void prodnameEdit(const QString& metadata);
    void partnumEdit(const QString& metadata);
    void monthEdit(const QString& metadata);
    void yearEdit(const QString& metadata);
    void revisionEdit(const QString& metadata);
    void fileSave();
    void fileSaveAs();
    void loadSource();
    void autoUpdateDupKeyrefs();

private:
    Ui::RegulatoryTemplateClass ui;
    std::vector<std::string> lines;
    std::unique_ptr<XmlData> _xmlData;
    QString bookFileSave;
    static QString bookFile;
    static QString ditavalFile;
    static QString mapFile;
    static pugi::xml_document bookDoc;
    int premadeRows = 5; //number of premade form rows
};
