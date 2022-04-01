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

void RegulatoryTemplate::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        fileSave();
        event->accept();
    }
    else {
        event->accept();
    }
}

bool RegulatoryTemplate::maybeSave()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
        tr("Save file before closing?"),
        QMessageBox::Save | QMessageBox::Close);
    if (ret == QMessageBox::Save)
        return true;
    else if (ret == QMessageBox::Close)
        return false;
}

//TODO make a button that exports versions of the document with translation info, like language codes. this is for later.

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

//TODO add a button to load files - loaded file should also populate the keyref QTextEdits

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
        RegulatoryTemplate::fileSaveAs();
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
    bookFile = QFileDialog::getSaveFileName(this, tr("Save As..."), "PRODUCT-rg-en.ditamap", tr("DITA Bookmap (*.ditamap)"));

    pugi::xml_parse_result result = bookDoc.load_file(_xmlData->sourceBookmapFile);
    pugi::xml_node bookmap = bookDoc.child("bookmap");
    bookDoc.save_file(bookFile.toStdString().c_str());

    //ditaval
    ditavalFile = QFileInfo(bookFile).absolutePath() + "/dv-" + QFileInfo(bookFile).baseName() + ".ditaval";
    pugi::xml_parse_result valResult = valDoc.load_file(_xmlData->sourceDitavalFile);
    valDoc.save_file(ditavalFile.toStdString().c_str());

    //map
    mapFile = QFileInfo(bookFile).absolutePath() + "/m-" + QFileInfo(bookFile).baseName() + ".ditamap";
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

    //TODO need a way to deal with multiple instances of the same keyref in the form. maybe when u edit one, it automatically puts the same text in all of them?
    //connect all QTextEdit textChange signals to autoUpdateDupKeyrefs slot
    auto keyrefs = ui.centralWidget->findChildren<QTextEdit*>();
    for (auto& key : keyrefs)
    {
        bool connectResult = connect(key, &QTextEdit::textChanged, this, &RegulatoryTemplate::autoUpdateDupKeyrefs);
    }
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

void RegulatoryTemplate::autoUpdateDupKeyrefs()
{
    //temporarily turn off the signal from sender
    auto senderObject = QObject::sender();

    senderObject->blockSignals(true);
    auto senderName = senderObject->objectName();
    QTextEdit* senderCast = qobject_cast<QTextEdit*>(senderObject);

    auto keyrefs = ui.centralWidget->findChildren<QTextEdit*>(senderName);
    if (keyrefs.size() < 2) { return; }; //only continue if list has at least 2 items

    for (auto& key : keyrefs)
    {
        auto keyName = key->objectName();
        auto focus = key->hasFocus();
        if (senderName == key->objectName())
        {
            auto senderText = senderCast->toPlainText();
            key->setPlainText(senderText);
        }
    }
    senderObject->blockSignals(false);
}