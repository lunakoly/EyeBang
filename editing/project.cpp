#include "project.h"

#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>


Project::Project(QObject *parent) : QObject(parent)
{

}

QString Project::getVideoFile()
{
	return videoFile;
}

void Project::setVideoFile(const QString &filename)
{
	videoFile = filename;
	emit videoFileChanged(filename);
}

Layer *Project::addLayer(const QString &name)
{
	if (!layers.contains(name))
	{
		auto it = new Layer(name, this);
		layers[name] = it;
		emit layerAdded(it);
		return it;
	}

	return nullptr;
}

Layer *Project::removeLayer(const QString &name)
{
	if (layers.contains(name))
	{
		auto it = layers[name];
		layers.remove(name);
		emit layerRemoved(it);
		return it;
	}

	return nullptr;
}

Layer *Project::getLayer(const QString &name)
{
	if (layers.contains(name))
	{
		return layers[name];
	}

	return nullptr;
}

QList<Layer *> Project::getLayers()
{
	return layers.values();
}

bool Project::hasLayers()
{
	return !layers.isEmpty();
}

void Project::rename(const QString &currentName, const QString &newName)
{
	if (layers.contains(currentName))
	{
		if (!layers.contains(newName))
		{
			auto layer = layers[currentName];
			layers.remove(currentName);
			layers[newName] = layer;
			layer->setName(newName);
			emit layerRenamed(currentName, newName);
		}
		else
		{
			QMessageBox::warning(nullptr, tr("Note"), tr("There's already a layer with such a name"));
		}
	}
}

bool Project::containsLayer(const QString &name)
{
	return layers.contains(name);
}

QString timeToString(int time)
{
	return QDateTime::fromMSecsSinceEpoch(time).toUTC().toString("h:mm:ss.zzz");
}

void Project::exportRangesFile(QFile &file)
{
	QTextStream output(&file);

	for (auto &layer : layers)
	{
		for (auto &segment : layer->getSegments())
		{
			output << timeToString(segment.begin) + '-' + timeToString(segment.end) + ' ' + layer->getName() + '\n';
		}

		output << '\n';
	}
}
