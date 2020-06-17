#include "editor_window_2.h"

#include "editing/project.h"

#include <QTabWidget>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>


void EditorWindow2::setupActions()
{
	actionAbout = new QAction(tr("About"), this);

	actionNewVideoFile = new QAction(tr("Video File"), this);
	actionNewYouTube   = new QAction(tr("YouTube"),    this);

	actionOpen = new QAction(tr("Open"), this);
	actionSave = new QAction(tr("Save"), this);

	actionOpen->setShortcut(QKeySequence(Qt::Key_O));
	actionSave->setShortcut(QKeySequence(Qt::Key_S));

	actionLoadRangesFile   = new QAction(tr("Load Ranges File"),   this);
	actionExportRangesFile = new QAction(tr("Export Ranges File"), this);

	connect(actionAbout, &QAction::triggered, this, &EditorWindow2::runAbout);

	connect(actionNewVideoFile, &QAction::triggered, this, &EditorWindow2::runNewVideoFile);
	connect(actionNewYouTube,   &QAction::triggered, this, &EditorWindow2::runNewYouTube);

	connect(actionOpen, &QAction::triggered, this, &EditorWindow2::runNewVideoFile);
	connect(actionSave, &QAction::triggered, this, &EditorWindow2::runSave);

	connect(actionLoadRangesFile,   &QAction::triggered, this, &EditorWindow2::runLoadRangesFile);
	connect(actionExportRangesFile, &QAction::triggered, this, &EditorWindow2::runExportRangesFile);
}

void EditorWindow2::setupMenu()
{
	menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

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

EditorWindow2::EditorWindow2() : OverlayWindow("Ranger")
{
	setupActions();
	setupMenu();

	QTabWidget *tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);
	setContentWidget(tabs);

	// shows either the VideoTab or
	// the welcome page
	QWidget *editingTabStack = new QWidget(this);
	tabs->addTab(editingTabStack, "Editing");

	editingTabLayout = new QStackedLayout(editingTabStack);

	videoTab = new VideoTab2(this);
	editingTabLayout->addWidget(videoTab);

	connect(videoTab, &VideoTab2::userInputTextRequired, this, &EditorWindow2::userInputText);

	QWidget *videoTabWelcomePage = new QWidget(this);
	editingTabLayout->addWidget(videoTabWelcomePage);

	QVBoxLayout *videoTabWelcomeLayout = new QVBoxLayout(videoTabWelcomePage);
	videoTabWelcomeLayout->setAlignment(Qt::AlignCenter);

	QLabel *editingTabWelcomMessage = new QLabel(tr("Create a new project from a video file"), this);
	videoTabWelcomeLayout->addWidget(editingTabWelcomMessage);

	editingTabLayout->setCurrentWidget(videoTabWelcomePage);
}

void EditorWindow2::runAbout()
{
	QMessageBox::about(this, tr("About"), tr("EyeBang v0.0.2"));
}

void EditorWindow2::runNewVideoFile()
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

	auto project = new Project(this);
	project->setVideoFile(filename);
	videoTab->loadProject(project);
	editingTabLayout->setCurrentIndex(0);
}

void EditorWindow2::runNewYouTube()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow2::runOpen()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow2::runSave()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow2::runLoadRangesFile()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow2::runExportRangesFile()
{
	QMessageBox::critical(this, tr("TODO"), tr("The feature hasn't been implemented."));
}

void EditorWindow2::userInputText(const QString &question, QObject *receiver, TextCallback callback)
{
	overlay->askForText(question, receiver, callback);
}
