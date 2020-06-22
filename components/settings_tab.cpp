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
		pythonCommandLabel->setToolTip(tr("The command to use for running the cut.py script (the legacy Ranger CLI utility)"));
		line1->addWidget(pythonCommandLabel);

		QHBoxLayout *line1End = new QHBoxLayout();
		line1->addLayout(line1End);

		legacyCLIPythonCommandValue = new QLineEdit(legacyCLIGroup);
		legacyCLIPythonCommandValue->setPlaceholderText("python");
		legacyCLIPythonCommandValue->setToolTip(tr("Type in the name of the command or use the Find button to choose a file"));
		line1End->addWidget(legacyCLIPythonCommandValue);

		connect(legacyCLIPythonCommandValue, &QLineEdit::textEdited, [&](const QString &text){
			if (settings != nullptr)
			{
				settings->legacyCLIPythonCommand = text;
			}
		});

		QPushButton *pythonCommandFind = new QPushButton("Find", legacyCLIGroup);
		pythonCommandFind->setToolTip(tr("Click to open the file inspector for choosing the file"));
		line1End->addWidget(pythonCommandFind);

		connect(pythonCommandFind, &QPushButton::clicked, this, &SettingsTab::legacyCLIPythonCommandFindClicked);

		QHBoxLayout *line2 = new QHBoxLayout();
		legacyCLILayout->addLayout(line2);

		QLabel *scriptPathLabel = new QLabel(tr("Path to legacy Ranger CLI utility:"), legacyCLIGroup);
		scriptPathLabel->setToolTip(tr("Path to the cut.py script file (the legacy Ranger CLI utility)"));
		line2->addWidget(scriptPathLabel);

		QHBoxLayout *line2End = new QHBoxLayout();
		line2->addLayout(line2End);

		legacyCLIScriptPathButton = new QPushButton("Set Path", legacyCLIGroup);
		legacyCLIScriptPathButton->setToolTip(tr("Click to choose a file"));
		line2End->addWidget(legacyCLIScriptPathButton);

		QPushButton *scriptPathReset = new QPushButton("X", legacyCLIGroup);
		scriptPathReset->setToolTip(tr("Click to reset the cut.py path"));
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
		outputFilesPatternLabel->setToolTip(tr("Format used for the output files. Use {} to substitute the layer name"));
		line3->addWidget(outputFilesPatternLabel);

		outputFilesPatternValue = new QLineEdit(legacyCLIGroup);
		outputFilesPatternValue->setToolTip(tr("Type in the format for the output files. Use {} to substitute the layer name"));
		outputFilesPatternValue->setPlaceholderText("{}.mp4");
		line3->addWidget(outputFilesPatternValue);

		connect(outputFilesPatternValue, &QLineEdit::textEdited, [&](const QString &text){
			if (settings != nullptr)
			{
				settings->legacyCLIOutputFilesPattern = text;
			}
		});

		QHBoxLayout *line4 = new QHBoxLayout();
		legacyCLILayout->addLayout(line4);

		QLabel *rangesFileNameLabel = new QLabel(tr("Output files pattern:"), legacyCLIGroup);
		rangesFileNameLabel->setToolTip(tr("If not empty, the ranges file with that name will be saved to the output directory"));
		line4->addWidget(rangesFileNameLabel);

		rangesFileNameValue = new QLineEdit(legacyCLIGroup);
		rangesFileNameValue->setToolTip(tr("If not empty, the ranges file with that name will be saved to the output directory"));
		line4->addWidget(rangesFileNameValue);

		connect(rangesFileNameValue, &QLineEdit::textEdited, [&](const QString &text){
			if (settings != nullptr)
			{
				settings->legacyCLIRangesFileName = text;
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

		outputFilesPatternValue->setText(settings->legacyCLIOutputFilesPattern);
		rangesFileNameValue->setText(settings->legacyCLIRangesFileName);
	}
}
