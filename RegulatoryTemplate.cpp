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
#include <filesystem>
#include "LanguageDialog.h"
#include "Xml.h"

QString RegulatoryTemplate::saveBookFile;
QString RegulatoryTemplate::saveDitavalFile;
QString RegulatoryTemplate::saveMapFile;
QString RegulatoryTemplate::tempBook;
QString RegulatoryTemplate::tempDitaval;
QString RegulatoryTemplate::tempMap;
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

void RegulatoryTemplate::addPropRows()
{
    //for each prop set, add a row to the ui
    for (auto& propRow : _xmlData->_propsRows)
    {
        auto propUI = new PropRow(*propRow, ui.centralWidget);
        ui.formLayout->addRow(propUI);
        
        auto rowName = propUI->propRowName().c_str();
        auto val = valDoc.child("val");
        auto valNode = val.find_child_by_attribute("val", rowName);
        std::string actionValue = valNode.attribute("action").value();

        if (actionValue == "include")
        {
            propUI->findChild<QCheckBox*>(rowName)->setChecked(true);
        }

        connect(propUI, &PropRow::updateKeyrefs, this, &RegulatoryTemplate::autoUpdateDupKeyrefs);
        connect(propUI, &PropRow::updateCheckboxes, this, &RegulatoryTemplate::autoUpdateCheckboxes);
    }
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
    const static QString OkColor{ "background-color:#ffffff;color:#000000" };
    const static QString WarningColor{ "background-color:yellow;color:#000000" };

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
            if (senderName == key->objectName())
            {
                key->clear();
                key->insertPlainText(senderText);
                //key->setStyleSheet(key->parentWidget()->findChild<QCheckBox*>()->isChecked() ? OkColor : WarningColor);
            }
        }
    }
    else 
    { 
        //need to clear the data from single keyref boxes also, otherwise first letter stays gray
        SignalBlocker blocker(keyrefs.first()); 
        keyrefs.first()->clear(); 
        keyrefs.first()->insertPlainText(senderText);
    };
}

