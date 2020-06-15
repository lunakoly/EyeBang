#include "super_video_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDateTime>


#define STEP_SIZE 100
#define JUMP_SIZE 1000


QLayout *SuperVideoWidget::createDetails()
{
	cursorTimeLabel = new QLabel(tr("Cursor: ") + timeToString(0), this);

	QVBoxLayout *cursorDetails = new QVBoxLayout();
	cursorDetails->addWidget(cursorTimeLabel);

	totalTimeLabel = new QLabel(tr("Total: ") + timeToString(0), this);
	totalTimeLabel->setAlignment(Qt::AlignRight);

	QVBoxLayout *totalDetails = new QVBoxLayout();
	totalDetails->addWidget(totalTimeLabel);

	QHBoxLayout *details = new QHBoxLayout();
	details->addLayout(cursorDetails);
	details->addLayout(totalDetails);

	return details;
}

SuperVideoWidget::SuperVideoWidget(QWidget *parent) : QWidget(parent)
{
	mediaPlayer = new QMediaPlayer(this);

	videoWidget = new QVideoWidget(this);
	videoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

//	videoSlider = new QSlider(Qt::Horizontal, this);
//	videoSlider->setEnabled(false);

	timeline = new TimelineWidget(this);
	timeline->setEnabled(false);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(videoWidget);
//	layout->addWidget(videoSlider);
	layout->addWidget(timeline);
	layout->addLayout(createDetails());

	mediaPlayer->setVideoOutput(videoWidget);

//	connect(videoSlider, &QSlider::sliderPressed,  this, &SuperVideoWidget::videoSliderPress);
//	connect(videoSlider, &QSlider::sliderReleased, this, &SuperVideoWidget::videoSliderRelease);

	connect(timeline, &TimelineWidget::sliderPressed,  this, &SuperVideoWidget::videoSliderPress);
	connect(timeline, &TimelineWidget::sliderReleased, this, &SuperVideoWidget::videoSliderRelease);

	connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &SuperVideoWidget::mediaPlayerDurationChanged);

	connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &SuperVideoWidget::mediaPlayerPositionChanged);
//	connect(videoSlider, &QSlider::valueChanged,         this, &SuperVideoWidget::videoSliderValueChanged);

	connect(timeline, &TimelineWidget::valueChanged, this, &SuperVideoWidget::videoSliderValueChanged);
}

QString SuperVideoWidget::timeToString(qint64 time)
{
	return QDateTime::fromMSecsSinceEpoch(time).toUTC().toString("h:mm:ss.zzz");
}

void SuperVideoWidget::loadFromVideoFile(const QString &filename)
{
	mediaPlayer->setMedia(QUrl::fromLocalFile(filename));
//	videoSlider->setEnabled(true);
	timeline->setEnabled(true);
	// now durationChanged is called
}

void SuperVideoWidget::mediaPlayerDurationChanged(qint64 duration)
{
//	videoSlider->setMaximum(duration);
	timeline->setMaximum(duration);
	totalTimeLabel->setText(tr("Total: ") + timeToString(duration));

	// start rendering the current frame instead
	// of just the gray box. 100 - just some random values
	// TODO: find better solution
	resize(100, 100);
}

void SuperVideoWidget::play()
{
	mediaPlayer->play();
}

void SuperVideoWidget::pause()
{
	mediaPlayer->pause();
}

void SuperVideoWidget::togglePlayback()
{
	if (mediaPlayer->state() == QMediaPlayer::PlayingState)
	{
		pause();
	}
	else
	{
		play();
	}
}

void SuperVideoWidget::stepLeft()
{
	// isSeekable here to prevent scrolling before
	// a video is loaded into the mediaPlayers
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() - STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void SuperVideoWidget::stepRight()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() + STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void SuperVideoWidget::jumpLeft()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() - JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void SuperVideoWidget::jumpRight()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() + JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void SuperVideoWidget::videoSliderPress()
{
	wasPlayingBeforeSliderPress = mediaPlayer->state() == QMediaPlayer::PlayingState;

	if (wasPlayingBeforeSliderPress)
	{
		pause();
	}

	isSliderPressed = true;
}

void SuperVideoWidget::videoSliderRelease()
{
	if (wasPlayingBeforeSliderPress)
	{
		play();
	}

	isSliderPressed = false;
}

void SuperVideoWidget::mediaPlayerPositionChanged(qint64 position)
{
	if (!isSliderPressed)
	{
		double state = ((double) position) / mediaPlayer->duration();
		qint64 length = timeline->maximum() - timeline->minimum();
		qint64 desiredPosition = (qint64) (state * length);
//		videoSlider->setValue(desiredPosition);
		timeline->setValue(desiredPosition);

		cursorTimeLabel->setText(tr("Cursor: ") + timeToString(mediaPlayer->position()));
	}
}

void SuperVideoWidget::videoSliderValueChanged(qint64 position)
{
	if (isSliderPressed)
	{
		qint64 length = timeline->maximum() - timeline->minimum();
		double state = ((double) position) / length;
		qint64 desiredPosition = (qint64) (state * mediaPlayer->duration());
		mediaPlayer->setPosition(desiredPosition);

		cursorTimeLabel->setText(tr("Cursor: ") + timeToString(mediaPlayer->position()));
	}
}
