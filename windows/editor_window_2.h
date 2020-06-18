#ifndef EDITORWINDOW2_H
#define EDITORWINDOW2_H

#include "overlay_window.h"

#include <QMenuBar>
#include <QStackedLayout>

#include "components/video_tab.h"
#include "components/placeholder.h"


/**
 * The main window for the app.
 */
class EditorWindow2 : public OverlayWindow
{
		Q_OBJECT

	public:
		EditorWindow2();

	private:
		Project *project = nullptr;

		QAction *actionAbout;
		QAction *actionNewVideoFile;
		QAction *actionNewYouTube;
		QAction *actionOpen;
		QAction *actionSave;
		QAction *actionLoadRangesFile;
		QAction *actionExportRangesFile;
		QAction *actionTogglePlayback;
		QAction *actionStepLeft;
		QAction *actionStepRight;
		QAction *actionJumpLeft;
		QAction *actionJumpRight;
		QAction *actionAddLayer;
		QAction *actionRemoveLayer;
		QAction *actionSelectUpperLayer;
		QAction *actionSelectLowerLayer;
		QAction *actionRecordSegment;
		QAction *actionNewLeftBound;
		QAction *actionNewRightBound;

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
		void runAddLayer();
		void receiveNewLayerName(bool isCanceled, const QString &name);
		void runRemoveLayer();
		void runNewLeftBound();
		void runNewRightBound();

		QMenuBar *menuBar;

		void setupMenu();

		Placeholder *videoTabPlaceholder;
		VideoTab2 *videoTab;
};


#endif // EDITORWINDOW2_H
