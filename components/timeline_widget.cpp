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

//	if (rect().width() != 0 && rect().height() != 0)
//	{
//		QSize videoSize = frameProber.videoSize();

//		qDebug() << "Video Size =" << videoSize;

//		qreal rescale = (double) videoSize.height() / rect().height();

//		qDebug() << "Rescale =" << rescale;

//		int previewWidth = qRound(videoSize.width() * rescale);
//		qint64 interval = qRound64((double) previewWidth / rect().width() * frameProber.duration());

//		qDebug() << "Frame width =" << previewWidth;
//		qDebug() << "Interval =" << interval;

//		frameProber.captureFrames(interval);

//		qDebug() << "Size =" << frameProber.capturedFrames.size();

//		for (int it = 0; it < frameProber.capturedFrames.size(); it++)
//		{
//			QRect place = rect();
//			place.setX(it * previewWidth);
//			place.setWidth(previewWidth);
//			painter.drawPixmap(place, frameProber.capturedFrames[it]);
//		}
//	}
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

//TimelineWidget::VideoFrameProber::VideoFrameProber() : QAbstractVideoSurface()
//{
//	mediaPlayer = new QMediaPlayer(this);
//	mediaPlayer->setVideoOutput(this);
//}

//QSize TimelineWidget::VideoFrameProber::videoSize()
//{
//	return mediaPlayer->metaData("Resolution").toSize();
//}

//qint64 TimelineWidget::VideoFrameProber::duration()
//{
//	return mediaPlayer->duration();
//}

//void TimelineWidget::VideoFrameProber::captureFrames(qint64 interval)
//{
//	capturedFrames.clear();
//	mediaPlayer->setPosition(0);
//	mediaPlayer->play();

//	qint64 current = 0;

//	while (current < mediaPlayer->duration())
//	{
//		mediaPlayer->setPosition(current);
//		current += interval;
//	}
//}

//QList<QVideoFrame::PixelFormat> TimelineWidget::VideoFrameProber::supportedPixelFormats(
//	QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::HandleType::NoHandle
//) const
//{
//	Q_UNUSED(type);
//	return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
//}

//bool TimelineWidget::VideoFrameProber::present(const QVideoFrame &frame)
//{
//	QImage::Format format = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

//	QImage image(
//		frame.bits(),
//		frame.width(),
//		frame.height(),
//		frame.bytesPerLine(),
//		format
//	);

//	QPixmap pixmap = QPixmap::fromImage(image);
//	capturedFrames.append(pixmap);

//	return true;
//}
