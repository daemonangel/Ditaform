#include "PropRow.h"

PropRow::PropRow(const propValueCollection& propsRow, QWidget *parent)
	: QWidget(parent), _myPropsRow(propsRow)
{
	ui.setupUi(this);

	// fill in UI from propRow
	for (auto& node : _myPropsRow.propsNodes)
	{
		//get all nodes inside this props
		auto allText = node.select_nodes(".//node()");

		for (auto& child : allText)
		{
			if (child.node().attribute("keyref"))
			{
				ui.textBrowser->setFontPointSize(12);
				ui.textBrowser->setTextColor(QColor(255, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().attribute("keyref").value());

				//add keyref to the group box
				QFrame* line = new QFrame(this);
				line->setFrameStyle(QFrame::NoFrame);
				line->setLineWidth(5);
				QFont labelFont("Normal", 10, QFont::Bold);
				QLabel* label = new QLabel(this);
				label->setFrameStyle(QFrame::NoFrame);
				label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				label->setFont(labelFont);
				label->setText(child.node().attribute("keyref").value());
				QTextEdit* input = new QTextEdit();
				auto senderName = child.node().attribute("keyref").value();
				input->setObjectName(senderName);

				//connect signal textchanged from input object to slot function updateKeyref
				//bool connectResult = connect(input, &QTextEdit::textChanged, _RegulatoryTemplate.get(), &RegulatoryTemplate::updateKeyref);
				bool connectResult = connect(input, &QTextEdit::textChanged, this, &PropRow::updateKeyref);

				ui.verticalLayout->addWidget(line);
				ui.verticalLayout->addWidget(label);
				ui.verticalLayout->addWidget(input);
			}
			else if (child.node().name() == std::string("title") || child.parent().name() == std::string("title"))
			{
				ui.textBrowser->setFontPointSize(16);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Bold);
				ui.textBrowser->insertPlainText(child.node().value());
				ui.textBrowser->insertHtml("<br>"); //line break
			}
			else
			{
				ui.textBrowser->setFontPointSize(12);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().value());
				ui.textBrowser->insertHtml("<p>"); //line break
			}
		}
	}

	//set title of groupbox to the props node name
	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow.propsName));

	//set object name of checkbox to the props node name
	ui.propRow_check->setObjectName(QString::fromStdString(_myPropsRow.propsName));

	//connect checkbox signal to updateDitaval slot
	bool connectResult = connect(ui.propRow_check, &QCheckBox::stateChanged, this, &PropRow::updateDitaval);

	//TODO checkboxes not always working
}

void PropRow::updateKeyref()
{
	//TODO get list of keyref values from topics into a vector and check against the list of keysResult - easy: compare list sizes - better: compare list sizes and values

	auto senderObject = QObject::sender();
	auto senderName = senderObject->objectName();

	pugi::xml_node map = mapDoc.child("map");
	//search all keydef nodes in the map for keys value that matches the object name

	//if same props row has more than one keyref, only the last one created emits a signal
	auto keysResult = map.select_nodes(".//*[@keys]");
	for (auto& key : keysResult)
	{
		QString keyName = key.node().first_attribute().value();
		if (keyName == senderName)
		{
			auto keyValue = key.node().child("topicmeta").child("keywords").child("keyword");
			QTextEdit* senderText = qobject_cast<QTextEdit*>(senderObject);
			keyValue.text().set(senderText->toPlainText().toStdString().c_str()); //this is setting keyValue to the senderObject name...
		}
	}
}

void PropRow::updateDitaval()
{
	QCheckBox* senderObject = qobject_cast<QCheckBox*>(QObject::sender());
	auto senderName = senderObject->objectName();

	pugi::xml_node val = valDoc.child("val");
	auto node = val.find_child_by_attribute("val", senderName.toStdString().c_str());

	if (senderObject->isChecked())
	{
		node.attribute("action").set_value("include");
	}
	else
	{
		node.attribute("action").set_value("exclude");
	}
}

PropRow::~PropRow()
{
}