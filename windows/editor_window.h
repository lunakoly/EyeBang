#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "custom_window.h"
#include "components/video_tab.h"

#include <QMenuBar>
#include <QAction>
#include <QShortcut>
#include <QStackedLayout>

#include "components/overlay.h"


/**
 * The main window for the app.
 */
class EditorWindow : public CustomWindow
{
		Q_OBJECT

	public:
		explicit EditorWindow(QWidget *parent = nullptr);

		/**
		 * This is a callback for the overlay
		 * when prompting for the user text input.
		 * It's used to get the name for the new layer
		 * when the user presses the shortcut.
		 */
		void doAddLayerOverlayCallback(const QString &answer);

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
		QShortcut *shortcutAddLayer;
		QShortcut *shortcutRemoveLayer;
		QShortcut *shortcutSelectUpperLayer;
		QShortcut *shortcutSelectLowerLayer;
		QShortcut *shortcutRecordSegment;
		QShortcut *shortcutNewLeftBound;
		QShortcut *shortcutNewRightBound;

		void setupShortcuts();
		// `do` - it's a callback
		// for a shortcut
		void doTogglePlayback();
		void doAddLayer();
		void doRemoveLayer();
		void doNewLeftBound();
		void doNewRightBound();

		QMenuBar *menuBar;
		VideoTab *videoTab;
		Overlay *overlay;
		QStackedLayout *layout;

		void setupMenu();
};


#endif // EDITORWINDOW_H
