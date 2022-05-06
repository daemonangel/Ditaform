#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include "pugixml.hpp"
#include <iostream>
#include "PropRow.h"
#include <format>
#include <QDateTime>
#include "LoadDialog.h"
#include "SaveDialog.h"
#include "FileDownloader.h"

QString RegulatoryTemplate::bookFile;
QString RegulatoryTemplate::ditavalFile;
QString RegulatoryTemplate::mapFile;
QString RegulatoryTemplate::sourceBookmapFile;
QString RegulatoryTemplate::sourceDitavalFile;
QString RegulatoryTemplate::sourceMapFile;
pugi::xml_document bookDoc;
pugi::xml_document valDoc;
pugi::xml_document mapDoc;

RegulatoryTemplate::RegulatoryTemplate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

const class SignalBlocker
{
private:
    QObject* _widget;
public:
    SignalBlocker(QObject* widget)
    {
        _widget = widget;
        _widget->blockSignals(true);
    }
    ~SignalBlocker()
    {
        _widget->blockSignals(false);
    }
};

void RegulatoryTemplate::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
    //TODO future: Only ask the user if they want to save if they have unsaved changes
}

bool RegulatoryTemplate::maybeSave()
{
    //Save = Save, Cancel = return to app, Discard = close app without changes
    //If you Save, but then cancel without saving, you return to the app
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Ditaform"),
        tr("<b>Save files now?</b>\nUnsaved changes will be lost."),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    else
        return true;
}
//TODO future : make a button that exports versions of the document with translation info, like language codes

void RegulatoryTemplate::enableDisableContent(bool checked)
{
    if (checked)
    {
        auto result = QInputDialog::getText(this, "Whatever", "Yea Ok.");
        lines.push_back(result.toStdString());
    }
    else
    {
        auto result = QInputDialog::getText(this, "Whatever", "No not happening");
        lines.push_back(result.toStdString());
    }
}

void RegulatoryTemplate::fileLoad()
{
    auto openDialog = new LoadDialog(this);
    openDialog->open();
    connect(openDialog, &QDialog::accepted, this, &RegulatoryTemplate::loadSource);
}

void RegulatoryTemplate::fileNew()
{
    //FileDownloader *downloader = new FileDownloader(this);
}

void RegulatoryTemplate::fileSaveAs()
{
    auto openDialog = new SaveDialog(this);
    openDialog->open();
    connect(openDialog, &QDialog::accepted, this, &RegulatoryTemplate::saveFiles);
}

bool RegulatoryTemplate::fileSave()
{
    if (!bookFile.isEmpty())
    {
        bookDoc.save_file(bookFile.toStdString().c_str());
        valDoc.save_file(ditavalFile.toStdString().c_str());
        mapDoc.save_file(mapFile.toStdString().c_str());
        return true;
    }
    else
    {
        fileSaveAs();
        return false;
    }
    // https://doc.qt.io/qt-5/qfilesystemwatcher.html#fileChanged
    //TODO future: Notify the user that they have unsaved changes while editing using an *. 
}

void RegulatoryTemplate::saveFiles()
{
    auto saveFiles = SaveDialog::savedFiles();

    bookFile = saveFiles.first;
    ditavalFile = saveFiles.second;

    //save a copy of the map source file at the bookmap file location
    mapFile = QFileInfo(bookFile).absolutePath() + "/" + QFileInfo(sourceMapFile).fileName();

    //save all docs
    bookDoc.save_file(bookFile.toStdString().c_str());
    valDoc.save_file(ditavalFile.toStdString().c_str());
    mapDoc.save_file(mapFile.toStdString().c_str());
}

//QString format("%1/dv-%2.ditaval");
//cppFormat = format.arg(QFileInfo(bookFile).absolutePath()).arg(QFileInfo(bookFile).baseName());
//C++ string = auto ditavalFile = std::format("{}/dv-{}.ditaval", QFileInfo(bookFile).absolutePath(), QFileInfo(bookFile).baseName());

