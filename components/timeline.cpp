#include "timeline.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <QPainterPath>


#define INDENT 4
#define TOP_ZONE 15
#define MINIMUM_ZONE 1000
#define CURSOR_WIDTH 2
#define ROUNDING 5

#define SCROLL_STEP 100


Timeline::Timeline(QWidget *parent) : QWidget(parent)
{

}

QSize Timeline::sizeHint() const
{
	return QSize(70, 70);
}

int Timeline::minimum()
{
	return minimumValue;
}

void Timeline::setMinimum(int value)
{
	minimumValue = value;

	if (value > currentValue)
	{
		currentValue = value;
	}
}

int Timeline::maximum()
{
	return maximumValue;
}

void Timeline::setMaximum(int value)
{
	maximumValue = value;

	if (value < currentValue)
	{
		currentValue = value;
	}
}

int Timeline::value()
{
	return currentValue;
}

void Timeline::setValue(int value)
{
	int oldValue = currentValue;
	currentValue = value;

	if (currentValue < minimumValue)
	{
		currentValue = minimumValue;
	}
	else if (currentValue > maximumValue)
	{
		currentValue = maximumValue;
	}

	if (oldValue != currentValue)
	{
		repaint();
		emit valueChanged(currentValue);
	}
}

qreal Timeline::getZoom()
{
	return zoom;
}

void Timeline::setZoom(qreal value)
{
	// remember where the center was
	int centerValue = positionToValue(0) + getVisibleZone() / 2;
	// extra space here is just for fun
	int maxZoom = (maximumValue - minimumValue) / MINIMUM_ZONE * 1.5;

	zoom = value;

	if (zoom < 0)
	{
		zoom = 0;
	}
	else if (zoom > maxZoom)
	{
		zoom = maxZoom;
	}

	// make the center be in the center
	scroll = centerValue - getVisibleZone() / 2;
	emit zoomed();
	repaint();
}

void Timeline::centerContent()
{
	scroll = (maximumValue - minimumValue) / 2 - getVisibleZone() / 2;
	setZoom((maximumValue - minimumValue) / MINIMUM_ZONE * 1.05);
}

Layer *Timeline::getCurrentLayer()
{
	return currentLayer;
}

Layer *Timeline::setCurrentLayer(Layer *layer)
{
	auto oldLayer = currentLayer;
	currentLayer = layer;

	if (oldLayer != layer)
	{
		emit currentLayerChanged(currentLayer);
		repaint();
	}

	return currentLayer;
}

void Timeline::toggleRecord()
{
	if (!isEnabled())
	{
		return;
	}

	if (isRecording)
	{
		isRecording = false;

		if (currentValue >= recordingStart)
		{
			recordedSegment = {recordingStart, currentValue};
		}
		else
		{
			recordedSegment = {currentValue, recordingStart};
		}

		if (currentLayer != nullptr)
		{
			currentLayer->addSegment(recordedSegment);
		}
		else
		{
			emit requestAddNewLayer();
			isReadyToSaveRecording = true;
		}
	}
	else
	{
		recordingStart = currentValue;
		isRecording = true;
	}

	// we can't write:
	// isRecording = !isRecording;
	// here because emit will trigger
	// the repaint and repaint will both
	// see the new layer and the selection box.
	// although, we may say here:
	// isRecording = !isRecording;
	// repaint();
}

void Timeline::stepScrollLeft()
{
	scroll -= SCROLL_STEP * getVisibleRatio();
	repaint();
}

void Timeline::stepScrollRight()
{
	scroll += SCROLL_STEP * getVisibleRatio();
	repaint();
}

void Timeline::layerAdded(Layer *layer)
{
	connect(layer, &Layer::segmentAdded,     this, &Timeline::segmentAdded);
	connect(layer, &Layer::segmentRemoved,   this, &Timeline::segmentRemoved);
	connect(layer, &Layer::segmentsModified, this, &Timeline::segmentsModified);

	if (isReadyToSaveRecording)
	{
		layer->addSegment(recordedSegment);
		isReadyToSaveRecording = false;
	}
}

void Timeline::layerRemoved(Layer *layer)
{
	disconnect(layer, &Layer::segmentAdded,     this, &Timeline::segmentAdded);
	disconnect(layer, &Layer::segmentRemoved,   this, &Timeline::segmentRemoved);
	disconnect(layer, &Layer::segmentsModified, this, &Timeline::segmentsModified);
}

void Timeline::notifyAddLayerCanceled()
{
	isReadyToSaveRecording = false;
}

qreal Timeline::getVisibleZone()
{
	return MINIMUM_ZONE * (1 + zoom);
}

qreal Timeline::getVisibleRatio()
{
	return getVisibleZone() / rect().width();
}

int Timeline::valueToPosition(int value)
{
	return round((value - scroll) / getVisibleRatio());
}

int Timeline::positionToValue(int position)
{
	return round((qreal) position * getVisibleRatio() + scroll);
}

void Timeline::wheelEvent(QWheelEvent *event)
{
	if (isEnabled())
	{
		QPoint pixelsDelta = event->pixelDelta();
		QPoint degreesDelta = event->angleDelta() / 8;

		if (!pixelsDelta.isNull())
		{
			setZoom(zoom - pixelsDelta.y());
		}
		else if (!degreesDelta.isNull())
		{
			setZoom(zoom - degreesDelta.y() / 15);
		}
	}
}

