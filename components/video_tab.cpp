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
#include <QShortcut>
#include <QFile>
#include <QMessageBox>

#include "layer_select_button.h"


#define STEP_SIZE 100
#define JUMP_SIZE 1000


QLayout *VideoTab2::createDetails()
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

QScrollArea *VideoTab2::createLayersArea()
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

VideoTab2::VideoTab2(QWidget *parent) : QWidget(parent)
{
	mediaPlayer = new QMediaPlayer(this);
	mediaPlayer->setNotifyInterval(16);

	QVideoWidget *videoWidget = new QVideoWidget(this);
	videoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	mediaPlayer->setVideoOutput(videoWidget);

	timeline = new Timeline2(this);

	QVBoxLayout *main = new QVBoxLayout();
	main->addWidget(videoWidget);
	main->addWidget(timeline);
	main->addLayout(createDetails());

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addLayout(main);
	layout->addWidget(createLayersArea());

	connect(timeline, &Timeline2::sliderPressed,  this, &VideoTab2::timelinePressed);
	connect(timeline, &Timeline2::sliderReleased, this, &VideoTab2::timelineReleased);

	connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoTab2::mediaPlayerDurationChanged);

	connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoTab2::mediaPlayerPositionChanged);
	connect(timeline,    &Timeline2::valueChanged,  this, &VideoTab2::timelineValueChanged);

	connect(timeline, &Timeline2::currentLayerChanged, this, &VideoTab2::currentLayerChanged);

	// will be enable when the project
	// loads
	setEnabled(false);
}

void VideoTab2::loadProject(Project *project)
{
	if (project == nullptr)
	{
		return;
	}

	QFile file(project->getVideoFile());

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Can't open file: ") + file.errorString());
		return;
	}

	connect(project, &Project::layerAdded,   this, &VideoTab2::layerAdded);
	connect(project, &Project::layerRemoved, this, &VideoTab2::layerRemoved);

	connect(project, &Project::layerAdded,   timeline, &Timeline2::layerAdded);
	connect(project, &Project::layerRemoved, timeline, &Timeline2::layerRemoved);

	this->project = project;
	setEnabled(true);

	mediaPlayer->setMedia(QUrl::fromLocalFile(project->getVideoFile()));
	// now durationChanged is called
}

void VideoTab2::mediaPlayerDurationChanged(qint64 duration)
{
	timeline->setMaximum(duration);
	totalTimeLabel->setText(tr("Total: ") + timeToString(duration));

	// start rendering the current frame instead
	// of just the gray box. 100 - just some random values
	// TODO: find better solution
	resize(100, 100);
}

Timeline2 *VideoTab2::getTimeline()
{
	return timeline;
}

void VideoTab2::togglePlayback()
{
	if (isEnabled())
	{
		if (mediaPlayer->state() == QMediaPlayer::PlayingState)
		{
			mediaPlayer->pause();
		}
		else
		{
			mediaPlayer->play();
		}
	}
}

void VideoTab2::stepLeft()
{
	if (!isSliderPressed && isEnabled())
	{
		qint64 position = mediaPlayer->position() - STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab2::stepRight()
{
	if (!isSliderPressed && isEnabled())
	{
		qint64 position = mediaPlayer->position() + STEP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab2::jumpLeft()
{
	if (!isSliderPressed && isEnabled())
	{
		qint64 position = mediaPlayer->position() - JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab2::jumpRight()
{
	if (!isSliderPressed && isEnabled())
	{
		qint64 position = mediaPlayer->position() + JUMP_SIZE;
		mediaPlayer->setPosition(position);
	}
}

void VideoTab2::selectUpperLayer()
{
	if (isEnabled())
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
}

void VideoTab2::selectLowerLayer()
{
	if (isEnabled())
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
}

QString VideoTab2::timeToString(qint64 time)
{
	return QDateTime::fromMSecsSinceEpoch(time).toUTC().toString("h:mm:ss.zzz");
}

void VideoTab2::timelinePressed()
{
	wasPlayingBeforeSliderPress = mediaPlayer->state() == QMediaPlayer::PlayingState;

	if (wasPlayingBeforeSliderPress)
	{
		mediaPlayer->pause();
	}

	isSliderPressed = true;
}

void VideoTab2::timelineReleased()
{
	if (wasPlayingBeforeSliderPress)
	{
		mediaPlayer->play();
	}

	isSliderPressed = false;
}

void VideoTab2::mediaPlayerPositionChanged(qint64 position)
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

void VideoTab2::timelineValueChanged(qint64 position)
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

void VideoTab2::layerAdded(Layer *layer)
{
	LayerSelectButton *button = new LayerSelectButton(layer, heightProvider);
	layersList->addWidget(button);
	connect(button, &LayerSelectButton::activated, this, &VideoTab2::selectLayerByButton);
	button->setChecked(true);
	timeline->setCurrentLayer(layer);
}

void VideoTab2::selectLayerByButton(Layer *layer)
{
	timeline->setCurrentLayer(layer);
}

void VideoTab2::layerRemoved(Layer *layer)
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
			buttons[it]->deleteLater();
		}
	}

	if (timeline->getCurrentLayer() == layer)
	{
		Layer *newCurrent = nullptr;

		if (project != nullptr && project->hasLayers())
		{
			newCurrent = project->getLayers().first();
		}

		timeline->setCurrentLayer(newCurrent);
	}
}

void VideoTab2::currentLayerChanged(Layer *newLayer)
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

