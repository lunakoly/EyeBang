#include "editor_window.h"
#include "components/super_video_widget.h"

#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QTabWidget>

#include <QStyleFactory>
#include <QApplication>
#include <QStyle>

#include <QFileDialog>
#include <QMessageBox>


void EditorWindow::setupAction()
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

	connect(shortcutTogglePlayback, &QShortcut::activated, this, &EditorWindow::doTogglePlayback);

	connect(shortcutStepLeft,  &QShortcut::activated, videoTab, &SuperVideoWidget::stepLeft);
	connect(shortcutStepRight, &QShortcut::activated, videoTab, &SuperVideoWidget::stepRight);
	connect(shortcutJumpLeft,  &QShortcut::activated, videoTab, &SuperVideoWidget::jumpLeft);
	connect(shortcutJumpRight, &QShortcut::activated, videoTab, &SuperVideoWidget::jumpRight);
}

EditorWindow::EditorWindow(QWidget *parent) : CustomWindow(tr("EyeBang"), parent)
{
	setupAction();
	setupMenu();

	videoTab = new SuperVideoWidget(this);

	QTabWidget *tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);
	tabs->addTab(videoTab, tr("Video Tab"));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMenuBar(menuBar);
	layout->addWidget(tabs);

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

