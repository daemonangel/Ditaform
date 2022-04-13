#pragma once

#include <QtWidgets/QMainWindow>
#include "XmlData.h"
#include "ui_RegulatoryTemplate.h"
#include "qwidget.h"
#include "qinputdialog.h"
#include <QtGui>

extern pugi::xml_document mapDoc;
extern pugi::xml_document valDoc;

class RegulatoryTemplate : public QMainWindow
{
    Q_OBJECT
    
public:
    RegulatoryTemplate(QWidget *parent = Q_NULLPTR);
    static QString sourceBookmapFile;
    static QString sourceMapFile;
    static QString sourceDitavalFile;

private slots:
    void enableDisableContent(bool checked);
    void prodnameEdit(const QString& metadata);
    void partnumEdit(const QString& metadata);
    void monthEdit(const QString& metadata);
    void yearEdit(const QString& metadata);
    void revisionEdit(const QString& metadata);
    bool fileSave();
    void fileSaveAs();
    void fileLoad();
    void autoUpdateDupKeyrefs();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::RegulatoryTemplateClass ui;
    std::vector<std::string> lines;
    std::unique_ptr<XmlData> _xmlData;
    static QString bookFile;
    static QString ditavalFile;
    static QString mapFile;
    static pugi::xml_document bookDoc;
    int premadeRows = 5; //number of premade form rows

    bool maybeSave();
    void loadSource();
    void clearBookInfo();
    void removePropRows();
    void addPropRows();
    void connectPropRowTextChange();
};
