//
// Autorelease pool class, based on array class
//
// Author: Alex V. Boreskoff
//
// Last modified: 26/11/2002
//

#include	"Array.h"
#include	"AutoreleasePool.h"

AutoreleasePool :: AutoreleasePool ( const char * theName ) : Object ( theName )
{
	pool      = new Array;
}

AutoreleasePool :: ~AutoreleasePool ()
{
	if ( pool != NULL )
		pool -> release ();
}

bool	AutoreleasePool :: isOk () const
{
	return pool != NULL && pool -> isOk ();
}


void	AutoreleasePool :: add ( Object * obj )
{
	if ( pool != NULL )
		pool -> insertNoRetain ( obj );
}

void	AutoreleasePool :: releaseAll ()
{
	if ( pool != NULL )
		pool -> removeAll ();
}

AutoreleasePool * autoreleasePool = new AutoreleasePool ( "Autorelease Pool" );
