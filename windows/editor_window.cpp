#include "editor_window.h"
#include "components/video_tab.h"

#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QTabWidget>
#include <QStackedLayout>
#include <QGraphicsAnchorLayout>

#include <QStyleFactory>
#include <QApplication>
#include <QStyle>

#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>


void EditorWindow::setupActions()
{
	actionAbout = new QAction(tr("About"), this);

	actionNewVideoFile = new QAction(tr("Video File"), this);
	actionNewYouTube   = new QAction(tr("YouTube"),    this);

	actionOpen = new QAction(tr("Open"), this);
	actionSave = new QAction(tr("Save"), this);

	actionLoadRangesFile   = new QAction(tr("Load Ranges File"),   this);
	actionExportRangesFile = new QAction(tr("Export Ranges File"), this);

	connect(actionAbout, &QAction::triggered, this, &EditorWindow::runAbout);

	connect(actionNewVideoFile, &QAction::triggered, this, &EditorWindow::runNewVideoFile);
	connect(actionNewYouTube,   &QAction::triggered, this, &EditorWindow::runNewYouTube);

	connect(actionOpen, &QAction::triggered, this, &EditorWindow::runNewVideoFile);
	connect(actionSave, &QAction::triggered, this, &EditorWindow::runSave);

	connect(actionLoadRangesFile,   &QAction::triggered, this, &EditorWindow::runLoadRangesFile);
	connect(actionExportRangesFile, &QAction::triggered, this, &EditorWindow::runExportRangesFile);
}

void EditorWindow::setupMenu()
{
	menuBar = new QMenuBar(this);

	// I found these indents handy
	QMenu *fileMenu = menuBar->addMenu(tr("File"));
		QMenu *fileNewMenu = fileMenu->addMenu(tr("New From..."));
			fileNewMenu->addAction(actionNewVideoFile);
			fileNewMenu->addAction(actionNewYouTube);
		fileMenu->addAction(actionOpen);
		fileMenu->addAction(actionSave);
		fileMenu->addSeparator();
		fileMenu->addAction(actionLoadRangesFile);
		fileMenu->addAction(actionExportRangesFile);

	QMenu *helpMenu = menuBar->addMenu(tr("Help"));
		helpMenu->addAction(actionAbout);
}

void EditorWindow::setupShortcuts()
{
	actionOpen->setShortcut(QKeySequence(Qt::Key_O));
	actionSave->setShortcut(QKeySequence(Qt::Key_S));

	shortcutTogglePlayback = new QShortcut(QKeySequence(Qt::Key_Space), this);

	shortcutStepLeft  = new QShortcut(QKeySequence(Qt::Key_A + Qt::SHIFT), this);
	shortcutStepRight = new QShortcut(QKeySequence(Qt::Key_D + Qt::SHIFT), this);
	shortcutJumpLeft  = new QShortcut(QKeySequence(Qt::Key_A),             this);
	shortcutJumpRight = new QShortcut(QKeySequence(Qt::Key_D),             this);

	shortcutAddLayer    = new QShortcut(QKeySequence(Qt::Key_L), this);
	shortcutRemoveLayer = new QShortcut(QKeySequence(Qt::Key_R), this);

	shortcutSelectUpperLayer = new QShortcut(QKeySequence(Qt::Key_Up), this);
	shortcutSelectLowerLayer = new QShortcut(QKeySequence(Qt::Key_Down), this);

	shortcutRecordSegment = new QShortcut(QKeySequence(Qt::Key_G), this);

	shortcutNewLeftBound  = new QShortcut(QKeySequence(Qt::Key_BracketLeft),  this);
	shortcutNewRightBound = new QShortcut(QKeySequence(Qt::Key_BracketRight), this);

	connect(shortcutTogglePlayback, &QShortcut::activated, this, &EditorWindow::doTogglePlayback);

	connect(shortcutStepLeft,  &QShortcut::activated, videoTab, &VideoTab::stepLeft);
	connect(shortcutStepRight, &QShortcut::activated, videoTab, &VideoTab::stepRight);
	connect(shortcutJumpLeft,  &QShortcut::activated, videoTab, &VideoTab::jumpLeft);
	connect(shortcutJumpRight, &QShortcut::activated, videoTab, &VideoTab::jumpRight);

	connect(shortcutAddLayer,    &QShortcut::activated, this, &EditorWindow::doAddLayer);
	connect(shortcutRemoveLayer, &QShortcut::activated, this, &EditorWindow::doRemoveLayer);

	connect(shortcutSelectUpperLayer, &QShortcut::activated, videoTab, &VideoTab::doSelectUpperLayer);
	connect(shortcutSelectLowerLayer, &QShortcut::activated, videoTab, &VideoTab::doSelectLowerLayer);

	connect(shortcutRecordSegment, &QShortcut::activated, videoTab->getTimeline(), &Timeline::toggleRecord);

	connect(shortcutNewLeftBound,  &QShortcut::activated, this, &EditorWindow::doNewLeftBound);
	connect(shortcutNewRightBound, &QShortcut::activated, this, &EditorWindow::doNewRightBound);
}

EditorWindow::EditorWindow(QWidget *parent) : CustomWindow(tr("EyeBang"), parent)
{
	setupActions();
	setupMenu();

	videoTab = new VideoTab(this);

	QTabWidget *tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);
	tabs->addTab(videoTab, tr("Video Tab"));

	overlay = new Overlay(this);
	// TODO: make proper access
	videoTab->getTimeline()->overlay = overlay;

	// we could've used absolute positioning
	// instead but int that case we'd need
	// to manually update overlay size
	layout = new QStackedLayout(this);
	layout->setStackingMode(QStackedLayout::StackAll);
	layout->setMenuBar(menuBar);
	layout->addWidget(overlay);
	layout->addWidget(tabs);

	layout->setCurrentWidget(overlay);
	overlay->setHidden(true);

	setupShortcuts();
}

void EditorWindow::runNewVideoFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Choose a video file"));

	if (filename.isNull())
	{
		return;
	}

	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Can't open file: ") + file.errorString());
		return;
	}

	setWindowTitle("EyeBang | " + filename);
	videoTab->loadFromVideoFile(filename);
}

void EditorWindow::runNewYouTube()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow::runOpen()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow::runSave()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow::runLoadRangesFile()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow::runExportRangesFile()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow::runAbout()
{
	QMessageBox::about(this, "About", "EyeBang v0.0.2");
}

void EditorWindow::doTogglePlayback()
{
	videoTab->togglePlayback();
}

void EditorWindow::doAddLayer()
{
	overlay->askForText("Layer Name", this, (Overlay::AnswerReceiver) &EditorWindow::doAddLayerOverlayCallback);
}

void EditorWindow::doAddLayerOverlayCallback(const QString &answer)
{
	if (!answer.isEmpty())
	{
		videoTab->getTimeline()->addLayer(answer);
	}
}

void EditorWindow::doRemoveLayer()
{
	videoTab->getTimeline()->removeCurrentLayer();
}

void EditorWindow::doNewLeftBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewLeftBound(timeline->value());
		timeline->repaint();
	}
}

void EditorWindow::doNewRightBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewRightBound(timeline->value());
		timeline->repaint();
	}
}

