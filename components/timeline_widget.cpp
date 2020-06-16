#include "timeline_widget.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <QPainterPath>


TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent)
{

}

QSize TimelineWidget::sizeHint() const
{
	return QSize(70, 70);
}

int TimelineWidget::minimum()
{
	return minimumValue;
}

void TimelineWidget::setMinimum(int value)
{
	minimumValue = value;

	if (value > currentValue)
	{
		currentValue = value;
	}
}

int TimelineWidget::maximum()
{
	return maximumValue;
}

void TimelineWidget::setMaximum(int value)
{
	maximumValue = value;

	if (value < currentValue)
	{
		currentValue = value;
	}
}

int TimelineWidget::value()
{
	return currentValue;
}

void TimelineWidget::setValue(int value)
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

Layer *TimelineWidget::addLayer(const QString &name)
{	
	if (!layers.contains(name))
	{
		layers[name] = new Layer(name, this);
	}

	auto it = layers[name];
	emit layerAdded(it);

	if (currentLayer == nullptr)
	{
		currentLayer = it;
		emit currentLayerChanged(it);
		repaint();
	}

	return it;
}

Layer *TimelineWidget::removeLayer(const QString &name)
{
	if (layers.contains(name))
	{
		auto it = layers[name];
		layers.remove(name);
		emit layerRemoved(it);

		if (currentLayer == it)
		{
			currentLayer = nullptr;

			if (!layers.isEmpty())
			{
				currentLayer = layers.values().first();
			}

			emit currentLayerChanged(currentLayer);
			repaint();
		}

		return it;
	}

	return nullptr;
}

Layer *TimelineWidget::getLayer(const QString &name)
{
	if (layers.contains(name))
	{
		return layers[name];
	}

	return nullptr;
}

QList<Layer *> TimelineWidget::getLayers()
{
	return layers.values();
}

Layer *TimelineWidget::getCurrentLayer()
{
	return currentLayer;
}

Layer *TimelineWidget::setCurrentLayer(const QString &name)
{
	if (layers.contains(name))
	{
		currentLayer = layers[name];
		emit currentLayerChanged(currentLayer);
		repaint();
		return currentLayer;
	}

	return nullptr;
}

Layer *TimelineWidget::removeCurrentLayer()
{
	if (currentLayer != nullptr)
	{
		auto result = removeLayer(currentLayer->name);
		emit currentLayerChanged(nullptr);
		repaint();
		return result;
	}

	return nullptr;
}

void TimelineWidget::toggleRecord()
{
	if (overlay != nullptr && !overlay->isHidden())
	{
		return;
	}

	if (currentLayer != nullptr && !currentLayer->canStart(currentValue))
	{
		return;
	}

	if (isRecording)
	{
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
			if (overlay != nullptr)
			{
				overlay->askForText("New Layer Name", this, (OverlayWidget::AnswerReceiver) &TimelineWidget::finishNewLayer);
			}
		}
	}
	else
	{
		recordingStart = currentValue;
	}

	isRecording = !isRecording;
	repaint();
}

void TimelineWidget::finishNewLayer(const QString &text)
{
	auto layer = addLayer(text);
	layer->addSegment(recordedSegment);
	qDebug() << "End" << recordedSegment.end;
	repaint();
}

void TimelineWidget::paintEvent(QPaintEvent *event)
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

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	if (isEnabled())
	{
		emit sliderPressed();
		QPointF position = event->localPos();
		recalculateCurrent(position.x());
	}
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	if (isEnabled())
	{
		emit sliderReleased();
	}
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		QPointF position = event->localPos();
		recalculateCurrent(position.x());
	}
}

void TimelineWidget::recalculateCurrent(qreal position)
{
	int value = qRound((double) position / width() * (maximumValue - minimumValue)) + minimumValue;
	setValue(value);
}
