#ifndef UFOATTACK_STORAGE_WIDGET_INCLUDED
#define UFOATTACK_STORAGE_WIDGET_INCLUDED

#include "../engine/uirendering.h"
#include "item.h"
#include "../engine/ufoutil.h"

class StorageWidget
{
public:
	StorageWidget(	const Texture* texture, const Texture* decoTexture, 
					const Screenport& port,
					const Storage* storage );

	~StorageWidget();

	const ItemDef* Tap( int x, int y );	
	void Draw();

	void SetOrigin( int x, int y );
	void SetButtonSize( int dx, int dy );
	void SetPadding( int dx, int dy );

	void CalcBounds( grinliz::Rectangle2I* _bounds );
	void Update()	{ valid = false; }

private:
	void SetButtons();

	bool valid;
	UIButtonBox* selectWidget;
	UIButtonBox* boxWidget;

	const Texture* buttonTexture;
	const Texture* decoTexture;

	int groupSelected;
	const Storage* storage;
	const ItemDef* itemDefMap[12];
};


#endif // UFOATTACK_STORAGE_WIDGET_INCLUDED