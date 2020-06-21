#ifndef TIMELINE2_H
#define TIMELINE2_H

#include <QWidget>

#include <QMediaPlayer>
#include <QAbstractVideoSurface>
#include <QHash>

#include "editing/layer.h"
#include "overlay.h"


/**
 * A timeline for scrolling videos.
 * It incapsulates layer management
 * as well.
 */
class Timeline : public QWidget
{
		Q_OBJECT

	public:
		explicit Timeline(QWidget *parent = nullptr);

		// defines the preferred size
		// but layouts may ignore it if
		// they will
		QSize sizeHint() const override;

		/**
		 * Just a common way to get
		 * the minimum value.
		 */
		int minimum();
		/**
		 * Just a common way to set
		 * the minimum value.
		 */
		void setMinimum(int value);

		/**
		 * Just a common way to get
		 * the maximum value.
		 */
		int maximum();
		/**
		 * Just a common way to set
		 * the maximum value.
		 */
		void setMaximum(int value);

		/**
		 * Just a common way to get
		 * the current value.
		 */
		int value();
		/**
		 * Just a common way to set
		 * the current value.
		 */
		void setValue(int value);

		/**
		 * Returns the current zoom value.
		 */
		qreal getZoom();
		/**
		 * Updates the zoom value.
		 */
		void setZoom(qreal value);
		/**
		 * Makes the current track appear
		 * in the center with the proper zoom.
		 */
		void centerContent();

		/**
		 * Returns the currently selected layer
		 * or nullptr.
		 */
		Layer *getCurrentLayer();
		/**
		 * Makes the layer with the specified
		 * name be the current one. Does nothing
		 * if the layer hasn't been found.
		 */
		Layer *setCurrentLayer(Layer *layer);

		/**
		 * Starts/stops the recording mode.
		 * During the recording mode a user is
		 * able to select some range and turn it
		 * into a segment. A new layer is created
		 * automatically if no one is selected.
		 */
		void toggleRecord();

		/**
		 * Scrolls to the left a little bit.
		 */
		void stepScrollLeft();
		/**
		 * Scrolls to the right a little bit.
		 */
		void stepScrollRight();

		/**
		 * Shouls be bound to the layerAdded
		 * event of the project.
		 */
		void layerAdded(Layer *layer);
		/**
		 * Should be called then a layer
		 * is removed from the project.
		 */
		void layerRemoved(Layer *layer);

		/**
		 * Called after the recording requested
		 * new layer creation to notify
		 * the timeline that it needs to cancel
		 * the recording mode and discard the result.
		 */
		void notifyAddLayerCanceled();

	signals:
		/**
		 * Just a signal like the one we have
		 * in the usual QSlider.
		 */
		void sliderPressed();
		/**
		 * Just a signal like the one we have
		 * in the usual QSlider.
		 */
		void sliderReleased();
		/**
		 * Just a signal like the one we have
		 * in the usual QSlider.
		 */
		void valueChanged(qint64 position);
		/**
		 * Emitted when the zoom is changed.
		 */
		void zoomed();

		/**
		 * Called when another layer is selected.
		 */
		void currentLayerChanged(Layer *newLayer);

		/**
		 * Emitted when the user is asked to
		 * enter the name for the new layer.
		 */
		void requestAddNewLayer();

	private:
		int minimumValue = 0;
		int maximumValue = 100;
		int currentValue = 0;

		qreal zoom = 0;
		qreal scroll = 0;

		// returns the width of
		// the visible to the user
		// part of the track.
		qreal getVisibleZone();
		// alias for getVisibleZone() / rect().width
		qreal getVisibleRatio();
		// from virtual track space
		// to position relative to the
		// widget
		int valueToPosition(int value);
		// can be used by mousePressed
		// event to determine the proper
		// value for the mouse position
		int positionToValue(int position);

		// true when mid mouse click
		bool isScrolling = false;
		// initial position
		// for scrolling
		int oldPosition;
		qreal oldScroll;

		// handle zoom
		void wheelEvent(QWheelEvent *event) override;

		Layer *currentLayer = nullptr;

		// currentValue when the user
		// started the recording
		int recordingStart = 0;
		int isRecording = false;
		int isReadyToSaveRecording = false;

		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;

		QHash<QString, Layer*> layers;

		// the last recorded segment.
		// we need to wait until the user
		// types in the name for the layer
		// in order to add this segment
		// to that layer, but up until then
		// we must store it somewhere
		Segment recordedSegment{0,0};

		// these funcs trigger repaint()
		void segmentAdded(const Segment &);
		void segmentRemoved(Segment);
		void segmentsModified();
};


#endif // TIMELINE2_H
