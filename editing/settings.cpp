#include "settings.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>
#include <QMessageBox>


void Settings::saveToFile(QFile &file)
{
	QJsonObject allSettings;
	allSettings.insert("version", "1.0.0");

	QJsonObject legacyCLI;
	legacyCLI.insert("python_command", legacyCLIPythonCommand);
	legacyCLI.insert("script_path", legacyCLIScriptPath);
	legacyCLI.insert("output_files_pattern", legacyCLIOutputFilesPattern);
	legacyCLI.insert("ranges_file_name", legacyCLIRangesFileName);
	allSettings.insert("legacy_cli", legacyCLI);

	QJsonDocument document(allSettings);

	QTextStream output(&file);
	output << document.toJson(QJsonDocument::Indented);
}

void Settings::save()
{
	QDir::home().mkdir(".ranger");

	QFile file = QDir::home().absoluteFilePath(".ranger/settings.json");

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Couldn't create the settings file") + " (" + file.errorString() + ")");
	}

	this->saveToFile(file);
}

Settings Settings::loadFromFile(QFile &file)
{
	QTextStream input(&file);

	QJsonDocument document = QJsonDocument::fromJson(input.readAll().toUtf8());

	if (!document.isObject())
	{
		return {};
	}

	QJsonObject allSettings = document.object();

	if (allSettings.value("version") != QJsonValue::fromVariant("1.0.0"))
	{
		return {};
	}

	return parseVersion_1_0_0(allSettings);
}

Settings Settings::parseVersion_1_0_0(QJsonObject allSettings)
{
	Settings settings;

	if (allSettings.value("legacy_cli").isObject())
	{
		QJsonObject legacyCLI = allSettings.value("legacy_cli").toObject();

		if (legacyCLI.value("python_command").isString())
		{
			settings.legacyCLIPythonCommand = legacyCLI.take("python_command").toString();
		}

		if (legacyCLI.value("script_path").isString())
		{
			settings.legacyCLIScriptPath = legacyCLI.take("script_path").toString();
		}

		if (legacyCLI.value("output_files_pattern").isString())
		{
			settings.legacyCLIOutputFilesPattern = legacyCLI.take("output_files_pattern").toString();
		}

		if (legacyCLI.value("ranges_file_name").isString())
		{
			settings.legacyCLIRangesFileName = legacyCLI.take("ranges_file_name").toString();
		}
	}

	return settings;
}

Settings Settings::load()
{
	QString filename = QDir::home().absoluteFilePath(".ranger/settings.json");
	QFile file(filename);

	if (file.exists() && !file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Can't open the settings file") + " (" + file.errorString() + ")");
		return {};
	}

	return Settings::loadFromFile(file);
}
