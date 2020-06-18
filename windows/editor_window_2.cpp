#include "editor_window_2.h"

#include "editing/project.h"

#include <QTabWidget>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>


EditorWindow2::EditorWindow2() : OverlayWindow("Ranger")
{
	menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

	QTabWidget *tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);
	setContentWidget(tabs);

	videoTabPlaceholder = new Placeholder(tr("Create a new project from a video file"), this);
	tabs->addTab(videoTabPlaceholder, "Editing");

	videoTab = new VideoTab2(this);
	videoTabPlaceholder->setContentWidget(videoTab);

	connect(videoTab->getTimeline(), &Timeline2::requestAddNewLayer, this, [&](){
		runAddLayer();
	});

	setupActions();
	setupMenu();
}

void EditorWindow2::setupActions()
{
	actionAbout = new QAction(tr("About"), this);

	connect(actionAbout, &QAction::triggered, this, &EditorWindow2::runAbout);

	actionNewVideoFile = new QAction(tr("Video File"), this);
	actionNewYouTube   = new QAction(tr("YouTube"),    this);

	connect(actionNewVideoFile, &QAction::triggered, this, &EditorWindow2::runNewVideoFile);
	connect(actionNewYouTube,   &QAction::triggered, this, &EditorWindow2::runNewYouTube);

	actionOpen = new QAction(tr("Open"), this);
	actionSave = new QAction(tr("Save"), this);

	actionOpen->setShortcut(QKeySequence(Qt::Key_O));
	actionSave->setShortcut(QKeySequence(Qt::Key_S));

	connect(actionOpen, &QAction::triggered, this, &EditorWindow2::runNewVideoFile);
	connect(actionSave, &QAction::triggered, this, &EditorWindow2::runSave);

	actionLoadRangesFile   = new QAction(tr("Load Ranges File"),   this);
	actionExportRangesFile = new QAction(tr("Export Ranges File"), this);

	connect(actionLoadRangesFile,   &QAction::triggered, this, &EditorWindow2::runLoadRangesFile);
	connect(actionExportRangesFile, &QAction::triggered, this, &EditorWindow2::runExportRangesFile);

	actionTogglePlayback = new QAction(tr("Toggle Playback"), this);

	actionTogglePlayback->setShortcut(QKeySequence(Qt::Key_Space));

	connect(actionTogglePlayback, &QAction::triggered, videoTab, &VideoTab2::togglePlayback);

	actionStepLeft  = new QAction(tr("Step Left"),  this);
	actionStepRight = new QAction(tr("Step Right"), this);
	actionJumpLeft  = new QAction(tr("Jump Left"),  this);
	actionJumpRight = new QAction(tr("Jump Right"), this);

	actionStepLeft ->setShortcut(QKeySequence(Qt::Key_A + Qt::SHIFT));
	actionStepRight->setShortcut(QKeySequence(Qt::Key_D + Qt::SHIFT));
	actionJumpLeft ->setShortcut(QKeySequence(Qt::Key_A));
	actionJumpRight->setShortcut(QKeySequence(Qt::Key_D));

	connect(actionStepLeft,  &QAction::triggered, videoTab, &VideoTab2::stepLeft);
	connect(actionStepRight, &QAction::triggered, videoTab, &VideoTab2::stepRight);
	connect(actionJumpLeft,  &QAction::triggered, videoTab, &VideoTab2::jumpLeft);
	connect(actionJumpRight, &QAction::triggered, videoTab, &VideoTab2::jumpRight);

	actionAddLayer    = new QAction(tr("Add Layer"),    this);
	actionRemoveLayer = new QAction(tr("Remove Layer"), this);

	actionAddLayer   ->setShortcut(QKeySequence(Qt::Key_L));
	actionRemoveLayer->setShortcut(QKeySequence(Qt::Key_R));

	connect(actionAddLayer,    &QAction::triggered, this, &EditorWindow2::runAddLayer);
	connect(actionRemoveLayer, &QAction::triggered, this, &EditorWindow2::runRemoveLayer);

	actionSelectUpperLayer = new QAction(tr("Select Upper Layer"), this);
	actionSelectLowerLayer = new QAction(tr("Select Lower Layer"), this);

	actionSelectUpperLayer->setShortcut(QKeySequence(Qt::Key_Up));
	actionSelectLowerLayer->setShortcut(QKeySequence(Qt::Key_Down));

	connect(actionSelectUpperLayer, &QAction::triggered, videoTab, &VideoTab2::selectUpperLayer);
	connect(actionSelectLowerLayer, &QAction::triggered, videoTab, &VideoTab2::selectLowerLayer);

	actionRecordSegment = new QAction(tr("Record Segment"), this);

	actionRecordSegment->setShortcut(QKeySequence(Qt::Key_G));

	connect(actionRecordSegment, &QAction::triggered, videoTab->getTimeline(), &Timeline2::toggleRecord);

	actionNewLeftBound  = new QAction(tr("Set New Left Bound"),  this);
	actionNewRightBound = new QAction(tr("Set New Right Bound"), this);

	actionNewLeftBound ->setShortcut(QKeySequence(Qt::Key_BracketLeft));
	actionNewRightBound->setShortcut(QKeySequence(Qt::Key_BracketRight));

	connect(actionNewLeftBound,  &QAction::triggered, this, &EditorWindow2::runNewLeftBound);
	connect(actionNewRightBound, &QAction::triggered, this, &EditorWindow2::runNewRightBound);
}

void EditorWindow2::setupMenu()
{
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

	QMenu *editMenu = menuBar->addMenu(tr("Edit"));
		editMenu->addAction(actionTogglePlayback);
		editMenu->addAction(actionRecordSegment);
		editMenu->addSeparator();
		editMenu->addAction(actionStepLeft);
		editMenu->addAction(actionStepRight);
		editMenu->addAction(actionJumpLeft);
		editMenu->addAction(actionJumpRight);
		editMenu->addSeparator();
		editMenu->addAction(actionAddLayer);
		editMenu->addAction(actionRemoveLayer);
		editMenu->addSeparator();
		editMenu->addAction(actionSelectUpperLayer);
		editMenu->addAction(actionSelectLowerLayer);
		editMenu->addSeparator();
		editMenu->addAction(actionNewLeftBound);
		editMenu->addAction(actionNewRightBound);

	QMenu *helpMenu = menuBar->addMenu(tr("Help"));
		helpMenu->addAction(actionAbout);
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

	project = new Project(this);
	project->setVideoFile(filename);
	videoTab->loadProject(project);
	videoTabPlaceholder->showContent();
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

void EditorWindow2::runAddLayer()
{
	if (project != nullptr)
	{
		overlay->askForText("New Layer Name", this, (TextCallback) &EditorWindow2::receiveNewLayerName);
	}
}

void EditorWindow2::receiveNewLayerName(bool isCanceled, const QString &name)
{
	if (isCanceled)
	{
		videoTab->getTimeline()->notifyAddLayerCanceled();
	}
	else if (project != nullptr)
	{
		project->addLayer(name);
	}
}

void EditorWindow2::runRemoveLayer()
{
	if (project != nullptr)
	{
		project->removeLayer(videoTab->getTimeline()->getCurrentLayer()->name);
	}
}

void EditorWindow2::runNewLeftBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewLeftBound(timeline->value());
	}
}

void EditorWindow2::runNewRightBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewRightBound(timeline->value());
	}
}