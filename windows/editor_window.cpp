#include "editor_window.h"

#include "editing/project.h"

#include <QTabWidget>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>


EditorWindow::EditorWindow() : OverlayWindow("Ranger")
{
	menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

	QTabWidget *tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);
	setContentWidget(tabs);

	videoTabPlaceholder = new Placeholder(tr("Create a new project from a video file"), this);
	tabs->addTab(videoTabPlaceholder, "Editing");

	videoTab = new VideoTab(this);
	videoTabPlaceholder->setContentWidget(videoTab);

	connect(videoTab->getTimeline(), &Timeline::requestAddNewLayer, this, [&](){
		runAddLayer();
	});

	setupActions();
	setupMenu();
}

void EditorWindow::setupActions()
{
	actionAbout = new QAction(tr("About"), this);

	connect(actionAbout, &QAction::triggered, this, &EditorWindow::runAbout);

	actionNewVideoFile = new QAction(tr("Video File"), this);
	actionNewYouTube   = new QAction(tr("YouTube"),    this);

	connect(actionNewVideoFile, &QAction::triggered, this, &EditorWindow::runNewVideoFile);
	connect(actionNewYouTube,   &QAction::triggered, this, &EditorWindow::runNewYouTube);

	actionOpen = new QAction(tr("Open"), this);
	actionSave = new QAction(tr("Save"), this);

	actionOpen->setShortcut(QKeySequence(Qt::Key_O));
	actionSave->setShortcut(QKeySequence(Qt::Key_S));

	connect(actionOpen, &QAction::triggered, this, &EditorWindow::runNewVideoFile);
	connect(actionSave, &QAction::triggered, this, &EditorWindow::runSave);

	actionLoadRangesFile   = new QAction(tr("Load Ranges File"),   this);
	actionExportRangesFile = new QAction(tr("Export Ranges File"), this);

	connect(actionLoadRangesFile,   &QAction::triggered, this, &EditorWindow::runLoadRangesFile);
	connect(actionExportRangesFile, &QAction::triggered, this, &EditorWindow::runExportRangesFile);

	actionTogglePlayback = new QAction(tr("Toggle Playback"), this);

	actionTogglePlayback->setShortcut(QKeySequence(Qt::Key_Space));

	connect(actionTogglePlayback, &QAction::triggered, videoTab, &VideoTab::togglePlayback);

	actionStepLeft  = new QAction(tr("Step Left"),  this);
	actionStepRight = new QAction(tr("Step Right"), this);
	actionJumpLeft  = new QAction(tr("Jump Left"),  this);
	actionJumpRight = new QAction(tr("Jump Right"), this);

	actionStepLeft ->setShortcut(QKeySequence(Qt::Key_A + Qt::SHIFT));
	actionStepRight->setShortcut(QKeySequence(Qt::Key_D + Qt::SHIFT));
	actionJumpLeft ->setShortcut(QKeySequence(Qt::Key_A));
	actionJumpRight->setShortcut(QKeySequence(Qt::Key_D));

	connect(actionStepLeft,  &QAction::triggered, videoTab, &VideoTab::stepLeft);
	connect(actionStepRight, &QAction::triggered, videoTab, &VideoTab::stepRight);
	connect(actionJumpLeft,  &QAction::triggered, videoTab, &VideoTab::jumpLeft);
	connect(actionJumpRight, &QAction::triggered, videoTab, &VideoTab::jumpRight);

	actionAddLayer    = new QAction(tr("Add Layer"),    this);
	actionRemoveLayer = new QAction(tr("Remove Layer"), this);
	actionRenameLayer = new QAction(tr("Rename Layer"), this);

	actionAddLayer   ->setShortcut(QKeySequence(Qt::Key_L));
	actionRemoveLayer->setShortcut(QKeySequence(Qt::Key_X));
	actionRenameLayer->setShortcut(QKeySequence(Qt::Key_R));

	connect(actionAddLayer,    &QAction::triggered, this, &EditorWindow::runAddLayer);
	connect(actionRemoveLayer, &QAction::triggered, this, &EditorWindow::runRemoveLayer);
	connect(actionRenameLayer, &QAction::triggered, this, &EditorWindow::runRenameLayer);

	actionSelectUpperLayer = new QAction(tr("Select Upper Layer"), this);
	actionSelectLowerLayer = new QAction(tr("Select Lower Layer"), this);

	actionSelectUpperLayer->setShortcut(QKeySequence(Qt::Key_Up));
	actionSelectLowerLayer->setShortcut(QKeySequence(Qt::Key_Down));

	connect(actionSelectUpperLayer, &QAction::triggered, videoTab, &VideoTab::selectUpperLayer);
	connect(actionSelectLowerLayer, &QAction::triggered, videoTab, &VideoTab::selectLowerLayer);

	actionRecordSegment = new QAction(tr("Record Segment"), this);

	actionRecordSegment->setShortcut(QKeySequence(Qt::Key_G));

	connect(actionRecordSegment, &QAction::triggered, videoTab->getTimeline(), &Timeline::toggleRecord);

	actionNewLeftBound  = new QAction(tr("Set New Left Bound"),  this);
	actionNewRightBound = new QAction(tr("Set New Right Bound"), this);

	actionNewLeftBound ->setShortcut(QKeySequence(Qt::Key_BracketLeft));
	actionNewRightBound->setShortcut(QKeySequence(Qt::Key_BracketRight));

	connect(actionNewLeftBound,  &QAction::triggered, this, &EditorWindow::runNewLeftBound);
	connect(actionNewRightBound, &QAction::triggered, this, &EditorWindow::runNewRightBound);
}

