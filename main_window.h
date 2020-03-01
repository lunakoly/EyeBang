#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		QMediaPlayer mediaPlayer;

		QShortcut *togglePlaybackShortcut;
		QShortcut *stepToLefterFrameShortcut;
		QShortcut *stepToRighterFrameShortcut;
		QShortcut *jumpToLefterFrameShortcut;
		QShortcut *jumpToRighterFrameShortcut;

		bool wasPlayingBeforeSLiderPress = false;
		bool isSliderPressed = false;

		qint64 frameJumpSize = 200;

	private slots:
		void open();

		void save();

		void youtube();

		void about();

		void play();

		void pause();

		void togglePlayback();

		void updatePlaybackPosition(qint64);

		void setPlaybackPosition(qint64);

		void playbackSliderPress();

		void playbackSliderRelease();

		void stepToLefterFrame();

		void stepToRighterFrame();

		void jumpToLefterFrame();

		void jumpToRighterFrame();
};
#endif // MAINWINDOW_H
