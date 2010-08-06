/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UIRENDERING_INCLUDED
#define UIRENDERING_INCLUDED

#include "../gamui/gamui.h"
#include "gpustatemanager.h"


enum {
	ICON_GREEN_BUTTON		= 0,
	ICON_BLUE_BUTTON		= 1,
	ICON_RED_BUTTON			= 2,
	ICON_GREEN_BUTTON_DOWN	= 8,
	ICON_BLUE_BUTTON_DOWN	= 9,
	ICON_RED_BUTTON_DOWN	= 10,

	ICON_AWARD_PURPLE_CIRCLE = 4,
	ICON_AWARD_ALIEN_1		= 5,
	ICON_AWARD_ALIEN_5		= 6,
	ICON_AWARD_ALIEN_10		= 7,

	ICON_GREEN_STAND_MARK	= 16,
	ICON_YELLOW_STAND_MARK	= 17,
	ICON_ORANGE_STAND_MARK	= 18,

	ICON_GREEN_STAND_MARK_OUTLINE	= 11,
	ICON_YELLOW_STAND_MARK_OUTLINE	= 12,
	ICON_ORANGE_STAND_MARK_OUTLINE	= 13,

	ICON_STAND_HIGHLIGHT	= 21,

	ICON_TARGET_STAND		= 19,
	ICON_TARGET_POINTER		= 20,

	ICON_GREEN_WALK_MARK	= 24,
	ICON_YELLOW_WALK_MARK	= 25,
	ICON_ORANGE_WALK_MARK	= 26,

	ICON_RANK_0				= 27,
	ICON_RANK_1				= 28,
	ICON_RANK_2				= 29,
	ICON_RANK_3				= 30,
	ICON_RANK_4				= 31,

	DECO_CHARACTER		= 0,
	DECO_PISTOL			= 1,
	DECO_RIFLE			= 2,
	DECO_SHELLS			= 3,
	DECO_ROCKET			= 4,
	DECO_CELL			= 5,
	DECO_RAYGUN			= 6,

	DECO_AIMED			= 8,
	DECO_AUTO			= 9,
	DECO_SNAP			= 10,
	DECO_MEDKIT			= 11,
	DECO_KNIFE			= 12,
	DECO_ALIEN			= 13,
	DECO_ARMOR			= 14,
	DECO_BOOM			= 15,

	DECO_METAL			= 16,
	DECO_TECH			= 17,
	DECO_FUEL			= 18,
	DECO_SWAP			= 19,
	DECO_HELP			= 20,
	DECO_LAUNCH			= 21,
	DECO_END_TURN		= 22,

	DECO_PREV			= 24,
	DECO_NEXT			= 25,
	DECO_ROTATE_CCW		= 26,
	DECO_ROTATE_CW		= 27,
	DECO_SHIELD			= 28,

	DECO_NONE			= 31,
};


class UIRenderer : public gamui::IGamuiRenderer, public gamui::IGamuiText
{
public:
	enum {
		RENDERSTATE_UI_NORMAL = 1,		// normal UI rendering (1.0 alpha)
		RENDERSTATE_UI_NORMAL_OPAQUE,	// same, but for resources that don't blend (background images)
		RENDERSTATE_UI_DISABLED,		// disabled of both above
		RENDERSTATE_UI_TEXT,			// text rendering
		RENDERSTATE_UI_TEXT_DISABLED,
		RENDERSTATE_UI_DECO,			// deco rendering
		RENDERSTATE_UI_DECO_DISABLED,
	};

	UIRenderer() : shader( true ), textRed( 1 ), textGreen( 1 ), textBlue( 1 ) {}

	void SetTextColor( float r, float g, float b )		{ textRed = r; textGreen = g; textBlue = b; }

	virtual void BeginRender();
	virtual void EndRender();

	virtual void BeginRenderState( const void* renderState );
	virtual void BeginTexture( const void* textureHandle );
	virtual void Render( const void* renderState, const void* textureHandle, int nIndex, const int16_t* index, int nVertex, const gamui::Gamui::Vertex* vertex ) ;

	static void SetAtomCoordFromPixel( int x0, int y0, int x1, int y1, int w, int h, gamui::RenderAtom* );

	static gamui::RenderAtom CalcDecoAtom( int id, bool enabled=true );
	static gamui::RenderAtom CalcParticleAtom( int id, bool enabled=true );
	static gamui::RenderAtom CalcIconAtom( int id, bool enabled=true );

	enum {
		PALETTE_GREEN, PALETTE_BLUE, PALETTE_RED, PALETTE_YELLOW, PALETTE_GREY,
		PALETTE_NORM=0, PALETTE_BRIGHT, PALETTE_DARK
	};
	static gamui::RenderAtom CalcPaletteAtom( int c0, int c1, int blend, float w, float h, bool enable=true );

	virtual void GamuiGlyph( int c, gamui::IGamuiText::GlyphMetrics* metric );
	
private:
	CompositingShader shader;
	float textRed, textGreen, textBlue;
};



#endif // UIRENDERING_INCLUDED