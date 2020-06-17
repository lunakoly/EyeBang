#ifndef SUPERVIDEOWIDGET_H
#define SUPERVIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QScrollArea>

#include "timeline.h"
#include "layer_select_button.h"


/**
 * Holds the video widget, the timeline
 * and other related things.
 */
class VideoTab : public QWidget
{
		Q_OBJECT

	public:
		explicit VideoTab(QWidget *parent = nullptr);

		/**
		 * Loads the video from a file.
		 */
		void loadFromVideoFile(const QString &filename);

		/**
		 * Returns the timeline. This is needed
		 * for the user to access the layer functionality
		 * of the timeline.
		 * TODO: review required.
		 */
		Timeline *getTimeline();

		/**
		 * Plays the video.
		 */
		void play();
		/**
		 * Pauses the video.
		 */
		void pause();
		/**
		 * Plays the video if paused.
		 * Pauses otherwise.
		 */
		void togglePlayback();

		/**
		 * Makes a small step to the left.
		 */
		void stepLeft();
		/**
		 * Makes a small step to the right.
		 */
		void stepRight();
		/**
		 * Makes a big step to the left.
		 */
		void jumpLeft();
		/**
		 * Makes a big step to the right.
		 */
		void jumpRight();

		// from editor_window shortcuts
		/**
		 * Switches the the previous layer.
		 */
		void doSelectUpperLayer();
		/**
		 * Switches the the next layer.
		 */
		void doSelectLowerLayer();

	protected:
		QMediaPlayer *mediaPlayer;
		QVideoWidget *videoWidget;
		Timeline *timeline;

		// for some reason I couldn't get
		// children of layersList so I started
		// parenting layer selection buttons to heightProvider
		// and using findChildren<>() over it
		QWidget *heightProvider;
		QVBoxLayout *layersList;

	private:
		QLabel *cursorTimeLabel;
		QLabel *totalTimeLabel;

		// needed to integrate the slider
		// with the video widget
		bool wasPlayingBeforeSliderPress = false;
		bool isSliderPressed = false;

		// helps to set things up
		QLayout *createDetails();
		QScrollArea *createLayersArea();

		// helps to get the unified time representation
		QString timeToString(qint64 time);

		// slider == simplified timeline.
		// but maybe we should change these names
		// to `timelinePress/Release`
		void videoSliderPress();
		void videoSliderRelease();

		// updates the max of the slider/timeline
		void mediaPlayerDurationChanged(qint64 duration);

		void mediaPlayerPositionChanged(qint64 position);
		void videoSliderValueChanged(qint64 position);

		// update the radio buttons when
		// layers change somehow
		void updateLayerAdded(Layer *layer);
		void selectLayerByButton(Layer *layer);
		void updateLayerRemoved(Layer *layer);
		void updateCurrentLayerChanged(Layer *newLayer);
};


#endif // SUPERVIDEOWIDGET_H
