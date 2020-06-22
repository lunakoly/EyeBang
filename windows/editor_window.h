#ifndef EDITORWINDOW2_H
#define EDITORWINDOW2_H

#include "overlay_window.h"

#include <QMenuBar>
#include <QStackedLayout>
#include <QTemporaryFile>

#include "components/video_tab.h"
#include "components/placeholder.h"
#include "editing/settings.h"
#include "components/settings_tab.h"


/**
 * The main window for the app.
 */
class EditorWindow : public OverlayWindow
{
		Q_OBJECT

	public:
		EditorWindow();

	private:
		Project *project = nullptr;
		Settings settings;

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
		QAction *actionRenameLayer;
		QAction *actionSelectUpperLayer;
		QAction *actionSelectLowerLayer;
		QAction *actionRecordSegment;
		QAction *actionNewLeftBound;
		QAction *actionNewRightBound;
		QAction *actionStepScrollLeft;
		QAction *actionStepScrollRight;
		QAction *actionRenderLegacy;

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
		void receiveAddLayerName(bool isCanceled, const QString &name);
		void runRemoveLayer();

		QTemporaryFile *rangesFile;
		void runRenderLegacy();

		QString oldNameToReplace;
		void runRenameLayer();
		void receiveRenameLayer(bool isCanceled, const QString &newName);

		void runNewLeftBound();
		void runNewRightBound();

		QMenuBar *menuBar;

		void setupMenu();

		Placeholder *videoTabPlaceholder;
		VideoTab *videoTab;
		SettingsTab *settingsTab;

		void closeEvent(QCloseEvent *event) override;
};


#endif // EDITORWINDOW2_H
