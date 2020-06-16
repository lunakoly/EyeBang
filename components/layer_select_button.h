#ifndef LAYERSELECTBUTTON_H
#define LAYERSELECTBUTTON_H

#include <QRadioButton>

#include "layer.h"


class LayerSelectButton : public QRadioButton
{
		Q_OBJECT

	public:
		Layer * const layer;

		LayerSelectButton(Layer *layer, QWidget *parent = nullptr);

	signals:
		void activated(Layer *layer);

	private:
		void triggerChecked(bool checked);
};


#endif // LAYERSELECTBUTTON_H
