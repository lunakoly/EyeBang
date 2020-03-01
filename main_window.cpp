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

	connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePlaybackPosition);
	connect(ui->playbackSlider, &QSlider::valueChanged, this, &MainWindow::setPlaybackPosition);
	connect(ui->playbackSlider, &QSlider::sliderPressed, this, &MainWindow::playbackSliderPress);
	connect(ui->playbackSlider, &QSlider::sliderReleased, this, &MainWindow::playbackSliderRelease);

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

void MainWindow::updatePlaybackPosition(qint64 position)
{
	if (!isSliderPressed)
	{
		double state = ((double) position) / mediaPlayer.duration();
		qint64 length = ui->playbackSlider->maximum() - ui->playbackSlider->minimum();
		qint64 desiredPosition = (qint64) (state * length);
		ui->playbackSlider->setValue(desiredPosition);

		ui->timeLabel->setText("State: " + QString::number(state));
	}
}

void MainWindow::setPlaybackPosition(qint64 position)
{
	if (isSliderPressed)
	{
		qint64 length = ui->playbackSlider->maximum() - ui->playbackSlider->minimum();
		double state = ((double) position) / length;
		qint64 desiredPosition = (qint64) (state * mediaPlayer.duration());
		mediaPlayer.setPosition(desiredPosition);

		ui->timeLabel->setText("Phase: " + QString::number(state));
	}
}

void MainWindow::playbackSliderPress()
{
	if (mediaPlayer.state() == QMediaPlayer::PlayingState)
	{
		wasPlayingBeforeSLiderPress = true;
		pause();
	}
	else
	{
		wasPlayingBeforeSLiderPress = false;
	}

	isSliderPressed = true;
}

void MainWindow::playbackSliderRelease()
{
	if (wasPlayingBeforeSLiderPress)
	{
		play();
	}

	isSliderPressed = false;

	// because changed is not really accurate
//	setPlaybackPosition(ui->playbackSlider->value());
}

