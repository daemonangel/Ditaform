#include "RegulatoryTemplate.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qinputdialog.h"
#include <format>
#include "pugixml.hpp"
#include <iostream>
#include "XmlData.h"

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

QGroupBox *RegulatoryTemplate::contentUi_group()
{

    //create group for each props where text is the props value converted to normal text. this_is_a_prop -> This Is A Prop
    QGroupBox* groupBox = new QGroupBox(tr("Condition Name"));

    //each group gets one checkbox where the checkbox objectName is the props value
    auto action_checkbox = new QCheckBox(QObject::tr("Enable/Disable"));
    QObject::connect(action_checkbox, SIGNAL(clicked(bool)), this, SLOT(enableDisableContent(bool, action_checkbox.text)));

    //each group gets zero or more label/lineEdit pairs - one for each keyref found
    auto keydef_label = new QLabel(QObject::tr("Keyref Value")); // text will be keyref value
    auto keydef_edit = new QLineEdit;

    //add the checkbox and all the label/lineEdit pairs to the group.
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(action_checkbox);
    vbox->addWidget(keydef_label);
    vbox->addWidget(keydef_edit);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

void RegulatoryTemplate::fileOpen()
{
    //auto filename = QFileDialog::getOpenFileName(this, "Open File", "./");
    
    //get the bookmap
    pugi::xml_document bookmap;
    pugi::xml_parse_result result = bookmap.load_file("../source/maps/bm-sample-source.ditamap");

    //get the map by finding the first child chapter with a href attribute
    pugi::xml_document map;
    //auto mapHref = pugi::xml_node::find_child(allow_remote_predicate()).attribute("Href").value() << std::endl;

    //XmlData s;
    //how do i get the ui from the snippet class? I get an access violation error when I try to call ui.formLayout from snippet.cpp

    //add one row for each box/textBrowser pair
    auto contentPreview_browser = new QTextBrowser;
    ui.formLayout->addRow(contentUi_group(), contentPreview_browser);
}

void RegulatoryTemplate::prodnameEdit([[maybe_unused]] const QString& metadata)
{
    // to do - let user select the file instead of hard coding it
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file("../source/maps/bm-sample-source.ditamap");

    pugi::xml_node bookmap = doc.child("bookmap");

    // can also use xpath
    //std::cout << "Bookmap prodname: " << bookmap.child("bookmeta").child("prodinfo").child("prodname").text().get() << std::endl;

    auto prodnameNode = bookmap.child("bookmeta").child("prodinfo").child("prodname");
    
    prodnameNode.text().set("a test project");

    doc.save_file("../source/maps/bm-sample-source1.ditamap");
}