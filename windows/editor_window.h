#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <components/custom_window.h>
#include <components/super_video_widget.h>

#include <QMenuBar>
#include <QAction>
#include <QShortcut>
#include <QStackedLayout>

#include "components/overlay_widget.h"


class EditorWindow : public CustomWindow
{
		Q_OBJECT

	public:
		explicit EditorWindow(QWidget *parent = nullptr);

		void doAddLayerOverlayCallback(const QString &answer);

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
		QShortcut *shortcutAddLayer;
		QShortcut *shortcutRemoveLayer;
		QShortcut *shortcutSelectUpperLayer;
		QShortcut *shortcutSelectLowerLayer;
		QShortcut *shortcutRecordSegment;
		QShortcut *shortcutNewLeftBound;
		QShortcut *shortcutNewRightBound;

		void setupShortcuts();
		void doTogglePlayback();
		void doAddLayer();
		void doRemoveLayer();
		void doNewLeftBound();
		void doNewRightBound();

		QMenuBar *menuBar;
		SuperVideoWidget *videoTab;
		OverlayWidget *overlay;
		QStackedLayout *layout;

		void setupMenu();
};


#endif // EDITORWINDOW_H
