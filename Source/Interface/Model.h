//
// Generic model for 3D-scene.
// Provides methods for rendering, animation and 
// performing collision detection
//
// Author: Alex V. Boreskoff
//
// Last modified: 31/10/2002
//

#ifndef	__MODEL_3D__
#define	__MODEL_3D__

#include	"../Foundation/Object.h"
#include	"../3D/Vector3D.h"
										// class predeclarations
class	Array;
class	BoundingBox;
class	Camera;
class	View;
class	Controller;

class	Model : public Object
{
public:
	Model ( const char * theName ) : Object ( theName ) {}

										// methods for rendering and getting potential colliders
	virtual	void	render       ( View& view, const Camera& camera ) {}
	virtual	void	getColliders ( const BoundingBox& area, Array& colliders ) {}
	virtual	void	update       ( Controller *, float systemTime ) {}
};

#endif
