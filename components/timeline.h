#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

#include <QMediaPlayer>
#include <QAbstractVideoSurface>
#include <QHash>

#include "layer.h"
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
		// TODO: make proper access
		Overlay *overlay = nullptr;

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
		 * Instructs the timeline to create
		 * a new layer with the specified name.
		 */
		Layer *addLayer(const QString &name);
		/**
		 * Instructs the timeline to remove
		 * a new layer with the specified name.
		 * Does nothing if no such a layer found.
		 */
		Layer *removeLayer(const QString &name);

		/**
		 * Returns the layer with the specified
		 * name or nullptr.
		 */
		Layer *getLayer(const QString &name);
		/**
		 * Returns the list of all layers.
		 */
		QList<Layer *> getLayers();

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
		Layer *setCurrentLayer(const QString &name);
		/**
		 * Removes the currently selected layer
		 * or does nothing if no layer selected.
		 */
		Layer *removeCurrentLayer();

		/**
		 * Starts/stops the recording mode.
		 * During the recording mode a user is
		 * able to select some range and turn it
		 * into a segment. A new layer is created
		 * automatically if no one is selected.
		 */
		void toggleRecord();
		/**
		 * If there wasn't a selected layer by the time
		 * the recording mode finished, the timeline
		 * prompts the user for a name of the new layer.
		 * Then this function must be called with this
		 * layer name as the parameter.
		 * (So, it's a callback).
		 */
		void finishNewLayer(const QString &text);

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
		 * Called when a new layer is created.
		 */
		void layerAdded(Layer *layer);
		/**
		 * Called a new layer is removed.
		 */
		void layerRemoved(Layer *layer);
		/**
		 * Called when another layer is selected.
		 */
		void currentLayerChanged(Layer *newLayer);

	protected:
		int minimumValue = 0;
		int maximumValue = 100;

		int currentValue = 0;

		Layer *currentLayer = nullptr;

		// currentValue when the user
		// started the recording
		int recordingStart = 0;
		int isRecording = false;

		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;

		QHash<QString, Layer*> layers;

	private:
		// the last recorded segment.
		// we need to wait until the user
		// types in the name for the layer
		// in order to add this segment
		// to that layer, but up until then
		// we must store it somewhere
		Segment recordedSegment{0,0};

		// updates the value based on mouse position
		// relative to the widget
		void recalculateCurrent(qreal position);
};


#endif // TIMELINEWIDGET_H