void RegulatoryTemplate::autoUpdateCheckboxes(const QString& senderName)
{
    //TODO recommend breaking up this function into smaller functions for semantics

    const static QString OkColor { "background-color:#f0f0f0;" };
    const static QString WarningColor { "background-color:yellow;" };

    // find all checked boxes
    auto checkboxes = ui.centralWidget->findChildren<QCheckBox*>();
    std::vector<std::string> checked;
    for (const auto box : checkboxes)
    {
        if (box->isChecked())
        {
            checked.push_back(box->objectName().toStdString());
        }
    }

    //use senderName to search _dataNodes for related checkboxes
    for (auto& node : _xmlData->_dataNodes)
    {
        //PARENT NODE
        if (senderName.toStdString() == node.parent) 
        {
            QCheckBox* parentCheckbox = ui.centralWidget->findChild<QCheckBox*>(senderName);

            if (parentCheckbox->isChecked())
            {
                //if any child is checked, unhighlight this parent row
                parentCheckbox->setStyleSheet(node.isAnyChild(checked) ? OkColor : WarningColor);
            }
            else
            {
                //if no children are checked, unhighlight this parent row
                parentCheckbox->setStyleSheet(!node.isAnyChild(checked) ? OkColor : WarningColor);
            }
        }
        //CHILD NODE
        else if (node.isChild(senderName.toStdString())) 
        {      
            QCheckBox* childCheckbox = ui.centralWidget->findChild<QCheckBox*>(senderName);
            QCheckBox* parentCheckbox = ui.centralWidget->findChild<QCheckBox*>(node.parent.c_str());

            if (!childCheckbox->isChecked())
            {
                //if any child is checked, unhighlight this parent
                parentCheckbox->setStyleSheet(node.isAnyChild(checked) ? OkColor : WarningColor);
            }
            else if (node.rule == data_node::RuleType::One) //make sure only one child is checked
            {
                //uncheck siblings
                for (auto& child : node.children)
                {
                    if (child != senderName.toStdString())
                    {
                        ui.centralWidget->findChild<QCheckBox*>(child.c_str())->setChecked(false);
                    }
                }

                //if parent is checked, unhighlight the parent
                parentCheckbox->setStyleSheet(parentCheckbox->isChecked() ? OkColor : WarningColor);
            }
            else if (node.rule == data_node::RuleType::Any)
            {
                //if parent is checked, unhighlight the parent
                parentCheckbox->setStyleSheet(parentCheckbox->isChecked() ? OkColor : WarningColor);
            }
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

void RegulatoryTemplate::createDitaval()
{
    //if the doc already has info, reset it
    if (!valDoc.empty())
    {
        valDoc.reset();
    }
    pugi::xml_node val = valDoc.append_child("val");
    for (auto& propRow : _xmlData->_propsRows)
    {
        auto node = Xml::CreateNode(val, "prop", "");
        auto name = propRow->propsName;
        Xml::CreateAttrib(node, "att", "props");
        Xml::CreateAttrib(node, "val", name);
        Xml::CreateAttrib(node, "action", "exclude");
    }
}

void RegulatoryTemplate::dateEdit([[maybe_unused]] const QDate& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto monthNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("month");
    monthNode.text().set(ui.date_edit->date().month());

    auto yearCompletedNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("year");
    auto yearCopyrightNode = bookmap.child("bookmeta").child("bookrights").child("copyrlast").child("year");
    yearCompletedNode.text().set(ui.date_edit->date().year());
    yearCopyrightNode.text().set(ui.date_edit->date().year());

    bookDoc.save_file(RegulatoryTemplate::saveBookFile.toStdString().c_str());
}

void RegulatoryTemplate::deleteTempFiles()
{
    std::filesystem::remove(tempBook.toStdString());
    std::filesystem::remove(tempDitaval.toStdString());
    std::filesystem::remove(tempMap.toStdString());
}

//QString format("%1/dv-%2.ditaval");
//cppFormat = format.arg(QFileInfo(saveBookFile).absolutePath()).arg(QFileInfo(saveBookFile).baseName());
//C++ string = auto saveDitavalFile = std::format("{}/dv-{}.ditaval", QFileInfo(saveBookFile).absolutePath(), QFileInfo(saveBookFile).baseName());

void RegulatoryTemplate::fileNew()
{
    //FileDownloader *downloader = new FileDownloader(this);

    sourceBookmapFile = "maps/template.ditamap";
    valDoc.reset(); //reset the file since we will create it automatically
    loadSource();
}

void RegulatoryTemplate::fileOpen()
{
    auto openDialog = new LoadDialog(this);
    openDialog->open();
    connect(openDialog, &QDialog::accepted, this, &RegulatoryTemplate::openSource);
}

bool RegulatoryTemplate::fileSave()
{
    if (!saveBookFile.isEmpty())
    {
        bookDoc.save_file(saveBookFile.toStdString().c_str());
        valDoc.save_file(saveDitavalFile.toStdString().c_str());
        mapDoc.save_file(saveMapFile.toStdString().c_str());
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

void RegulatoryTemplate::fileSaveAs()
{
    auto openDialog = new SaveDialog(this);
    openDialog->open();
    connect(openDialog, &QDialog::accepted, this, &RegulatoryTemplate::saveFiles);
}

QString RegulatoryTemplate::getMapFileFromBookmap()
{
    //get map from bookmap - only supporting one map for now
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    std::string mapHref = bookmap.child("chapter").attribute("href").value();
    sourceMapFile = QFileInfo(sourceBookmapFile).absolutePath() + "/" + mapHref.c_str();
    return sourceMapFile;
}

void RegulatoryTemplate::helpAbout()
{
    if (QFile inputFile("readme.md"); inputFile.exists())
    {
        QString text;

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                text.append(line);
                text.append("<br/>");
            }
            inputFile.close();
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle("About Ditaform");
        msgBox.setText(text);
        msgBox.exec();
    }
    else
    {
        QMessageBox::warning(this, tr("IMPORTANT!"),
            tr("Readme.md is missing."),
            QMessageBox::Ok);
    }
}

void RegulatoryTemplate::helpDitaform()
{
    if (QFile inputFile("help.txt"); inputFile.exists())
    {
        QString text;

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                text.append(line);
                text.append("<br/>");
            }
            inputFile.close();
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle("Ditaform Help");
        msgBox.setText(text);
        msgBox.exec();
    }
    else
    {
        QMessageBox::warning(this, tr("IMPORTANT!"),
            tr("help.txt is missing."),
            QMessageBox::Ok);
    }
}

void RegulatoryTemplate::languagesEdit()
{
    QStringList selectedLanguages; //fill this in from bookdoc node

    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto languages = bookmap.child("bookmeta").find_child_by_attribute("type", "languages");
    for (auto& child : languages)
    {
        selectedLanguages.append(child.child_value());
    }

    auto languagesDialog = new LanguageDialog(selectedLanguages, this);
    languagesDialog->open();
    connect(languagesDialog, &QDialog::accepted, this, &RegulatoryTemplate::updateLanguages);
}

void RegulatoryTemplate::loadSource()
{
    //map and bookmap set to parse_full to avoid losing DOCTYPE
    bookDoc.load_file(sourceBookmapFile.toStdString().c_str(), pugi::parse_full);
    sourceMapFile = getMapFileFromBookmap();
    mapDoc.load_file(sourceMapFile.toStdString().c_str(), pugi::parse_full);

    _xmlData = std::make_unique<XmlData>();

    //if the doc doesn't exist or has no top level element, create it dynamically
    if (!valDoc.document_element())
    {
        createDitaval();
    }

    saveTempFiles();
    removePropRows();
    clearBookInfo();
    addPropRows();

    if (!ui.prodname_edit->isEnabled())
    {
        ui.prodname_edit->setEnabled(true);
        ui.requestorname_edit->setEnabled(true);
        ui.date_edit->setEnabled(true);
        ui.languages_button->setEnabled(true);
        ui.date_edit->setDate(QDate::currentDate());
    }
}

bool RegulatoryTemplate::maybeSave()
{
    //Save = Save, Cancel = return to app, Discard = close app without changes
    //If you Save, but then cancel without saving, you return to the app
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Ditaform"),
        tr("Save files now? Unsaved changes will be lost."),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    else
        deleteTempFiles(); //give a hoot don't pollute
    return true;
}
//TODO future : make a button that exports versions of the document with translation info, like language codes

void RegulatoryTemplate::openSource()
{
    auto loadFiles = LoadDialog::openedFiles();

    //when opening custom files, the source and save files are the same
    sourceBookmapFile = loadFiles.first;
    saveBookFile = loadFiles.first;
    sourceDitavalFile = loadFiles.second;
    saveDitavalFile = loadFiles.second;

    valDoc.load_file(sourceDitavalFile.toStdString().c_str());

    loadSource();
}

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    prodnameNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::saveBookFile.toStdString().c_str());
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

void RegulatoryTemplate::requestornameEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto requestorNode = bookmap.child("bookmeta").child("author");
    requestorNode.text().set(ui.requestorname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::saveBookFile.toStdString().c_str());
}

void RegulatoryTemplate::saveFiles()
{
    auto saveFiles = SaveDialog::savedFiles();

    saveBookFile = saveFiles;
    
    //save a copy of the map source file at the bookmap file location
    saveMapFile = QFileInfo(saveBookFile).absolutePath() + "/" + QFileInfo(saveBookFile).baseName() + "-map.ditamap";
    saveDitavalFile = QFileInfo(saveBookFile).absolutePath() + "/" + QFileInfo(saveBookFile).baseName() + ".ditaval";

    //update bookmap chapter href to saveMapFile
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    bookmap.child("chapter").attribute("href").set_value(QFileInfo(saveMapFile).fileName().toStdString().c_str());
    
    //save all docs
    bookDoc.save_file(saveBookFile.toStdString().c_str());
    valDoc.save_file(saveDitavalFile.toStdString().c_str());
    mapDoc.save_file(saveMapFile.toStdString().c_str());

    //delete temp files since we don't need them anymore
    deleteTempFiles();
}

void RegulatoryTemplate::saveTempFiles()
{
    tempBook = QDir::tempPath() + "/" + QFileInfo(sourceBookmapFile).fileName();
    tempDitaval = QDir::tempPath() + "/dv-" + QFileInfo(sourceBookmapFile).baseName() + ".ditaval";
    
    //save a copy of the map source file at the bookmap file location
    tempMap = QDir::tempPath() + "/m-" + QFileInfo(sourceBookmapFile).fileName();

    //save all docs
    bookDoc.save_file(tempBook.toStdString().c_str());
    valDoc.save_file(tempDitaval.toStdString().c_str());
    mapDoc.save_file(tempMap.toStdString().c_str());
}

void RegulatoryTemplate::updateLanguages()
{
    auto savedLanguages = LanguageDialog::savedLanguages();
    
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto metadata = bookmap.child("bookmeta");

    //delete <data type="languages"> if it exists
    auto languages = metadata.find_child_by_attribute("type", "languages");
    metadata.remove_child(languages);

    auto dataNode = Xml::CreateNode(metadata, "data", "");
    Xml::CreateAttrib(dataNode, "type", "languages");

    for (auto& item : savedLanguages)
    {
        Xml::CreateNode(dataNode, "data", item.toStdString());
    }
}