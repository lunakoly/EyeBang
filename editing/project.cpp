#include "project.h"

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
	return layers.isEmpty();
}
