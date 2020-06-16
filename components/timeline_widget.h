#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

#include <QMediaPlayer>
#include <QAbstractVideoSurface>
#include <QHash>

#include "layer.h"
#include "overlay_widget.h"


class TimelineWidget : public QWidget
{
		Q_OBJECT

	public:
		// TODO: make proper access
		OverlayWidget *overlay = nullptr;

		explicit TimelineWidget(QWidget *parent = nullptr);

		QSize sizeHint() const override;

		int getSingleStepValue() const;
		void setSingleStepValue(int value);

		int minimum();
		void setMinimum(int value);

		int maximum();
		void setMaximum(int value);

		int value();
		void setValue(int value);

		Layer *addLayer(const QString &name);
		Layer *removeLayer(const QString &name);

		Layer *getLayer(const QString &name);
		QList<Layer *> getLayers();

		Layer *getCurrentLayer();
		Layer *setCurrentLayer(const QString &name);
		Layer *removeCurrentLayer();

		void toggleRecord();

		void finishNewLayer(const QString &text);

	signals:
		void sliderPressed();
		void sliderReleased();
		void valueChanged(qint64 position);

		void layerAdded(Layer *layer);
		void layerRemoved(Layer *layer);
		void currentLayerChanged(Layer *newLayer);

	protected:
		int minimumValue = 0;
		int maximumValue = 100;

		int currentValue = 0;

		Layer *currentLayer = nullptr;

		int recordingStart = 0;
		int isRecording = false;

		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;

		QHash<QString, Layer*> layers;

	private:
		Segment recordedSegment{0,0};

		void recalculateCurrent(qreal position);
};


#endif // TIMELINEWIDGET_H
