#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include <format>
#include "pugixml.hpp"
#include <iostream>
#include "PropRow.h"

QString RegulatoryTemplate::bookFile;
QString RegulatoryTemplate::ditavalFile;
QString RegulatoryTemplate::mapFile;
pugi::xml_document RegulatoryTemplate::bookDoc;
pugi::xml_document RegulatoryTemplate::valDoc;
pugi::xml_document RegulatoryTemplate::mapDoc;

RegulatoryTemplate::RegulatoryTemplate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //ui is loaded each time loadSource() runs because of third property in connect() in PropRow.cpp
}

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

void RegulatoryTemplate::fileSave()
{
    if (!bookFile.isEmpty())
    {
        bookDoc.save_file(bookFile.toStdString().c_str());
        valDoc.save_file(ditavalFile.toStdString().c_str());
        mapDoc.save_file(mapFile.toStdString().c_str());
    }
    else
    {
        _xmlData = std::make_unique<XmlData>();

        //save a copy of the source files to user defined location/file for bookmap
        //bookmap
        bookFile = QFileDialog::getSaveFileName(this, tr("Save As..."), "bm-PRODUCT-rg-en.ditamap", tr("DITA Map (*.ditamap)"));
        pugi::xml_parse_result result = bookDoc.load_file(_xmlData->sourceBookmapFile);
        pugi::xml_node bookmap = bookDoc.child("bookmap");
        bookDoc.save_file(bookFile.toStdString().c_str());

        //ditaval
        //TODO: Update the file name to dv- and everything that comes after bm- in the bookFile name.
        ditavalFile = QFileInfo(bookFile).absolutePath() + "/dv-PRODUCT-rg-en.ditaval";
        pugi::xml_parse_result valResult = valDoc.load_file(_xmlData->sourceDitavalFile);
        valDoc.save_file(ditavalFile.toStdString().c_str());

        //map
        //TODO: Update the file name to m- and everything that comes after bm- in the bookFile name. Update the bookmap file to match.
        mapFile = QFileInfo(bookFile).absolutePath() + "/m-PRODUCT-rg-en.ditamap";
        pugi::xml_parse_result mapResult = mapDoc.load_file(_xmlData->sourceMapFile);
        mapDoc.save_file(mapFile.toStdString().c_str());
    }
    
}

void RegulatoryTemplate::loadSource()
{
    _xmlData = std::make_unique<XmlData>();

    //TODO - if propsRows already exist, delete them before loading

    //for each prop set, add a row to the ui
    for (auto& propRow : _xmlData->_propsRows)
    {
        auto propUI = new PropRow(propRow.second, ui.centralWidget);
        ui.formLayout->addRow(propUI);
    }

    //maybe instead of connecting PropRow/input signal to updateKeyref, after data loads find all the keyrefs and connect them in the UI?
}

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    prodnameNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(bookFile.toStdString().c_str());
}

void RegulatoryTemplate::partnumEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto partnumNode = bookmap.child("bookmeta").child("bookid").child("bookpartno");
    partnumNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::monthEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto monthNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("month");
    monthNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::yearEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto yearCompletedNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("year");
    auto yearCopyrightNode = bookmap.child("bookmeta").child("bookrights").child("copyrlast").child("year");
    yearCompletedNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    yearCopyrightNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::revisionEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    auto revisionNode = bookmap.child("bookmeta").child("bookid").child("volume");
    revisionNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    bookDoc.save_file(RegulatoryTemplate::bookFile.toStdString().c_str());
}

void RegulatoryTemplate::updateKeyref()
{
    auto senderObject = QObject::sender();
    auto senderName = senderObject->objectName();

    pugi::xml_node map = mapDoc.child("map");
    //search all keydef nodes in the map for keys value that matches the object name
    //set the value of node.child("topicmeta").child("keywords").child("keyword") to ui.objectName->text().toStdString().c_str();

    //only something-boring and ex-name work
    //if there are two props rows each with same props name with a different keyref it fails
    //if the keyrefs are out of order it fails
    //need better method for checking
    auto keysResult = map.select_nodes(".//*[@keys]");
    for (auto& key : keysResult)
    {
        if (key.node().attribute("keys").value() == senderName)
        {
            auto keyValue = key.node().child("topicmeta").child("keywords").child("keyword");
            QTextEdit* senderText = qobject_cast<QTextEdit*>(senderObject);
            keyValue.text().set(senderText->toPlainText().toStdString().c_str()); //this is setting keyValue to the senderObject name...
        }
    }

    mapDoc.save_file(RegulatoryTemplate::mapFile.toStdString().c_str());
}

//TODO get list of keyref values from topics into a vector and check against the list of keysResult - easy: compare list sizes - better: compare list sizes and values