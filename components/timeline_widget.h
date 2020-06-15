#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

#include <QMediaPlayer>
#include <QAbstractVideoSurface>


class TimelineWidget : public QWidget
{
		Q_OBJECT

	public:
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

	signals:
		void sliderPressed();
		void sliderReleased();
		void valueChanged(qint64 position);

	protected:
//		struct VideoFrameProber : public QAbstractVideoSurface
//		{
//			QMediaPlayer *mediaPlayer;
//			QVector<QPixmap> capturedFrames;

//			VideoFrameProber();

//			QSize videoSize();
//			qint64 duration();
//			void captureFrames(qint64 interval);

//			QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;
//			bool present(const QVideoFrame &frame) override;
//		};

//		VideoFrameProber frameProber;

		int minimumValue = 0;
		int maximumValue = 100;

		int currentValue = 0;

		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;

	private:
		void recalculateCurrent(qreal position);
};

#endif // TIMELINEWIDGET_H
