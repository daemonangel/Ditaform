#include "XmlData.h"

XmlData::XmlData(QWidget* parent)
    : QDialog(parent)
{
    action_checkbox = new QCheckBox(QObject::tr("Enable/Disable"));

    // text will be keyref value
    keydef_label = new QLabel(QObject::tr("Keydef"));

    keydef_edit = new QLineEdit;

    contentPreview_browser = new QTextBrowser;

    //ui.formLayout->addRow(keydef_label, keydef_edit);
}