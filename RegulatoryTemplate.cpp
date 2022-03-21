#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include <format>
#include "pugixml.hpp"
#include <iostream>
#include "PropRow.h"

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

void RegulatoryTemplate::fileOpen()
{
    //save a copy of the source bookmap to user defined location/file
    auto saveFile = QFileDialog::getSaveFileName(this, tr("Choose a folder"), "bm-PRODUCT-rg-en.ditamap", tr("DITA Map (*.ditamap)"));
    _xmlData = std::make_unique<XmlData>();
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(_xmlData->sourceFile);
    pugi::xml_node bookmap = doc.child("bookmap");
    doc.save_file(saveFile.toStdString().c_str());

    _xmlData = std::make_unique<XmlData>();

    //for each prop set, add a row to the ui

    for (auto& propRow : _xmlData->_propsRows)
    {
        auto propUI = new PropRow(propRow.second, ui.centralWidget);
        ui.formLayout->addRow(propUI);
    }
}

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    //saveFile = null
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(saveFile.toStdString().c_str());
    pugi::xml_node bookmap = doc.child("bookmap");

    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    prodnameNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    doc.save_file(saveFile.toStdString().c_str());
}

void RegulatoryTemplate::partnumEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(RegulatoryTemplate::saveFile.toStdString().c_str());
    pugi::xml_node bookmap = doc.child("bookmap");

    auto partnumNode = bookmap.child("bookmeta").child("bookid").child("bookpartno");
    partnumNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    doc.save_file(RegulatoryTemplate::saveFile.toStdString().c_str());
}

void RegulatoryTemplate::monthEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(RegulatoryTemplate::saveFile.toStdString().c_str());
    pugi::xml_node bookmap = doc.child("bookmap");

    auto monthNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("month");
    monthNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    doc.save_file(RegulatoryTemplate::saveFile.toStdString().c_str());
}

void RegulatoryTemplate::yearEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(RegulatoryTemplate::saveFile.toStdString().c_str());
    pugi::xml_node bookmap = doc.child("bookmap");

    auto yearCompletedNode = bookmap.child("bookmeta").child("publisherinformation").child("published").child("completed").child("year");
    auto yearCopyrightNode = bookmap.child("bookmeta").child("bookrights").child("copyrlast").child("year");
    yearCompletedNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    yearCopyrightNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    doc.save_file(RegulatoryTemplate::saveFile.toStdString().c_str());
}

void RegulatoryTemplate::revisionEdit([[maybe_unused]] const QString& metadata)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(RegulatoryTemplate::saveFile.toStdString().c_str());
    pugi::xml_node bookmap = doc.child("bookmap");

    auto revisionNode = bookmap.child("bookmeta").child("bookid").child("volume");
    revisionNode.text().set(ui.prodname_edit->text().toStdString().c_str());
    doc.save_file(RegulatoryTemplate::saveFile.toStdString().c_str());
}