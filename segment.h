#ifndef SEGMENT_H
#define SEGMENT_H


/**
 * A continuous portion of a video.
 */
struct Segment
{
	/**
	 * The begin time in milliseconds.
	 */
	int begin;
	/**
	 * The end time in milliseconds.
	 */
	int end;
};


#endif // SEGMENT_H
