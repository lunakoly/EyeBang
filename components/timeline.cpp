#include "timeline.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <QPainterPath>


Timeline2::Timeline2(QWidget *parent) : QWidget(parent)
{

}

QSize Timeline2::sizeHint() const
{
	return QSize(70, 70);
}

int Timeline2::minimum()
{
	return minimumValue;
}

void Timeline2::setMinimum(int value)
{
	minimumValue = value;

	if (value > currentValue)
	{
		currentValue = value;
	}
}

int Timeline2::maximum()
{
	return maximumValue;
}

void Timeline2::setMaximum(int value)
{
	maximumValue = value;

	if (value < currentValue)
	{
		currentValue = value;
	}
}

int Timeline2::value()
{
	return currentValue;
}

void Timeline2::setValue(int value)
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

Layer *Timeline2::getCurrentLayer()
{
	return currentLayer;
}

Layer *Timeline2::setCurrentLayer(Layer *layer)
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

void Timeline2::toggleRecord()
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

void Timeline2::layerAdded(Layer *layer)
{
	connect(layer, &Layer::segmentAdded,     this, &Timeline2::segmentAdded);
	connect(layer, &Layer::segmentRemoved,   this, &Timeline2::segmentRemoved);
	connect(layer, &Layer::segmentsModified, this, &Timeline2::segmentsModified);

	if (isReadyToSaveRecording)
	{
		layer->addSegment(recordedSegment);
		isReadyToSaveRecording = false;
	}
}

void Timeline2::layerRemoved(Layer *layer)
{
	disconnect(layer, &Layer::segmentAdded,     this, &Timeline2::segmentAdded);
	disconnect(layer, &Layer::segmentRemoved,   this, &Timeline2::segmentRemoved);
	disconnect(layer, &Layer::segmentsModified, this, &Timeline2::segmentsModified);
}

void Timeline2::notifyAddLayerCanceled()
{
	isReadyToSaveRecording = false;
}

void Timeline2::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	int cursorWidth = 2;
	int paddingTop = 15;
	int paddingSides = 4;
	int rounding = 5;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();

	// background
	painter.setBrush(QColor("#383838"));
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(0, paddingTop, rect().width(), rect().height() - paddingTop, rounding, rounding);
	painter.drawRect(0, paddingTop, rect().width(), rounding);

	painter.setBrush(QColor("#424242"));
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(0, 0, rect().width(), paddingTop, rounding, rounding);
	painter.drawRect(0, paddingTop - rounding, rect().width(), rounding);

	// background strips
	int current = paddingSides;
	int total = rect().width() - paddingSides * 2;
	int step = total / 80;

	// to prevent an infinite loop
	if (step == 0)
	{
		step = 10;
	}

	painter.setPen(QPen(QColor("#666"), 1));

	while (current < paddingSides + total)
	{
		painter.drawLine(current, 0, current, paddingTop);
		current += step;
	}

	// layers
	if (currentLayer != nullptr)
	{
		painter.setPen(QPen(QColor(255, 253, 135), 2));
		painter.setBrush(QColor(255, 253, 135, 200));

		for (auto &it : currentLayer->getSegments())
		{
			qreal beginState = (double) (it.begin - minimumValue) / (maximumValue - minimumValue);
			qreal   endState = (double) (it.end   - minimumValue) / (maximumValue - minimumValue);

			int beginPosition = qRound(beginState * (rect().width() - cursorWidth - 2 * paddingSides)) + cursorWidth / 2 + paddingSides;
			int   endPosition = qRound(  endState * (rect().width() - cursorWidth - 2 * paddingSides)) + cursorWidth / 2 + paddingSides;

			painter.drawRect(beginPosition, paddingTop + 10, endPosition - beginPosition, rect().height() - paddingTop - 20);
		}
	}

	// recording segment
	if (isRecording)
	{
		qreal        currentState = (double) (currentValue   - minimumValue) / (maximumValue - minimumValue);
		qreal recordingStartState = (double) (recordingStart - minimumValue) / (maximumValue - minimumValue);

		int        currentPosition = qRound(       currentState * (rect().width() - cursorWidth - 2 * paddingSides)) + cursorWidth / 2 + paddingSides;
		int recordingStartPosition = qRound(recordingStartState * (rect().width() - cursorWidth - 2 * paddingSides)) + cursorWidth / 2 + paddingSides;

		painter.setPen(QPen(QColor(255, 253, 255), 2));
		painter.setBrush(QColor(255, 255, 255, 200));

		if (currentPosition < recordingStartPosition)
		{
			painter.drawRect(currentPosition, paddingTop + 5, recordingStartPosition - currentPosition, rect().height() - paddingTop - 10);
		}
		else
		{
			painter.drawRect(recordingStartPosition, paddingTop + 5, currentPosition - recordingStartPosition, rect().height() - paddingTop - 10);
		}

		painter.setPen(QPen(QColor("#aaaaaa"), 1));
		painter.drawLine(recordingStartPosition, paddingTop, recordingStartPosition, rect().height());

	}

	// red cursor
	if (isEnabled())
	{
		// cursor vertical line
		qreal state = (double) (currentValue - minimumValue) / (maximumValue - minimumValue);
		int position = qRound(state * (rect().width() - cursorWidth - 2 * paddingSides)) + cursorWidth / 2 + paddingSides;

		painter.setPen(QPen(Qt::red, cursorWidth));
		painter.drawLine(position, rect().height(), position, paddingTop);

		// cursor top triangle
		QPainterPath cursorTop;
		cursorTop.moveTo(position, paddingTop);
		cursorTop.lineTo(position - 3, 0);
		cursorTop.lineTo(position + 3, 0);
		cursorTop.closeSubpath();

		painter.setPen(QPen(QColor("#ff2222"), 1));
		painter.setBrush(Qt::red);
		painter.drawPath(cursorTop);
	}

	painter.restore();
}

void Timeline2::mousePressEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	if (isEnabled())
	{
		emit sliderPressed();
		QPointF position = event->localPos();
		recalculateCurrent(position.x());
	}
}

void Timeline2::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	if (isEnabled())
	{
		emit sliderReleased();
	}
}

void Timeline2::mouseMoveEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		QPointF position = event->localPos();
		recalculateCurrent(position.x());
	}
}

void Timeline2::recalculateCurrent(qreal position)
{
	int value = qRound((double) position / width() * (maximumValue - minimumValue)) + minimumValue;
	setValue(value);
}

void Timeline2::segmentAdded(const Segment &)
{
	repaint();
}

void Timeline2::segmentRemoved(Segment)
{
	repaint();
}

void Timeline2::segmentsModified()
{
	repaint();
}
