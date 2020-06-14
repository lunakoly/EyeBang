#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <components/custom_window.h>
#include <components/super_video_widget.h>

#include <QMenuBar>
#include <QAction>
#include <QShortcut>


class EditorWindow : public CustomWindow
{
		Q_OBJECT

	public:
		explicit EditorWindow(QWidget *parent = nullptr);

	private:
		QAction *actionNewVideoFile;
		QAction *actionNewYouTube;
		QAction *actionOpen;
		QAction *actionSave;
		QAction *actionLoadRangesFile;
		QAction *actionExportRangesFile;
		QAction *actionAbout;

		void setupAction();
		void runNewVideoFile();
		void runNewYouTube();
		void runOpen();
		void runSave();
		void runLoadRangesFile();
		void runExportRangesFile();
		void runAbout();

		QShortcut *shortcutTogglePlayback;
		QShortcut *shortcutStepLeft;
		QShortcut *shortcutStepRight;
		QShortcut *shortcutJumpLeft;
		QShortcut *shortcutJumpRight;

		void setupShortcuts();
		void doTogglePlayback();

		QMenuBar *menuBar;
		SuperVideoWidget *videoTab;

		void setupMenu();
};


#endif // EDITORWINDOW_H
