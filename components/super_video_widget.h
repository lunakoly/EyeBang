#ifndef SUPERVIDEOWIDGET_H
#define SUPERVIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>


class SuperVideoWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit SuperVideoWidget(QWidget *parent = nullptr);

		void loadFromVideoFile(const QString &filename);

		void play();
		void pause();
		void togglePlayback();

		void stepLeft();
		void stepRight();
		void jumpLeft();
		void jumpRight();

	protected:
		QMediaPlayer *mediaPlayer;
		QVideoWidget *videoWidget;
		QSlider *videoSlider;

	private:
		QLabel *cursorTimeLabel;
		QLabel *totalTimeLabel;

		bool wasPlayingBeforeSliderPress = false;
		bool isSliderPressed = false;

		QLayout *createDetails();

		QString timeToString(qint64 time);

		void videoSliderPress();
		void videoSliderRelease();

		void mediaPlayerDurationChanged(qint64 duration);

		void mediaPlayerPositionChanged(qint64 position);
		void videoSliderValueChanged(qint64 position);
};

#endif // SUPERVIDEOWIDGET_H
