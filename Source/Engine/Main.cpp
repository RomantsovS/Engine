#include	<windows.h>
#include	<stdio.h>
#include	"Application.h"
											// main function of the app
int PASCAL WinMain ( HINSTANCE hCurInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow )
{
	Application	app ( "Engine", cmdLine );

	if ( app.isOk () )
		return app.run ();

	return 0;
}

