#include "video_tab.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDateTime>
#include <QScrollArea>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>

#include "layer_select_button.h"


#define STEP_SIZE 100
#define JUMP_SIZE 1000


QLayout *VideoTab::createDetails()
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

QScrollArea *VideoTab::createLayersArea()
{
	// here we have QScrollArea that must have
	// it's own widget (set via setWidget). Trying to
	// call layersListHolder->widget()->setSizePolicy() results
	// in a crash of the app. So we add our own QWidget
	// with the desired size policy and set it's layout as
	// QVBox.
	//
	// it's essential that we set our widget to a scroll area
	// only after we've set a layout for our widget. Otherwise
	// there might be cases when we won't see the widget (and
	// this is described in docs)
	//
	// setWidgetResizable() is important, otherwise size policy
	// won't work
	//
	// https://doc.qt.io/archives/qt-4.8/qscrollarea.html#setWidget

	heightProvider = new QWidget(this);
	heightProvider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	layersList = new QVBoxLayout(heightProvider);

	QScrollArea *layersListHolder = new QScrollArea(this);
	layersListHolder->setFixedWidth(250);
	layersListHolder->setWidgetResizable(true);
	layersListHolder->setWidget(heightProvider);

	return layersListHolder;
}

VideoTab::VideoTab(QWidget *parent) : QWidget(parent)
{
	mediaPlayer = new QMediaPlayer(this);
	mediaPlayer->setNotifyInterval(16);

	videoWidget = new QVideoWidget(this);
	videoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	timeline = new Timeline(this);
	timeline->setEnabled(false);

	QVBoxLayout *main = new QVBoxLayout();
	main->addWidget(videoWidget);
	main->addWidget(timeline);
	main->addLayout(createDetails());

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addLayout(main);
	layout->addWidget(createLayersArea());

	mediaPlayer->setVideoOutput(videoWidget);

	connect(timeline, &Timeline::sliderPressed,  this, &VideoTab::videoSliderPress);
	connect(timeline, &Timeline::sliderReleased, this, &VideoTab::videoSliderRelease);

	connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoTab::mediaPlayerDurationChanged);

	connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoTab::mediaPlayerPositionChanged);
	connect(timeline,    &Timeline::valueChanged,  this, &VideoTab::videoSliderValueChanged);

	connect(timeline, &Timeline::layerAdded,          this, &VideoTab::updateLayerAdded);
	connect(timeline, &Timeline::layerRemoved,        this, &VideoTab::updateLayerRemoved);
	connect(timeline, &Timeline::currentLayerChanged, this, &VideoTab::updateCurrentLayerChanged);
}

QString VideoTab::timeToString(qint64 time)
{
	return QDateTime::fromMSecsSinceEpoch(time).toUTC().toString("h:mm:ss.zzz");
}

void VideoTab::loadFromVideoFile(const QString &filename)
{
	mediaPlayer->setMedia(QUrl::fromLocalFile(filename));
	timeline->setEnabled(true);
	// now durationChanged is called
}

void VideoTab::mediaPlayerDurationChanged(qint64 duration)
{
	timeline->setMaximum(duration);
	totalTimeLabel->setText(tr("Total: ") + timeToString(duration));

	// start rendering the current frame instead
	// of just the gray box. 100 - just some random values
	// TODO: find better solution
	resize(100, 100);
}

Timeline *VideoTab::getTimeline()
{
	return timeline;
}

void VideoTab::play()
{
	mediaPlayer->play();
}

void VideoTab::pause()
{
	mediaPlayer->pause();
}

void VideoTab::togglePlayback()
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

void VideoTab::stepLeft()
{
	// isSeekable here to prevent scrolling before
	// a video is loaded into the mediaPlayers.
	// TODO: add something like	`isVideoAvailable()`.
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() - STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab::stepRight()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() + STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab::jumpLeft()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() - JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab::jumpRight()
{
	if (!isSliderPressed && mediaPlayer->isSeekable())
	{
		qint64 position = mediaPlayer->position() + JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab::videoSliderPress()
{
	wasPlayingBeforeSliderPress = mediaPlayer->state() == QMediaPlayer::PlayingState;

	if (wasPlayingBeforeSliderPress)
	{
		pause();
	}

	isSliderPressed = true;
}

void VideoTab::videoSliderRelease()
{
	if (wasPlayingBeforeSliderPress)
	{
		play();
	}

	isSliderPressed = false;
}

void VideoTab::mediaPlayerPositionChanged(qint64 position)
{
	if (!isSliderPressed)
	{
		double state = ((double) position) / mediaPlayer->duration();
		qint64 length = timeline->maximum() - timeline->minimum();
		qint64 desiredPosition = (qint64) (state * length);
		timeline->setValue(desiredPosition);

		cursorTimeLabel->setText(tr("Cursor: ") + timeToString(mediaPlayer->position()));
	}
}

void VideoTab::videoSliderValueChanged(qint64 position)
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

void VideoTab::updateLayerAdded(Layer *layer)
{
	LayerSelectButton *button = new LayerSelectButton(layer, heightProvider);
	layersList->addWidget(button);
	connect(button, &LayerSelectButton::activated, this, &VideoTab::selectLayerByButton);
	button->setChecked(true);
}

void VideoTab::selectLayerByButton(Layer *layer)
{
	timeline->setCurrentLayer(layer->name);
}

void VideoTab::updateLayerRemoved(Layer *layer)
{
	QList<LayerSelectButton *> buttons = heightProvider->findChildren<LayerSelectButton *>();

	if (!buttons.isEmpty())
	{
		int it = 0;

		while (it < buttons.size() && buttons[it]->layer != layer)
		{
			it += 1;
		}

		if (it < buttons.size())
		{
			layersList->removeWidget(buttons[it]);
		}
	}
}

void VideoTab::updateCurrentLayerChanged(Layer *newLayer)
{
	QList<LayerSelectButton *> buttons = heightProvider->findChildren<LayerSelectButton *>();

	if (!buttons.isEmpty())
	{
		int it = 0;

		while (it < buttons.size() && buttons[it]->layer != newLayer)
		{
			it += 1;
		}

		if (it < buttons.size())
		{
			buttons[it]->setChecked(true);
		}
	}
}

void VideoTab::doSelectUpperLayer()
{
	QList<LayerSelectButton *> buttons = heightProvider->findChildren<LayerSelectButton *>();

	if (!buttons.isEmpty())
	{
		int it = 0;

		while (it < buttons.size() && !buttons[it]->isChecked())
		{
			it += 1;
		}

		if (0 < it && it < buttons.size())
		{
			buttons[it - 1]->setChecked(true);
		}
	}
}

void VideoTab::doSelectLowerLayer()
{
	QList<LayerSelectButton *> buttons = heightProvider->findChildren<LayerSelectButton *>();

	if (!buttons.isEmpty())
	{
		int it = buttons.size() - 1;

		while (it >= 0 && !buttons[it]->isChecked())
		{
			it -= 1;
		}

		if (0 <= it && it < buttons.size() - 1)
		{
			buttons[it + 1]->setChecked(true);
		}
	}
}

