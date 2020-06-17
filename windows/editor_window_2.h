#ifndef EDITORWINDOW2_H
#define EDITORWINDOW2_H

#include "overlay_window.h"

#include <QMenuBar>
#include <QStackedLayout>

#include "components/video_tab_2.h"


/**
 * The main window for the app.
 */
class EditorWindow2 : public OverlayWindow
{
		Q_OBJECT

	public:
		EditorWindow2();

	private:
		QAction *actionNewVideoFile;
		QAction *actionNewYouTube;
		QAction *actionOpen;
		QAction *actionSave;
		QAction *actionLoadRangesFile;
		QAction *actionExportRangesFile;
		QAction *actionAbout;

		void setupActions();
		// `run` - it's a callback
		// for an action
		void runAbout();
		void runNewVideoFile();
		void runNewYouTube();
		void runOpen();
		void runSave();
		void runLoadRangesFile();
		void runExportRangesFile();

		QMenuBar *menuBar;

		void setupMenu();

		VideoTab2 *videoTab;
		QStackedLayout *editingTabLayout;

		// accepts userInputTextRequired from
		// the video tab and passes it to the
		// overlay
		void userInputText(const QString &question, QObject *receiver, TextCallback callback);
};


#endif // EDITORWINDOW2_H
