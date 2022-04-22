//
// Basic events for windows messages
//
// Author: Alex V. Boreskoff
//
// Last modified: 28/11/2002
//

#include	"Events.h"
#include	"Controller.h"

int	KeyEvent :: handle ( Controller * controller )
{
	if ( controller != NULL )
		return controller -> handleKey ( keyCode, pressed );

	return controllerContinue;
}

int	CharEvent :: handle ( Controller * controller )
{
	if ( controller != NULL )
		return controller -> handleChar ( ch );

	return controllerContinue;
}

