#include "layer.h"


Layer::Layer(const QString &name, QObject *parent)
	: QObject(parent)
	, name(name)
{

}

const QVector<Segment> Layer::getSegments() const
{
	return segments;
}

void Layer::addSegment(const Segment &segment)
{
	// detect if segments is contained
	// inside some other segment
	for (auto &it : segments)
	{
		if (it.begin <= segment.begin && segment.end <= it.end)
		{
			return;
		}
	}

	// detect segments that the new
	// one contains itself
	int it = 0;

	while (it < segments.size())
	{
		if (segment.begin < segments[it].begin && segments[it].end < segment.end)
		{
			segments.remove(it);
		}
		else
		{
			it += 1;
		}
	}

	// detect touching or one-side
	// overlapping segments
	int touchLeft = -1;
	int touchRight = -1;

	for (int it = 0; it < segments.size(); it++)
	{
		if (segments[it].begin <= segment.begin && segment.begin <= segments[it].end)
		{
			touchLeft = it;
		}
		else if (segments[it].begin <= segment.end && segment.end <= segments[it].end)
		{
			touchRight = it;
		}
	}

	if (touchLeft != -1 && touchRight != -1)
	{
		segments[touchLeft].end = segments[touchRight].end;
		segments.remove(touchRight);
		emit segmentAdded(segments[touchLeft]);
	}
	else if (touchLeft != -1)
	{
		segments[touchLeft].end = segment.end;
		emit segmentAdded(segments[touchLeft]);
	}
	else if (touchRight != -1)
	{
		segments[touchRight].begin = segment.begin;
		emit segmentAdded(segments[touchRight]);
	}
	else
	{
		segments.append(segment);
		emit segmentAdded(segment);
	}
}

void Layer::removeSegment(int index)
{
	auto &it = segments[index];
	segments.remove(index);
	emit segmentRemoved(it);
}

void Layer::clearSegments()
{
	while (!segments.isEmpty())
	{
		auto &it = segments[0];
		segments.removeFirst();
		emit segmentRemoved(it);
	}
}

void Layer::setNewLeftBound(int position)
{
	if (!segments.isEmpty())
	{
		int closest = -1;

		for (int it = 0; it < segments.size(); it++)
		{
			if (position < segments[it].end)
			{
				if (closest == -1)
				{
					closest = it;
				}
				else if (segments[it].end < segments[closest].end)
				{
					closest = it;
				}
			}
		}

		if (closest != -1)
		{
			if (position < segments[closest].begin)
			{
				addSegment({position, segments[closest].end});
			}
			else
			{
				segments[closest].begin = position;
				emit segmentsModified();
			}
		}
	}
}

void Layer::setNewRightBound(int position)
{	
	if (!segments.isEmpty())
	{
		int closest = -1;

		for (int it = 0; it < segments.size(); it++)
		{
			if (segments[it].begin < position)
			{
				if (closest == -1)
				{
					closest = it;
				}
				else if (segments[it].begin > segments[closest].begin)
				{
					closest = it;
				}
			}
		}

		if (closest != -1)
		{
			if (segments[closest].end < position)
			{
				addSegment({segments[closest].begin, position});
			}
			else
			{
				segments[closest].end = position;
				emit segmentsModified();
			}
		}
	}
}

bool Layer::canStart(int position)
{
	for (auto &it : segments)
	{
		if (it.begin < position && position < it.end)
		{
			return false;
		}
	}

	return true;
}
