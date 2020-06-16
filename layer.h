#ifndef LAYER_H
#define LAYER_H

#include <QObject>

#include "segment.h"


class Layer : public QObject
{
		Q_OBJECT

	public:
		const QString name;

		explicit Layer(const QString &name, QObject *parent = nullptr);

		const QVector<Segment> getSegments() const;

		void addSegment(const Segment &segment);
		void removeSegment(int index);
		void clearSegments();

		void setNewLeftBound(int position);
		void setNewRightBound(int position);

		bool canStart(int position);

	signals:
		void segmentAdded(const Segment &);
		void segmentRemoved(const Segment &);
		void segmentsModified();

	private:
		QVector<Segment> segments;
};

#endif // LAYER_H
