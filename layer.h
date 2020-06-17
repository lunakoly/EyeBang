#ifndef LAYER_H
#define LAYER_H

#include <QObject>

#include "segment.h"


/**
 * A named set of segments.
 */
class Layer : public QObject
{
		Q_OBJECT

	public:
		/**
		 * The unique name of the layer
		 * (it used to be called a `marker` in the past).
		 */
		const QString name;

		explicit Layer(const QString &name, QObject *parent = nullptr);

		/**
		 * Returns a read-only vector of the
		 * stored segments.
		 */
		const QVector<Segment> getSegments() const;

		/**
		 * Accepts a request of adding a segment
		 * and ensures it doesn't overlay any other
		 * segments (performs the nessesary fixes).
		 */
		void addSegment(const Segment &segment);
		/**
		 * Removes the segment with the given index.
		 */
		void removeSegment(int index);
		/**
		 * Removes all segments.
		 */
		void clearSegments();

		/**
		 * Updates the left bound of the closest
		 * to the right of the positiion segment.
		 */
		void setNewLeftBound(int position);
		/**
		 * Updates the right bound of the closest
		 * to the left of the positiion segment.
		 */
		void setNewRightBound(int position);

	signals:
		/**
		 * Emitted when a new segment is added
		 * (the segment passed as the parameter may
		 * be not the same one that was requested to
		 * be added).
		 */
		void segmentAdded(const Segment &);
		/**
		 * Emitted when a segment is removed.
		 */
		void segmentRemoved(Segment);
		/**
		 * Emitted whenever any change to segments
		 * happens.
		 */
		void segmentsModified();

	private:
		QVector<Segment> segments;
};


#endif // LAYER_H
