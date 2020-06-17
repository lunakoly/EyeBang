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
#include "timeline_2.h"
#include "layer_select_button.h"
#include "editing/project.h"


/**
 * Holds the video widget, the timeline
 * and other related things.
 */
class VideoTab2 : public QWidget
{
		Q_OBJECT

	public:
		explicit VideoTab2(QWidget *parent = nullptr);

		/**
		 * Loads the project and performs
		 * the required setup.
		 */
		void loadProject(Project *project);

		/**
		 * Returns true if the video file is
		 * loaded and ready to be used.
		 */
		bool isMediaAvailable();

//		/**
//		 * Plays the video.
//		 */
//		void play();
//		/**
//		 * Pauses the video.
//		 */
//		void pause();
//		/**
//		 * Plays the video if paused.
//		 * Pauses otherwise.
//		 */
//		void togglePlayback();

//		/**
//		 * Makes a small step to the left.
//		 */
//		void stepLeft();
//		/**
//		 * Makes a small step to the right.
//		 */
//		void stepRight();
//		/**
//		 * Makes a big step to the left.
//		 */
//		void jumpLeft();
//		/**
//		 * Makes a big step to the right.
//		 */
//		void jumpRight();

		// from editor_window shortcuts
//		/**
//		 * Switches the the previous layer.
//		 */
//		void doSelectUpperLayer();
//		/**
//		 * Switches the the next layer.
//		 */
//		void doSelectLowerLayer();

	signals:
		/**
		 * Emitted when the user is asked to
		 * enter the name for the new layer.
		 */
		void userInputTextRequired(const QString &question, QObject *receiver, TextCallback callback);

	private:
		QShortcut *shortcutTogglePlayback;
		QShortcut *shortcutStepLeft;
		QShortcut *shortcutStepRight;
		QShortcut *shortcutJumpLeft;
		QShortcut *shortcutJumpRight;
		QShortcut *shortcutAddLayer;
		QShortcut *shortcutRemoveLayer;
		QShortcut *shortcutSelectUpperLayer;
		QShortcut *shortcutSelectLowerLayer;
		QShortcut *shortcutRecordSegment;
		QShortcut *shortcutNewLeftBound;
		QShortcut *shortcutNewRightBound;

		void setupShortcuts();
		// `do` - it's a callback
		// for a shortcut
		void doAddLayer(const QString &name);
		void doSelectUpperLayer();
		void doSelectLowerLayer();

		// the loaded project
		Project *project;

		QMediaPlayer *mediaPlayer;
		Timeline2 *timeline;
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
		void updateLayerAdded(Layer *layer);
		void selectLayerByButton(Layer *layer);
		void updateLayerRemoved(Layer *layer);
		void updateCurrentLayerChanged(Layer *newLayer);
};


#endif // VIDEOTAB2_H