void EditorWindow::setupMenu()
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
		editMenu->addAction(actionRenameLayer);
		editMenu->addSeparator();
		editMenu->addAction(actionSelectUpperLayer);
		editMenu->addAction(actionSelectLowerLayer);
		editMenu->addSeparator();
		editMenu->addAction(actionNewLeftBound);
		editMenu->addAction(actionNewRightBound);

	QMenu *helpMenu = menuBar->addMenu(tr("Help"));
		helpMenu->addAction(actionAbout);
}

void EditorWindow::runAbout()
{
	QMessageBox::about(this, tr("About"), tr("EyeBang v0.0.2"));
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

	project = new Project(this);
	project->setVideoFile(filename);
	videoTab->loadProject(project);
	videoTabPlaceholder->showContent();
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
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Export Ranges File"),
		"",
		tr("Ranges File (*.rng);;All Files (*)")
	);

	if (filename.isNull())
	{
		return;
	}

	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Can't open file: ") + file.errorString());
		return;
	}

	if (project != nullptr)
	{
		project->exportRangesFile(file);
	}

	file.close();
}

void EditorWindow::runAddLayer()
{
	if (project != nullptr)
	{
		overlay->askForText("New Layer Name", this, (TextCallback) &EditorWindow::receiveAddLayerName);
	}
}

void EditorWindow::receiveAddLayerName(bool isCanceled, const QString &name)
{
	if (isCanceled)
	{
		videoTab->getTimeline()->notifyAddLayerCanceled();
	}
	else if (project != nullptr)
	{
		if (project->containsLayer(name))
		{
			QMessageBox::warning(this, tr("Note"), tr("There's already a layer with such a name!"));
		}
		else
		{
			project->addLayer(name);
		}
	}
}

void EditorWindow::runRemoveLayer()
{
	if (project != nullptr)
	{
		project->removeLayer(videoTab->getTimeline()->getCurrentLayer()->getName());
	}
}

void EditorWindow::runRenameLayer()
{
	if (project != nullptr)
	{
		auto currentName = videoTab->getTimeline()->getCurrentLayer()->getName();
		QString message = tr("Rename `") + currentName + tr("` to");
		oldNameToReplace = currentName;

		overlay->askForText(message, this, (TextCallback) &EditorWindow::receiveRenameLayer);
	}
}

void EditorWindow::receiveRenameLayer(bool isCanceled, const QString &name)
{
	if (project != nullptr && !isCanceled)
	{
		project->rename(oldNameToReplace, name);
	}
}

void EditorWindow::runNewLeftBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewLeftBound(timeline->value());
	}
}

void EditorWindow::runNewRightBound()
{
	auto timeline = videoTab->getTimeline();

	if (timeline->getCurrentLayer() != nullptr)
	{
		timeline->getCurrentLayer()->setNewRightBound(timeline->value());
	}
}
