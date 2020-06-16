#ifndef SUPERVIDEOWIDGET_H
#define SUPERVIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QScrollArea>

#include "timeline_widget.h"
#include "layer_select_button.h"


class SuperVideoWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit SuperVideoWidget(QWidget *parent = nullptr);

		void loadFromVideoFile(const QString &filename);

		TimelineWidget *getTimeline();

		void play();
		void pause();
		void togglePlayback();

		void stepLeft();
		void stepRight();
		void jumpLeft();
		void jumpRight();

		// from editor_window shortcuts
		void doSelectUpperLayer();
		void doSelectLowerLayer();

	protected:
		QMediaPlayer *mediaPlayer;
		QVideoWidget *videoWidget;
		TimelineWidget *timeline;

		// for some reason I couldn't get
		// children of layersList so I started
		// parenting buttons to heightProvider
		// and using findChildren<>() over it
		QWidget *heightProvider;
		QVBoxLayout *layersList;

	private:
		QLabel *cursorTimeLabel;
		QLabel *totalTimeLabel;

		bool wasPlayingBeforeSliderPress = false;
		bool isSliderPressed = false;

		QLayout *createDetails();
		QScrollArea *createLayersArea();

		QString timeToString(qint64 time);

		void videoSliderPress();
		void videoSliderRelease();

		void mediaPlayerDurationChanged(qint64 duration);

		void mediaPlayerPositionChanged(qint64 position);
		void videoSliderValueChanged(qint64 position);

		void updateLayerAdded(Layer *layer);
		void selectLayerByButton(Layer *layer);
		void updateLayerRemoved(Layer *layer);
		void updateCurrentLayerChanged(Layer *newLayer);
};


#endif // SUPERVIDEOWIDGET_H
