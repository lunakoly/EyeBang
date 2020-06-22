#include "settings_tab.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>


SettingsTab::SettingsTab(QWidget *parent) : QScrollArea(parent)
{
	QWidget *content = new QWidget(this);
	content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QVBoxLayout *layout = new QVBoxLayout(content);

	QGroupBox *legacyCLIGroup = new QGroupBox("Legacy Ranger CLI", this);
	QVBoxLayout *legacyCLILayout = new QVBoxLayout(legacyCLIGroup);
	layout->addWidget(legacyCLIGroup);

	{
		QHBoxLayout *line1 = new QHBoxLayout();
		legacyCLILayout->addLayout(line1);

		QLabel *pythonCommandLabel = new QLabel(tr("Python interpreter command:"), legacyCLIGroup);
		line1->addWidget(pythonCommandLabel);

		QHBoxLayout *line1End = new QHBoxLayout();
		line1->addLayout(line1End);

		legacyCLIPythonCommandValue = new QLineEdit(legacyCLIGroup);
		legacyCLIPythonCommandValue->setPlaceholderText("python");
		line1End->addWidget(legacyCLIPythonCommandValue);

		connect(legacyCLIPythonCommandValue, &QLineEdit::textEdited, [&](const QString &text){
			if (settings != nullptr)
			{
				settings->legacyCLIPythonCommand = text;
			}
		});

		QPushButton *pythonCommandFind = new QPushButton("Find", legacyCLIGroup);
		line1End->addWidget(pythonCommandFind);

		connect(pythonCommandFind, &QPushButton::clicked, this, &SettingsTab::legacyCLIPythonCommandFindClicked);

		QHBoxLayout *line2 = new QHBoxLayout();
		legacyCLILayout->addLayout(line2);

		QLabel *scriptPathLabel = new QLabel(tr("Path to legacy Ranger CLI utility:"), legacyCLIGroup);
		line2->addWidget(scriptPathLabel);

		QHBoxLayout *line2End = new QHBoxLayout();
		line2->addLayout(line2End);

		legacyCLIScriptPathButton = new QPushButton("Set Path", legacyCLIGroup);
		line2End->addWidget(legacyCLIScriptPathButton);

		QPushButton *scriptPathReset = new QPushButton("X", legacyCLIGroup);
		line2End->addWidget(scriptPathReset);

		connect(legacyCLIScriptPathButton, &QPushButton::clicked, this, &SettingsTab::legacyCLIScriptPathButtonClicked);

		connect(scriptPathReset, &QPushButton::clicked, this, [&](){
			if (settings != nullptr)
			{
				settings->legacyCLIScriptPath = "";
				legacyCLIScriptPathButton->setText("Find");
			}
		});

		QHBoxLayout *line3 = new QHBoxLayout();
		legacyCLILayout->addLayout(line3);

		QLabel *outputFilesPatternLabel = new QLabel(tr("Output files pattern:"), legacyCLIGroup);
		line3->addWidget(outputFilesPatternLabel);

		QLineEdit *outputFilesPatternValue = new QLineEdit(legacyCLIGroup);
		outputFilesPatternValue->setPlaceholderText("{}.mp4");
		line3->addWidget(outputFilesPatternValue);

		connect(outputFilesPatternValue, &QLineEdit::textEdited, [&](const QString &text){
			if (settings != nullptr)
			{
				settings->legacyCLIOutputFilesPattern = text;
			}
		});
	}

	setWidgetResizable(true);
	setWidget(content);
}

void SettingsTab::legacyCLIPythonCommandFindClicked()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Find Python Interpreter")
	);

	if (filename.isNull())
	{
		return;
	}

	if (settings != nullptr)
	{
		legacyCLIPythonCommandValue->setText(filename);
		settings->legacyCLIPythonCommand = filename;
	}
}

void SettingsTab::legacyCLIScriptPathButtonClicked()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Legacy Ranges CLI Script"),
		"",
		tr("Python Script (*.py);;All Files (*)")
	);

	if (filename.isNull())
	{
		return;
	}

	if (settings != nullptr)
	{
		settings->legacyCLIScriptPath = filename;
		legacyCLIScriptPathButton->setText(filename);
	}
}

Settings *SettingsTab::getSettings()
{
	return settings;
}

void SettingsTab::setSettings(Settings *settings)
{
	if (settings != nullptr)
	{
		this->settings = settings;
		legacyCLIPythonCommandValue->setText(settings->legacyCLIPythonCommand);

		if (settings->legacyCLIScriptPath.isEmpty())
		{
			legacyCLIScriptPathButton->setText("Find");
		}
		else
		{
			legacyCLIScriptPathButton->setText(settings->legacyCLIScriptPath);
		}
	}
}

//void SettingsTab::legacyCLIScriptPathChanged()
//{
//	if (settings != nullptr)
//	{
//		if (settings->getLegacyCLIScriptPath().isEmpty()){
//			legacyCLIScriptPathButton->setText("Set Path");
//		}
//		else
//		{
//			legacyCLIScriptPathButton->setText(settings->getLegacyCLIScriptPath());
//		}
//	}
//}

//void SettingsTab::legacyCLIPythonCommandChanged()
//{
//	if (settings != nullptr)
//	{
//		legacyCLIPythonCommandValue->setText(settings->getLegacyCLIPythonCommand());
//	}
//}
