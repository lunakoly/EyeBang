#ifndef LAYERSELECTBUTTON_H
#define LAYERSELECTBUTTON_H

#include <QRadioButton>

#include "layer.h"


/**
 * The radio button that can know
 * which layer it's used to activate.
 */
class LayerSelectButton : public QRadioButton
{
		Q_OBJECT

	public:
		/**
		 * The layer the button is responsible for
		 */
		Layer * const layer;

		LayerSelectButton(Layer *layer, QWidget *parent = nullptr);

	signals:
		/**
		 * Called whenever the button is switched
		 * to `checked`. The button passes it's layer
		 * as an argument.
		 */
		void activated(Layer *layer);

	private:
		// override and watch for the state
		void triggerChecked(bool checked);
};


#endif // LAYERSELECTBUTTON_H
