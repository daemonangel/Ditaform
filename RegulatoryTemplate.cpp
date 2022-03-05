#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include <format>
#include "pugixml.hpp"
#include <iostream>
#include "XmlData.h"
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
    //auto filename = QFileDialog::getOpenFileName(this, "Open File", "./");
    
    XmlData _XmlData;

    //for each prop set, add a row to the ui

    /*for (int i = 0; i < 500; i++)
    {
        auto x = new PropRow(ui.centralWidget);
        ui.formLayout->addRow(x);
    }*/

    _XmlData.ProcessData();
}

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    // to do - let user select the file instead of hard coding it
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file("source/maps/bm-sample-source.ditamap");

    pugi::xml_node bookmap = doc.child("bookmap");

    // can also use xpath
    //std::cout << "Bookmap prodname: " << bookmap.child("bookmeta").child("prodinfo").child("prodname").text().get() << std::endl;

    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    
    prodnameNode.text().set("a test project");

    doc.save_file("source/maps/bm-sample-source1.ditamap");
}