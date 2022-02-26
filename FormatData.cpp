#include "FormatData.h"
#include "XmlData.h"

FormatData::FormatData()
    : QWidget()
{
    //for each pair in keysList
    //  create a group
    //    if node is a child of immediately preceeding node,
    //    create a label and line edit
    //      if node is a sibling of immediately preceeding node,
    //      create a label and line edit
    //  create a content preview of all nodes in the group (maybe need temporary variable to hold the nodes for this)
}

QGroupBox* FormatData::contentUi_group(std::vector<std::pair<pugi::xml_node, std::string>> keysList)
{
    //create group for each props where text is the props value converted to normal text. this_is_a_prop -> This Is A Prop
    QGroupBox* groupBox = new QGroupBox(tr("props attribute value"));

    //create one checkbox - set objectName to the props value - will use the objectName to find it in the ditaval file
    // need a function to enable/disable the prop in ditaval file based on the checkbox objectName
    auto action_checkbox = new QCheckBox(QObject::tr("Enable/Disable"));
    QObject::connect(action_checkbox, SIGNAL(clicked(bool)), this, SLOT(enableDisableContent(bool, action_checkbox.text)));

    //for each keyref found, assign keysList string to the label name
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

void FormatData::contentPreview()
{
    contentPreview_browser = new QTextBrowser;

    // add formatting
}
