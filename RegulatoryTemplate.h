#pragma once

#include <QtWidgets/QMainWindow>
#include "XmlData.h"
#include "ui_RegulatoryTemplate.h"
#include "qwidget.h"
#include "qinputdialog.h"
#include <QtGui>

extern pugi::xml_document mapDoc;
extern pugi::xml_document valDoc;
extern pugi::xml_document bookDoc;

class RegulatoryTemplate : public QMainWindow
{
    Q_OBJECT
    
public:
    RegulatoryTemplate(QWidget *parent = Q_NULLPTR);
    static QString sourceBookmapFile;
    static QString sourceDitavalFile;
    static QString sourceMapFile;

private slots:
    void autoUpdateDupKeyrefs(const QString& senderName, const QString& senderText);
    void dateEdit(const QDate& metadata);
    bool fileSave();
    void fileSaveAs();
    void fileOpen();
    void fileNew();
    void helpDitaform();
    void helpAbout(); 
    void languagesEdit();
    void openSource();
    void prodnameEdit(const QString& metadata);
    void requestornameEdit(const QString& metadata);
    void saveFiles();

protected:
    void closeEvent(QCloseEvent* event);

private:
    int premadeRows = 5; //number of premade form rows

    Ui::RegulatoryTemplateClass ui;
    std::vector<std::string> lines;
    std::unique_ptr<XmlData> _xmlData;
    static QString bookFile;
    static QString ditavalFile;
    static QString mapFile;
    static QString tempBook;
    static QString tempDitaval;
    static QString tempMap;

    void addPropRows();
    void clearBookInfo();
    void createDitaval();
    void deleteTempFiles();
    QString getMapFileFromBookmap();
    bool maybeSave();
    void loadSource();
    void removePropRows();
    void saveTempFiles();
    void updateLanguages();
};
