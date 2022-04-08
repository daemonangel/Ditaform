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

		formatRows(allText);
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

void PropRow::formatRows(const pugi::xpath_node_set& allText)
{
	QTextCursor cursor = ui.textBrowser->textCursor();

	QTextListFormat::Style bulletStyle = QTextListFormat::ListDisc;
	QTextListFormat bulletList;

	QTextListFormat::Style numberStyle = QTextListFormat::ListDecimal;
	QTextListFormat numberList;

	QTextBlockFormat blockFormat = cursor.blockFormat();

	QTextCharFormat charDefault;
	charDefault.setFontWeight(400);
	charDefault.setFontPointSize(10);
	charDefault.setForeground(Qt::black);

	QTextCharFormat charTitle;
	charTitle.setFontWeight(700);
	charTitle.setFontPointSize(14);
	charTitle.setForeground(Qt::black);

	pugi::xpath_query keyref_query("ancestor-or-self::*[@keyref]");
	pugi::xpath_query uicontrol_query("ancestor-or-self::uicontrol");
	pugi::xpath_query list_item_query("ancestor-or-self::li"); // li can be the root node of a row
	pugi::xpath_query step_item_query("ancestor-or-self::cmd");
	pugi::xpath_query has_next_sibling_query(".//following-sibling::*");
	pugi::xpath_query title_query("parent::title");
	pugi::xpath_query alt_query("parent::alt");
		 
	for (auto& child : allText)
	{
		cursor.setCharFormat(charDefault);

		bool keyref_item = child.node().select_node(keyref_query).node();
		bool uicontrol_item = child.node().select_node(uicontrol_query).node();
		bool list_item = child.node().select_node(list_item_query).node();
		bool step_item = child.node().select_node(step_item_query).node();
		bool has_next_sibling_item = child.node().select_node(has_next_sibling_query).node();
		bool title_item = child.node().select_node(title_query).node();
		bool alt_item = child.node().select_node(alt_query).node();

		//inline formatting
		if (keyref_item)
		{
			ui.textBrowser->setTextColor(QColor(255, 0, 0));
			ui.textBrowser->insertPlainText(child.node().attribute("keyref").value());
			insertKeyrefInput(child.node());
		}
		else if (uicontrol_item)
		{
			ui.textBrowser->setFontWeight(QFont::Bold);
			ui.textBrowser->insertPlainText(child.node().value());
		}

		//block formatting
		else if (alt_item)
		{
			ui.textBrowser->setTextColor(QColor(255, 0, 255));
			auto text = "[image] " + std::string(child.node().value()) + " [image]";
			ui.textBrowser->insertPlainText(text.c_str());
			if (!has_next_sibling_item) { cursor.insertBlock(); }
		}
		else if (title_item)
		{
			cursor.setCharFormat(charTitle);
			cursor.insertText(child.node().value());
			cursor.insertBlock();
		}
		else if (list_item && !keyref_item)
		{
			bulletList.setStyle(bulletStyle);
			cursor.insertText(child.node().value()); cursor.createList(bulletList);
			if (!has_next_sibling_item) { cursor.insertBlock(); }
		}
		else if (step_item && !keyref_item)
		{
			numberList.setStyle(numberStyle);
			cursor.insertText(child.node().value()); cursor.createList(numberList);
			if (!has_next_sibling_item) { cursor.insertBlock(); }
		}
		else
		{
			cursor.insertText(child.node().value());
			if (!has_next_sibling_item) { cursor.insertBlock(); }
		}	
	}
}

PropRow::~PropRow()
{
}