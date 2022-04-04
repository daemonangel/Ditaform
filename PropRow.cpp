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
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(255, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().attribute("keyref").value());
				insertKeyrefInput(child.node());
				//add a line break if last node in block
				if (child.node() == child.parent().last_child())
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}
			if (child.parent().name() == std::string("uicontrol"))
			{
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Bold);
				ui.textBrowser->insertPlainText(child.node().value());
				//add a line break if last node in block
				if (child.parent() == child.parent().parent().last_child())
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}
			else if (child.node().name() == std::string("title") || child.parent().name() == std::string("title"))
			{
				ui.textBrowser->setFontPointSize(14);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Bold);
				ui.textBrowser->insertPlainText(child.node().value());
				ui.textBrowser->insertHtml("<br>"); //line break
			}
			else if (child.parent().name() == std::string("li") && !child.node().previous_sibling().attribute("keyref"))
			{
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				//if first node in block
				if (child.node() == child.parent().first_child())
				{
					ui.textBrowser->insertPlainText("[bullet] ");
				}
				ui.textBrowser->insertPlainText(child.node().value());
				//add a line break if last node in block
				if (child.node() == child.parent().last_child())
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}
			else if (child.parent().name() == std::string("cmd") && !child.node().previous_sibling().attribute("keyref"))
			{
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText("[step] ");
				ui.textBrowser->insertPlainText(child.node().value());
				//add a line break if last node in block
				if (child.node() == child.parent().last_child())
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}
			/*else if (child.node().name() == std::string("alt"))
			{
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText("[image] ");
				ui.textBrowser->insertPlainText(child.node().value());
				//add a line break if last node in block
				if (child.node() == child.parent().last_child())
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}*/
			else
			{
				ui.textBrowser->setFontPointSize(10);
				ui.textBrowser->setTextColor(QColor(0, 0, 0));
				ui.textBrowser->setFontWeight(QFont::Normal);
				ui.textBrowser->insertPlainText(child.node().value());
				//add a line break if last node in block
				if (child.node() == child.parent().last_child() && child.node().type() != pugi::node_element)
				{
					ui.textBrowser->insertHtml("<br>"); //line break
				}
			}
		}
	}

	//set title of groupbox to the props node name
	ui.propRow_group->setTitle(QString::fromStdString(_myPropsRow.propsName));

	//set object name of checkbox to the props node name
	ui.propRow_check->setObjectName(QString::fromStdString(_myPropsRow.propsName));

	//connect checkbox signal to updateDitaval slot
	connect(ui.propRow_check, &QCheckBox::stateChanged, this, &PropRow::updateDitaval);
}

void PropRow::insertKeyrefInput(const pugi::xml_node& node)
{
	//add keyref to the group box
	QFrame* line = new QFrame(this);
	line->setFrameStyle(QFrame::NoFrame);
	line->setLineWidth(5);

	QLabel* label = new QLabel(this);
	label->setFrameStyle(QFrame::NoFrame);
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QFont labelFont("Normal", 10, QFont::Bold);
	label->setFont(labelFont);
	label->setText(node.attribute("keyref").value());

	QTextEdit* input = new QTextEdit();
	auto senderName = node.attribute("keyref").value();
	input->setObjectName(senderName);
	input->setMaximumSize(QSize(130, 50));
	input->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	//connect signal textchanged from input object to slot function updateKeyref
	connect(input, &QTextEdit::textChanged, this, &PropRow::updateKeyref);

	ui.verticalLayout->addWidget(line);
	ui.verticalLayout->addWidget(label);
	ui.verticalLayout->addWidget(input);
};

void PropRow::updateKeyref()
{
	//TODO get list of keyref values from topics into a vector and check against the list of keysResult - easy: compare list sizes - better: compare list sizes and values

	auto senderObject = QObject::sender();
	auto senderName = senderObject->objectName();

	pugi::xml_node map = mapDoc.child("map");

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