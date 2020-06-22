#ifndef SETTINGS2_H
#define SETTINGS2_H

#include <QObject>
#include <QFile>


/**
 * Represents the application
 * settings.
 */
class Settings
{
	public:
		/**
		 * The command or a path to the command
		 * that is a Python interpreter.
		 */
		QString legacyCLIPythonCommand;
		/**
		 * The path to the legacy Ranger
		 * CLI utility.
		 */
		QString legacyCLIScriptPath;
		/**
		 * The pattern to use for the resulting
		 * video files.
		 */
		QString legacyCLIOutputFilesPattern;

		/**
		 * Writes the settings to the
		 * specified file.
		 */
		void saveToFile(QFile &file);
		/**
		 * Returns an instance of Settings
		 * with the contents loaded
		 * from the specified file.
		 * Returns default settings in case
		 * of an error.
		 */
		static Settings loadFromFile(QFile &file);

	private:
		static Settings parseVersion_1_0_0(QJsonObject allSettings);
};


#endif // SETTINGS2_H
