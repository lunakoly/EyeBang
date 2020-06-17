#include "super_video_widget.h"

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

QScrollArea *SuperVideoWidget::createLayersArea()
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

SuperVideoWidget::SuperVideoWidget(QWidget *parent) : QWidget(parent)
{
	mediaPlayer = new QMediaPlayer(this);
	mediaPlayer->setNotifyInterval(16);

	videoWidget = new QVideoWidget(this);
	videoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	timeline = new TimelineWidget(this);
	timeline->setEnabled(false);

	QVBoxLayout *main = new QVBoxLayout();
	main->addWidget(videoWidget);
	main->addWidget(timeline);
	main->addLayout(createDetails());

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addLayout(main);
	layout->addWidget(createLayersArea());

	mediaPlayer->setVideoOutput(videoWidget);

	connect(timeline, &TimelineWidget::sliderPressed,  this, &SuperVideoWidget::videoSliderPress);
	connect(timeline, &TimelineWidget::sliderReleased, this, &SuperVideoWidget::videoSliderRelease);

	connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &SuperVideoWidget::mediaPlayerDurationChanged);

	connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &SuperVideoWidget::mediaPlayerPositionChanged);
	connect(timeline,    &TimelineWidget::valueChanged,  this, &SuperVideoWidget::videoSliderValueChanged);

	connect(timeline, &TimelineWidget::layerAdded,          this, &SuperVideoWidget::updateLayerAdded);
	connect(timeline, &TimelineWidget::layerRemoved,        this, &SuperVideoWidget::updateLayerRemoved);
	connect(timeline, &TimelineWidget::currentLayerChanged, this, &SuperVideoWidget::updateCurrentLayerChanged);
}

QString SuperVideoWidget::timeToString(qint64 time)
{
	return QDateTime::fromMSecsSinceEpoch(time).toUTC().toString("h:mm:ss.zzz");
}

void SuperVideoWidget::loadFromVideoFile(const QString &filename)
{
	mediaPlayer->setMedia(QUrl::fromLocalFile(filename));
	timeline->setEnabled(true);
	// now durationChanged is called
}

void SuperVideoWidget::mediaPlayerDurationChanged(qint64 duration)
{
	timeline->setMaximum(duration);
	totalTimeLabel->setText(tr("Total: ") + timeToString(duration));

	// start rendering the current frame instead
	// of just the gray box. 100 - just some random values
	// TODO: find better solution
	resize(100, 100);
}

TimelineWidget *SuperVideoWidget::getTimeline()
{
	return timeline;
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
	// a video is loaded into the mediaPlayers.
	// TODO: add something like	`isVideoAvailable()`.
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

void SuperVideoWidget::updateLayerAdded(Layer *layer)
{
	LayerSelectButton *button = new LayerSelectButton(layer, heightProvider);
	layersList->addWidget(button);
	connect(button, &LayerSelectButton::activated, this, &SuperVideoWidget::selectLayerByButton);
	button->setChecked(true);
}

void SuperVideoWidget::selectLayerByButton(Layer *layer)
{
	timeline->setCurrentLayer(layer->name);
}

void SuperVideoWidget::updateLayerRemoved(Layer *layer)
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

void SuperVideoWidget::updateCurrentLayerChanged(Layer *newLayer)
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

void SuperVideoWidget::doSelectUpperLayer()
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

void SuperVideoWidget::doSelectLowerLayer()
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

