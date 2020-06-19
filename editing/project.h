#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QHash>

#include "layer.h"


/**
 * Keeps information about the associated
 * video file, existing layers and so on.
 */
class Project : public QObject
{
		Q_OBJECT

	public:
		explicit Project(QObject *parent = nullptr);

		/**
		 * Returns the project video
		 * file or an empty string
		 * if no one set.
		 */
		QString getVideoFile();
		/**
		 * Sets the project video file.
		 */
		void setVideoFile(const QString &filename);

		/**
		 * Attepmts to create a new layer
		 * with the specified name if none
		 * exists and returns it. If there's
		 * a layer with that name, returns nullptr.
		 */
		Layer *addLayer(const QString &name);
		/**
		 * Attepmts to remove a layer with the
		 * specified name and return a pointer to it.
		 * Does nothing if no such a layer found
		 * and nullptr is return instead.
		 */
		Layer *removeLayer(const QString &name);

		/**
		 * Returns the layer with the specified
		 * name or nullptr.
		 */
		Layer *getLayer(const QString &name);
		/**
		 * Returns the list of all layers.
		 */
		QList<Layer *> getLayers();

		/**
		 * Returns true if there're more than
		 * zero layers available now.
		 */
		bool hasLayers();

		/**
		 * Rename the layer with the `currentName`
		 * to `newName`. Does nothing if no such
		 * a layer found.
		 * Note that this function
		 * must be used instead of Layer::setName()
		 * for layers controlled by this project.
		 */
		void rename(const QString &currentName, const QString &newName);

		/**
		 * Returns true if the specified layer
		 * has been created.
		 */
		bool containsLayer(const QString &name);

	signals:
		/**
		 * Emitted when the project video
		 * file changes. Passes the new file path
		 * as an argument.
		 */
		void videoFileChanged(const QString &filename);

		/**
		 * Called when a new layer is created.
		 */
		void layerAdded(Layer *layer);
		/**
		 * Called a new layer is removed.
		 */
		void layerRemoved(Layer *layer);

		/**
		 * Emitted whenever the name of a layer
		 * controlled by this project changes.
		 */
		void layerRenamed(const QString &oldName, const QString &newName);

	private:
		QString videoFile;
		QHash<QString, Layer*> layers;
};


#endif // PROJECT_H
