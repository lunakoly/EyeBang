#ifndef ALIASES_H
#define ALIASES_H

class QObject;
class QString;


/**
 * A type of a callback needed for accepting the
 * text answer. The one may need to cast their
 * funtion pointer to this type explicitly.
 */
using TextCallback = void (QObject::*)(const QString &);

class Layer;

/**
 * A type of a callback needed for accepting the
 * text answer. The one may need to cast their
 * funtion pointer to this type explicitly.
 */
using LayerCallback = void (QObject::*)(Layer *);


#endif // ALIASES_H
