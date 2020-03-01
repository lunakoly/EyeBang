#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionOpen,		&QAction::triggered, this, &MainWindow::open);
	connect(ui->actionSave,		&QAction::triggered, this, &MainWindow::save);
	connect(ui->actionYouTube,	&QAction::triggered, this, &MainWindow::youtube);

	connect(ui->actionAbout,	&QAction::triggered, this, &MainWindow::about);

	mediaPlayer.setVideoOutput(ui->videoWidget);

	connect(ui->buttonPlay, &QAbstractButton::clicked, this, &MainWindow::play);
	connect(ui->buttonPause, &QAbstractButton::clicked, this, &MainWindow::pause);

	ui->videoWidget->show();
	ui->videoWidget->setStyleSheet("background-color: black;");

	ui->actionOpen->setShortcut(QKeySequence(Qt::Key_O));
	ui->actionSave->setShortcut(QKeySequence(Qt::Key_S));

	togglePlaybackShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
	connect(togglePlaybackShortcut, &QShortcut::activated, this, &MainWindow::togglePlayback);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete togglePlaybackShortcut;
}

void MainWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this, "Choose a video file");

	if (filename.isNull())
	{
		// cancel
		return;
	}

	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
		return;
	}

	setWindowTitle("EyeBang | " + filename);
	mediaPlayer.setMedia(QUrl::fromLocalFile(filename));
}

void MainWindow::save()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save");

	if (filename.isNull())
	{
		// cancel
		return;
	}

	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
		return;
	}

	setWindowTitle("EyeBang | " + filename);
	QMessageBox::information(this, "TODO", "TODO");
}

void MainWindow::youtube()
{
	QMessageBox::information(this, "TODO", "TODO");
}

void MainWindow::about()
{
	QMessageBox::about(this, "About", "EyeBang v0.0.1");
}

void MainWindow::play()
{
	mediaPlayer.play();
}

void MainWindow::pause()
{
	mediaPlayer.pause();
}

void MainWindow::togglePlayback()
{
	if (mediaPlayer.state() == QMediaPlayer::PlayingState)
	{
		pause();
	}
	else
	{
		play();
	}
}

