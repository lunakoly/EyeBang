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

	private slots:
		void open();

		void save();

		void youtube();

		void about();

		void play();

		void pause();

		void togglePlayback();
};
#endif // MAINWINDOW_H
