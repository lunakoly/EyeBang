#ifndef VIDEOTAB2_H
#define VIDEOTAB2_H

#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QScrollArea>

#include "aliases.h"
#include "timeline.h"
#include "layer_select_button.h"
#include "editing/project.h"


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
		 * Loads the project and performs
		 * the required setup.
		 */
		void loadProject(Project *project);

		/**
		 * Returns the timeline. This simplifies
		 * connecting timeline signals and slots.
		 */
		Timeline *getTimeline();

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

		/**
		 * Switches the the previous layer.
		 */
		void selectUpperLayer();
		/**
		 * Switches the the next layer.
		 */
		void selectLowerLayer();

	private:
		// the loaded project
		Project *project;

		QMediaPlayer *mediaPlayer;
		Timeline *timeline;
		// for some reason I couldn't get
		// children of layersList so I started
		// parenting layer selection buttons to heightProvider
		// and using findChildren<>() over it
		QWidget *heightProvider;
		QVBoxLayout *layersList;

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
		void timelinePressed();
		void timelineReleased();

		// updates the max of the slider/timeline
		void mediaPlayerDurationChanged(qint64 duration);

		void mediaPlayerPositionChanged(qint64 position);
		void timelineValueChanged(qint64 position);

		// update the radio buttons when
		// layers change somehow
		void layerAdded(Layer *layer);
		void selectLayerByButton(Layer *layer);
		void layerRemoved(Layer *layer);
		void currentLayerChanged(Layer *newLayer);
		void layerRenamed(const QString &oldName, const QString &newName);
};


#endif // VIDEOTAB2_H
