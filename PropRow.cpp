#include "PropRow.h"
#include <sstream>
#include "DitaWellFormedHtml.h"

PropRow::PropRow(const propValueCollection& propsRow, QWidget *parent)
	: QWidget(parent), _myPropsRow(propsRow)
{
	ui.setupUi(this);

	pugi::xml_document doc;

	// fill in UI from propRow
	for (auto& node : _myPropsRow.propsNodes)
	{
		pugi::xpath_query keyref_query(".//*[@data-keyref]");
		auto keyref_results = node.select_nodes(keyref_query);

		for (auto& result : keyref_results)
		{
			insertKeyrefInput(result.node());
		}
		auto nodeCopy = doc.append_copy(node);
		DitaWellFormedHtml::makeWellFormed(nodeCopy);
	}

	std::stringstream ss;
	doc.print(ss);

	ui.textBrowser->setHtml(QString::fromStdString(ss.str()));

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
	QFont labelFont("Normal", 10, QFont::Normal);
	label->setFont(labelFont);
	label->setText(node.attribute("data-keyref").value());
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	label->setAlignment(Qt::AlignBottom);

	QTextEdit* input = new QTextEdit();
	auto senderName = node.attribute("data-keyref").value();
	input->setObjectName(senderName);
	input->setMaximumSize(QSize(130, 50));

	//connect signal textchanged from input object to slot function updateKeyref
	connect(input, &QTextEdit::textChanged, this, &PropRow::updateKeyref);
	//_keyRefTextEdits.push_back(input);

	ui.verticalLayout->addWidget(line);
	ui.verticalLayout->addWidget(label);
	ui.verticalLayout->addWidget(input);
};

void PropRow::updateKeyref()
{
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
			auto qtext = senderText->toPlainText();
			keyValue.text().set(qtext.toStdString().c_str()); //this is setting keyValue to the senderObject name...
			emit updateAllOtherKeyrefs(senderName, qtext);
			return;
		}
	}
	// if it gets here it didn't do anything
}

void PropRow::updateDitaval()
{
	QCheckBox* senderObject = qobject_cast<QCheckBox*>(QObject::sender());
	auto senderName = senderObject->objectName();

	pugi::xml_node val = valDoc.child("val");
	auto node = val.find_child_by_attribute("val", senderName.toStdString().c_str());

	std::string dataPath = ".//*[@name='" + senderName.toStdString() + "']";
	pugi::xpath_query data_query(dataPath.c_str());

	//search through all props rows
	for (auto& node : _myPropsRow.propsNodes)
	{
		//look for nodes with @name that matches the senderName
		auto data_results = node.select_nodes(data_query);

		//for each node found, pull the @value into a vector so we can search for matching nodes
		for (auto& data : data_results)
		{
			auto type = data.node().attribute("datatype").value();
			std::string dataValue = data.node().attribute("value").value();
			std::vector<std::string> values;
			std::string props;
			std::istringstream iss(dataValue);
			while (iss >> props) values.push_back(props);

			//for each node name found, 
			for (auto& nodeName : values)
			{
				//search through all the propsRows for the matching nodes
				for (auto& node : _myPropsRow.propsNodes)
				{
					std::string propsPath = ".//*[@props='" + nodeName + "']";
					pugi::xpath_query props_query(propsPath.c_str());
					auto props_results = node.select_nodes(props_query);

					//for each dependent node found, follow the dependency rules
					for (auto& dependency : props_results)
					{
						std::cout << dependency.node().name() << std::endl;
						//function call for dependency rules
					}
				}
			}
		}
	}

	if (senderObject->isChecked())
	{
		node.attribute("action").set_value("include");
	}
	else
	{
		node.attribute("action").set_value("exclude");
	}
}

bool PropRow::isIncluded() const
{
	return ui.propRow_check->isChecked();
}

const std::string& PropRow::propRowName() const
{
	return _myPropsRow.propsName;
}

PropRow::~PropRow()
{
}

struct blahblah
{
	std::string parent;
	std::vector<std::string> children;

	bool isChild(const std::string& name); // check if `name` exists in `children`
	bool isAnyChild(const std::vector<std::string>& names); // check if anything in `name` exists in `children`
};

std::vector<blahblah> parentStuff;
