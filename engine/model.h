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

#ifndef UFOATTACK_MODEL_INCLUDED
#define UFOATTACK_MODEL_INCLUDED

#include "../grinliz/gldebug.h"
#include "../grinliz/gltypes.h"
#include "../grinliz/glgeometry.h"
#include "vertex.h"
#include "enginelimits.h"
#include "serialize.h"

class Texture;
class SpaceTree;
class RenderQueue;

// The smallest draw unit: texture, vertex, index.
struct ModelAtom 
{
	const Texture* texture;
	U32 vertexID;
	U32 indexID;
	U32 nVertex;
	U32 nIndex;
	mutable int trisRendered;

	void Bind() const;
	void Draw() const;

	const U16* index;		// points back to ModelResource memory.
	const Vertex* vertex;	// points back to ModelResource memory.
};


class ModelResource
{
public:
	void Free();

	// In the coordinate space of the resource! (Object space.)
	int Intersect(	const grinliz::Vector3F& point,
					const grinliz::Vector3F& dir,
					grinliz::Vector3F* intersect );


	ModelHeader header;						// loaded

	grinliz::Sphere			boundSphere;	// computed
	float					boundRadius2D;	// computed, bounding 2D radius centered at 0,0
	grinliz::Rectangle3F	hitBounds;		// for picking - a bounds approximation
	U16*					allIndex;		// memory store for vertices and indices. Used for hit-testing.
	Vertex*					allVertex;

	int IsOriginUpperLeft()				{ return header.flags & ModelHeader::UPPER_LEFT; }
	const grinliz::Rectangle3F& AABB()	{ return header.bounds; }

	ModelAtom atom[EL_MAX_MODEL_GROUPS];
};


class ModelLoader
{
public:
	ModelLoader( const Texture* texture, int nTextures ) 	{
		this->texture = texture;
		this->nTextures = nTextures;
	}
	~ModelLoader()		{}

	void Load( FILE* fp, ModelResource* res );

private:
	const Texture* texture;
	int nTextures;
};


class Model
{
public:
	Model()		{	Init( 0, 0 ); }
	~Model()	{}

	void Init( ModelResource* resource, SpaceTree* tree );

	// Queued rendering
	enum {
		MODEL_TEXTURE,	// use the texture of the models
		NO_TEXTURE,		// no texture at all - shadow pass Z
	};
	void Queue( RenderQueue* queue, int textureState=MODEL_TEXTURE );

	// Used by the queued rendering system:
	void PushMatrix() const;
	void PopMatrix() const;

	enum {
		MODEL_SELECTABLE			= 0x01,
		MODEL_HIDDEN_FROM_TREE		= 0x02,
		MODEL_OWNED_BY_MAP			= 0x04,
		MODEL_NO_SHADOW				= 0x08,
	};

	int IsFlagSet( int f ) const	{ return flags & f; }
	void SetFlag( int f )			{ flags |= f; }
	void ClearFlag( int f )			{ flags &= (~f); }
	int Flags()	const			{ return flags; }

	const grinliz::Vector3F& Pos()						{ return pos; }
	void SetPos( const grinliz::Vector3F& pos );
	void SetPos( float x, float y, float z )	{ grinliz::Vector3F vec = { x, y, z }; SetPos( vec ); }
	float X() { return pos.x; }
	float Y() { return pos.y; }
	float Z() { return pos.z; }

	void SetYRotation( float rot )		{
		while( rot < 0 )		{ rot += 360.0f; }
		while( rot >= 360 )		{ rot -= 360.0f; }
		this->rot = rot;		// won't change tree location, don't need to call Update()
		Modify();
	}
	const float GetYRotation()			{ return rot; }

	int IsBillboard()		{ return resource->header.flags & ModelHeader::BILLBOARD; }
	int IsOriginUpperLeft()	{ return resource->header.flags & ModelHeader::UPPER_LEFT; }
	int IsShadowRotated()	{ return resource->header.flags & ModelHeader::ROTATE_SHADOWS; }
	
	// Set the skin texture (which is a special texture xform)
	void SetSkin( int armor, int skin, int hair );
	// Set the texture xform for rendering tricks
	void SetTexXForm( float a=1.0f, float d=1.0f, float x=0.0f, float y=0.0f );

	// Set the texture - overrides all textures
	void SetTexture( const Texture* t )			{ setTexture = t; }

	void CalcBoundSphere( grinliz::Sphere* sphere );
	void CalcBoundCircle( grinliz::Circle* circle );
	void CalcHitAABB( grinliz::Rectangle3F* aabb );

	// Calcs the AABB if the rotation is Nx90. Returs true for success.
	bool CalcAABB( grinliz::Rectangle3F* aabb ) ;

	// Returns grinliz::INTERSECT or grinliz::REJECT
	int IntersectRay(	const grinliz::Vector3F& origin, 
						const grinliz::Vector3F& dir,
						grinliz::Vector3F* intersect );

	ModelResource* GetResource()				{ return resource; }
	bool Sentinel()								{ return resource==0 && tree==0; }

	Model* next;			// used by the SpaceTree query
	Model* next0;			// used by the Engine sub-sorting

private:
	struct TexMat {
		float a, d, x, y;
		void Identity() { a=1.0f; d=1.0f; x=0.0f; y=0.0f; }
	};

	void Modify() { xformValid = false; invValid = false; }
	const grinliz::Matrix4& XForm() const;
	const grinliz::Matrix4& InvXForm() const;

	SpaceTree* tree;
	ModelResource* resource;
	grinliz::Vector3F pos;
	float rot;
	bool texMatSet;
	TexMat texMat;
	const Texture* setTexture;	// overrides the default texture
	int flags;
	
	mutable bool xformValid;
	mutable bool invValid;

	mutable grinliz::Matrix4 _xform;
	mutable grinliz::Matrix4 _invXForm;
};


#endif // UFOATTACK_MODEL_INCLUDED