void Timeline::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();

	// background
	painter.setBrush(QColor("#424242"));
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(0, 0, rect().width(), TOP_ZONE, ROUNDING, ROUNDING);
	painter.drawRect(0, TOP_ZONE - ROUNDING, rect().width(), ROUNDING);

	// background strips
	int current = ROUNDING;
	int total = (rect().width() - ROUNDING * 2) / 4 + 1;
	int shift = (int) round(scroll / getVisibleRatio()) % 4;

	painter.setPen(QPen(QColor("#666"), 1));
//	painter.setPen(QPen(QColor("#353535"), 1));

	for (int it = 0; it < total; it++)
	{
		painter.drawLine(current - shift, 0, current - shift, TOP_ZONE);
		current += 4;
	}

	painter.setBrush(QColor("#383838"));
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(0, TOP_ZONE, rect().width(), rect().height() - TOP_ZONE, ROUNDING, ROUNDING);
	painter.drawRect(0, TOP_ZONE, rect().width(), ROUNDING);

	// the whole video track
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor("#2c2c2c"));

	int beginPosition = valueToPosition(0);
	int   endPosition = valueToPosition(maximumValue);
	painter.drawRect(beginPosition, TOP_ZONE + 10, endPosition - beginPosition, rect().height() - TOP_ZONE - 20);

	// layers
	if (currentLayer != nullptr)
	{
		painter.setPen(QPen(QColor(255, 253, 135), 2));
		painter.setBrush(QColor(255, 253, 135, 200));

		for (auto &it : currentLayer->getSegments())
		{
			int beginPosition = valueToPosition(it.begin);
			int   endPosition = valueToPosition(it.end);
			painter.drawRect(beginPosition, TOP_ZONE + 10, endPosition - beginPosition, rect().height() - TOP_ZONE - 20);
		}
	}

	// recording segment
	if (isRecording)
	{
		int        currentPosition = valueToPosition(currentValue);
		int recordingStartPosition = valueToPosition(recordingStart);

		painter.setPen(QPen(QColor(255, 253, 255), 2));
		painter.setBrush(QColor(255, 255, 255, 200));

		if (currentPosition < recordingStartPosition)
		{
			painter.drawRect(currentPosition, TOP_ZONE + 5, recordingStartPosition - currentPosition, rect().height() - TOP_ZONE - 10);
		}
		else
		{
			painter.drawRect(recordingStartPosition, TOP_ZONE + 5, currentPosition - recordingStartPosition, rect().height() - TOP_ZONE - 10);
		}

		painter.setPen(QPen(QColor("#aaaaaa"), 1));
		painter.drawLine(recordingStartPosition, TOP_ZONE, recordingStartPosition, rect().height());
	}

	// red cursor
	if (isEnabled())
	{
		// cursor vertical line
		int position = valueToPosition(currentValue);

		if (position < -2)
		{
			// left arrow
			QPainterPath cursorTop;
			cursorTop.moveTo(0, 7);
			cursorTop.lineTo(8, 4);
			cursorTop.lineTo(8, 11);
			cursorTop.closeSubpath();

			painter.setPen(QPen(QColor("#ff2222"), 1));
			painter.setBrush(Qt::red);
			painter.drawPath(cursorTop);
		}
		else if (position > rect().width() + 2)
		{
			// left arrow
			QPainterPath cursorTop;
			cursorTop.moveTo(rect().width() - 1, 7);
			cursorTop.lineTo(rect().width() - 9, 4);
			cursorTop.lineTo(rect().width() - 9, 11);
			cursorTop.closeSubpath();

			painter.setPen(QPen(QColor("#ff2222"), 1));
			painter.setBrush(Qt::red);
			painter.drawPath(cursorTop);
		}
		else
		{
			// cursor top triangle
			painter.setPen(QPen(Qt::red, CURSOR_WIDTH));
			painter.drawLine(position, rect().height(), position, TOP_ZONE);

			QPainterPath cursorTop;
			cursorTop.moveTo(position, TOP_ZONE);
			cursorTop.lineTo(position - 3, 0);
			cursorTop.lineTo(position + 3, 0);
			cursorTop.closeSubpath();

			painter.setPen(QPen(QColor("#ff2222"), 1));
			painter.setBrush(Qt::red);
			painter.drawPath(cursorTop);
		}
	}

	painter.restore();
}

void Timeline::mousePressEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		if (event->buttons() == Qt::LeftButton)
		{
			emit sliderPressed();
			QPointF position = event->localPos();
			setValue(positionToValue(position.x()));
		}
		else if (event->button() == Qt::MidButton)
		{
			isScrolling = true;
			oldPosition = event->pos().x();
			oldScroll = scroll;
		}
	}
}

void Timeline::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	if (isEnabled())
	{
		emit sliderReleased();
	}

	isScrolling = false;
}

void Timeline::mouseMoveEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		if (isScrolling)
		{
			scroll = oldScroll - (event->pos().x() - oldPosition) * getVisibleRatio();
			repaint();
		}
		else
		{
			QPointF position = event->localPos();
			setValue(positionToValue(position.x()));
		}
	}
}

void Timeline::segmentAdded(const Segment &)
{
	repaint();
}

void Timeline::segmentRemoved(Segment)
{
	repaint();
}

void Timeline::segmentsModified()
{
	repaint();
}
