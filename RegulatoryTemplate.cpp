#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include "pugixml.hpp"
#include <iostream>
#include "PropRow.h"

QString RegulatoryTemplate::bookFile;
QString RegulatoryTemplate::ditavalFile;
QString RegulatoryTemplate::mapFile;
pugi::xml_document RegulatoryTemplate::bookDoc;
pugi::xml_document valDoc;
pugi::xml_document mapDoc;

RegulatoryTemplate::RegulatoryTemplate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
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
        bookFileSave = QFileDialog::getSaveFileName(this, tr("Save As..."), "product-rg-en.ditamap", tr("DITA Map (*.ditamap)"));
        bookFile = QFileInfo(bookFileSave).absolutePath() + "/bm-" + QFileInfo(bookFileSave).baseName() + ".ditamap";
        pugi::xml_parse_result result = bookDoc.load_file(_xmlData->sourceBookmapFile);
        pugi::xml_node bookmap = bookDoc.child("bookmap");
        bookDoc.save_file(bookFile.toStdString().c_str());

        //ditaval
        ditavalFile = QFileInfo(bookFileSave).absolutePath() + "/dv-" + QFileInfo(bookFileSave).baseName() + ".ditaval";
        pugi::xml_parse_result valResult = valDoc.load_file(_xmlData->sourceDitavalFile);
        valDoc.save_file(ditavalFile.toStdString().c_str());

        //map
        mapFile = QFileInfo(bookFileSave).absolutePath() + "/m-" + QFileInfo(bookFileSave).baseName() + ".ditamap";
        pugi::xml_parse_result mapResult = mapDoc.load_file(_xmlData->sourceMapFile);
        mapDoc.save_file(mapFile.toStdString().c_str());
    }
    //TODO: On closing the app, check if file is saved. If it isn't ask user if they want to save the file.
    // https://doc.qt.io/qt-5/qfilesystemwatcher.html#fileChanged
    //TODO: Notify the user somehow that they have unsaved changes
}

void RegulatoryTemplate::fileSaveAs()
{
    _xmlData = std::make_unique<XmlData>();

    //save a copy of the source files to user defined location/file for bookmap
    //bookmap
    bookFileSave = QFileDialog::getSaveFileName(this, tr("Save As..."), "PRODUCT-rg-en.ditamap", tr("DITA Map (*.ditamap)"));
    bookFile = QFileInfo(bookFileSave).absolutePath() + "/bm-" + QFileInfo(bookFileSave).baseName() + ".ditamap";
    pugi::xml_parse_result result = bookDoc.load_file(_xmlData->sourceBookmapFile);
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    bookDoc.save_file(bookFile.toStdString().c_str());

    //ditaval
    //TODO: Update the file name to dv- and everything that comes after bm- in the bookFile name.

    ditavalFile = QFileInfo(bookFileSave).absolutePath() + "/dv-" + QFileInfo(bookFileSave).baseName() + ".ditaval";
    pugi::xml_parse_result valResult = valDoc.load_file(_xmlData->sourceDitavalFile);
    valDoc.save_file(ditavalFile.toStdString().c_str());

    //map
    //TODO: Update the file name to m- and everything that comes after bm- in the bookFile name. Update the bookmap file to match.
    mapFile = QFileInfo(bookFileSave).absolutePath() + "/m-" + QFileInfo(bookFileSave).baseName() + ".ditamap";
    pugi::xml_parse_result mapResult = mapDoc.load_file(_xmlData->sourceMapFile);
    mapDoc.save_file(mapFile.toStdString().c_str());
}

void RegulatoryTemplate::loadSource()
{
    _xmlData = std::make_unique<XmlData>();

    //TODO - test that reloading rows works as expected.
    //if the row count is more than the number of premade rows, delete the dynamically created rows
    if (ui.formLayout->rowCount() > premadeRows)
    {
        auto rowCount = ui.formLayout->rowCount() - 1;
        for (int i = rowCount; i >= premadeRows; i--)
        {
            ui.formLayout->removeRow(i);
        }
    }
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