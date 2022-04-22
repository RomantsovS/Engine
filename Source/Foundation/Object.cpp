//
// Root class for all object hierarchy
//
// Author: Alex V. Boreskoff
//
// Last modified: 9/12/2002
//

#include    <malloc.h>
#include    <string.h>
#include	"Object.h"
#include	"ObjStr.h"
#include	"AutoreleasePool.h"

///////////////////////////// Object methods ///////////////////////////

Object :: Object ()
{
	name      = strdup ( "" );
	refCount  = 1;
	flags     = 0;
	owner     = NULL;
	lockCount = 0;
}

Object :: Object ( const char * aName )
{
	name      = strdup ( aName );
	refCount  = 1;
	flags     = 0;
	owner     = NULL;
	lockCount = 0;
}

Object :: ~Object () 
{
	free ( name );
}

int	Object :: compare ( const Object * obj ) const
{
//	int	res = strcmp ( getClassName (), obj -> getClassName () );

//	if ( res != 0 )
	//	return res;

	return strcmp ( getName (), obj -> getName () );
}

int	Object :: release ()
{
	if ( --refCount < 1 )
	{
		delete this;

		return 0;
	}

	return 1;
}

Object * Object :: autorelease ()
{
	autoreleasePool -> add ( this );

	return this;
}

void    Object :: setName ( const char * newName )
{
    free ( name );

    name = strdup ( newName );
}

void    Object :: setOwner ( Object * newOwner )
{
    if ( owner != NULL )
		owner -> release ();

    owner = newOwner;

	if ( owner != NULL )
		owner -> retain ();
}
