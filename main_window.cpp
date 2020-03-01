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
	connect(ui->buttonStop, &QAbstractButton::clicked, this, &MainWindow::stop);

	ui->videoWidget->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this, "Choose a video file");
	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
		return;
	}

	setWindowTitle("EyeBang | " + filename);
	QMessageBox::information(this, "Done!", "File has been imported: " + filename);

	mediaPlayer.setMedia(QUrl::fromLocalFile(filename));
}

void MainWindow::save()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save");
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
		return;
	}

	setWindowTitle("EyeBang | " + filename);
	QMessageBox::information(this, "Done!", "File has been saved: " + filename);
}

void MainWindow::youtube()
{
	QMessageBox::information(this, "Download...", "Let's download a file from YouTube");
}

void MainWindow::about()
{
	QMessageBox::about(this, "About", "EyeBang v0.0.1");
}

void MainWindow::play()
{
	mediaPlayer.play();
}

void MainWindow::stop()
{
	mediaPlayer.stop();
}

