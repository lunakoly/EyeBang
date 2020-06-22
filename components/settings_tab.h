#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>

#include "editing/settings.h"


/**
 * One of the main editor tabs.
 * Used to specify important parameters.
 */
class SettingsTab : public QScrollArea
{
		Q_OBJECT

	public:
		explicit SettingsTab(QWidget *parent = nullptr);

		/**
		 * Returns the associated
		 * Settings instance.
		 */
		Settings *getSettings();
		/**
		 * Instructs the widget to use
		 * and affect values from the
		 * given Settings instance.
		 */
		void setSettings(Settings *settings);

	private:
		Settings *settings;

		QLineEdit *legacyCLIPythonCommandValue;
		QPushButton *legacyCLIScriptPathButton;
		QLineEdit *outputFilesPatternValue;
		QLineEdit *rangesFileNameValue;

		void legacyCLIPythonCommandFindClicked();
		void legacyCLIPythonCommandChanged();
		void legacyCLIScriptPathButtonClicked();
		void legacyCLIScriptPathChanged();
};


#endif // SETTINGSTAB_H
