#include "layer_select_button.h"


LayerSelectButton::LayerSelectButton(Layer *layer, QWidget *parent)
	: QRadioButton(layer->name, parent)
	, layer(layer)
{
	connect(this, &LayerSelectButton::toggled, this, &LayerSelectButton::triggerChecked);
}

void LayerSelectButton::triggerChecked(bool checked)
{
	if (checked)
	{
		emit activated(layer);
	}
}