void RegulatoryTemplate::loadSource()
{
    auto loadFiles = LoadDialog::openedFiles();
    
    sourceBookmapFile = loadFiles.first;
    sourceDitavalFile = loadFiles.second;
    
    pugi::xml_parse_result bookresult = bookDoc.load_file(sourceBookmapFile.toStdString().c_str());
    pugi::xml_parse_result valResult = valDoc.load_file(sourceDitavalFile.toStdString().c_str());
    sourceMapFile = getMapFileFromBookmap();
    pugi::xml_parse_result mapResult = mapDoc.load_file(sourceMapFile.toStdString().c_str());

    //TODO Make it so that you don't have to save the file before you can make any changes to the form
    fileSaveAs();

    _xmlData = std::make_unique<XmlData>();

    removePropRows();

    clearBookInfo();

    addPropRows();

    //connectPropRowTextChange();
}

QString RegulatoryTemplate::getMapFileFromBookmap()
{
    //get map from bookmap - only supporting one map for now
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    std::string mapHref = bookmap.child("chapter").attribute("href").value();
    sourceMapFile = QFileInfo(sourceBookmapFile).absolutePath() + "/" + mapHref.c_str();
    return sourceMapFile;
}

void RegulatoryTemplate::removePropRows()
{
    //if the row count is more than the number of premade rows, delete the dynamically created rows
    if (ui.formLayout->rowCount() > premadeRows)
    {
        auto rowCount = ui.formLayout->rowCount() - 1;
        for (int i = rowCount; i >= premadeRows; i--)
        {
            ui.formLayout->removeRow(i);
        }
    }
}

void RegulatoryTemplate::clearBookInfo()
{
    //empty text from the remaining rows
    for (auto& row : ui.centralWidget->findChildren<QLineEdit*>())
    {
        row->setText("");
    }
}

void RegulatoryTemplate::addPropRows()
{
    //for each prop set, add a row to the ui
    for (auto& propRow : _xmlData->_propsRows)
    {
        auto propUI = new PropRow(*propRow, ui.centralWidget);
        ui.formLayout->addRow(propUI);
        connect(propUI, &PropRow::updateAllOtherKeyrefs, this, &RegulatoryTemplate::autoUpdateDupKeyrefs);
    }
}

/*void RegulatoryTemplate::connectPropRowTextChange()
{
    //connect all QTextEdit textChange signals to autoUpdateDupKeyrefs slot
    auto keyrefs = ui.centralWidget->findChildren<QTextEdit*>();
    for (auto& key : keyrefs)
    {
        connect(key, &QTextEdit::textChanged, this, &RegulatoryTemplate::autoUpdateDupKeyrefs);
    }
}*/

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    prodnameNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::partnumEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto partnumNode = bookmap.child("bookmeta").child("bookid").child("bookpartno");
    partnumNode.text().set(ui.partnumber_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::monthEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto monthNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("month");
    monthNode.text().set(ui.month_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::yearEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto yearCompletedNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("year");
    auto yearCopyrightNode = bookmap.child("bookmeta").child("bookrights").child("copyrlast").child("year");
    yearCompletedNode.text().set(ui.year_edit->text().toStdString().c_str());
    yearCopyrightNode.text().set(ui.year_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::revisionEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto revisionNode = bookmap.child("bookmeta").child("bookid").child("volume");
    revisionNode.text().set(ui.revision_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::autoUpdateDupKeyrefs(const QString& senderName, const QString& senderText)
{  
    /* if findChildren slows the form down, can put all textedits in a vector, OR
    * go through each row and use a premade vector of textedits to make changes as needed (see proprow //_keyRefTextEdits.push_back(input);)
    * 
    * for(auto& propRow : _propRows)
    {
        propRow.UpdateKeyRefTextForKeyNameOrSomething(senderName, senderText);
    }
    */ 
    auto keyrefs = ui.centralWidget->findChildren<QTextEdit*>(senderName);
    /*
        If there is only one, that was the one that sent us this signal, and we don't need to change it
        (we ignore the fact that we could be updating the one that sent us this signal, too much trouble to skip for now)
    */
    if (keyrefs.size() > 1)
    {
        for (auto& key : keyrefs)
        {
            //temporarily turn off the signal from key text box
            SignalBlocker blocker(key);
            auto keyName = key->objectName();
            if (senderName == key->objectName())
            {
                key->clear();
                key->insertPlainText(senderText);
            }
        }
    };   
}

//TODO add an about box with app info, Qt, and PugiXml