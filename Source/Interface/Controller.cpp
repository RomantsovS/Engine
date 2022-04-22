//
// Basic class to handle keys ( WM_CHAR messages ), key pressing/releasing
// and mouse/joystick movements
//
// Author: Alex V. Boreskoff
//
// Last modified: 28/11/2002
//

#include	<windows.h>					// for VK_ values
#include	<memory.h>

#include	"Controller.h"
#include	"../Foundation/Timer.h"
#include	"Model.h"
#include	"../3D/View.h"
//#include	"Font.h"
#include	"../3D/Camera.h"
//#include	"ResourceManager.h"
//#include	"FileSystem.h"
//#include	"Console.h"
#include	"Input.h"
//#include	"MutableData.h"
//#include	"TgaEncoder.h"
//#include	"Texture.h"
//#include	"FileUtils.h"
#include	"Events.h"
#include	"../Engine/Application.h"


Controller :: Controller ( const char * theName, Model * theModel, View * theView, Camera * theCamera )
                   : Object ( theName )
{

	model     = theModel;
	view      = theView;
	camera    = theCamera;
	timer     = :: getTimer ();
//	font      = Application :: instance -> getResourceManager () -> getSystemFont ();
	//console   = new Console ( "..\\Textures\\water.jpg", this );
	numFrames = 0;
	fps       = 0;
	yaw       = 0;
	pitch     = 0;
	roll      = 0;
	pos       = Vector3D ( 5, 5, -10 );

								// set all keys to default values

	resourcesUploaded = false;

	/*console -> addString ( getView () -> getVendor   () );
	console -> addString ( getView () -> getRenderer () );
	console -> addString ( getView () -> getVersion  () );
	console -> addString ( String ().printf ( "Current mode %dx%d color depth %d bits", getView () -> getWidth (), getView () -> getHeight (), getView () -> getBitsPerPixel () ) );
*/
	for ( int i = 0; i < 8; i++ )
		lastFrameTimes [i] = timer -> getTime ();

	view -> setController ( this );
}

Controller :: ~Controller ()
{
//	delete font;

	if ( timer != NULL )
		timer -> release ();

	if ( model != NULL )
		model -> release ();

	if ( view != NULL )
		view -> release ();

	if ( camera != NULL )
		camera -> release ();

	//if ( console != NULL )
//		console -> release ();
}

bool	Controller :: isOk () const
{
	return model   != NULL && model   -> isOk () && view   != NULL && view -> isOk () && 
		   /*console != NULL && console -> isOk () &&*/ camera != NULL && camera -> isOk ();
}

void	Controller :: setView ( View * theView )
{
	if ( view != NULL )
		view -> release ();

	view = theView;
}

void	Controller :: setModel ( Model * theModel )
{
	if ( model != NULL )
		model -> release ();

	model = theModel;

	if ( camera != NULL )
	{
		camera -> setEulerAngles ( yaw, pitch, roll );
		camera -> setPos         ( pos );
	}
}

void	Controller :: setCamera ( Camera * theCamera )
{
	if ( camera != NULL )
		camera -> release ();

	camera = theCamera;

	camera -> retain         ();
	camera -> setEulerAngles ( yaw, pitch, roll );
	camera -> setPos         ( pos );
}

int	Controller :: handleChar ( int ch )
{
//	console -> handleChar ( ch );

	return controllerContinue;
}

int	Controller :: handleKey ( int key, bool pressed )
{
	if ( !pressed )									// we're interested only in presses
		return controllerContinue;

	if ( key == keyEsc || key == keyF10 )			// check for quit command
		return controllerQuit;

	if ( key == keyF1 || key == keyPrintScreen )	// print screen request
	{
		int			 size = view -> getWidth () * view -> getHeight () * 3;	// size of image in bytes
//		MutableData	 data ( "screenshot", size + 1024 );
		//TgaEncoder	 encoder;
		//Texture    * image = view -> getScreenShot ();

		/*if ( image != NULL )
			if ( encoder.encode ( image, &data ) )
			{
				for ( int no = 1; fileExist ( String :: printf ( "screenshot%02d.tga", no ) ); no++ )
					;

				data.saveToFile ( String :: printf ( "screenshot%02d.tga", no ) );
			}

		image -> release ();*/
	}

//	console -> handleKey ( key, pressed );

	return controllerContinue;
}

void	Controller :: draw ()
{
	if ( !resourcesUploaded )
	{
//		Application :: instance -> getResourceManager () -> upload ();			// upload all loaded textures
	
		resourcesUploaded = true;
	}

	if ( getModel () != NULL && camera != NULL )
		getModel () -> render ( *getView (), *camera );

//	console -> draw ( getView () );

	float	curTime = timer -> getTime ();

	model -> update ( this, curTime );

	numFrames++;

	if ( numFrames >= 8 )
	{
		fps = 8.0f / (curTime - lastFrameTimes [0]);

		memcpy ( &lastFrameTimes [0], &lastFrameTimes [1], 7 * sizeof ( float ) );

		lastFrameTimes [7] = curTime;
	}
	else
	{
		fps = (float) numFrames / (curTime - lastFrameTimes [numFrames - 1] );

		lastFrameTimes [numFrames] = curTime;
	}

//	font -> print ( view, 10, 30, "fps %4.1f", fps );
}

int	Controller :: update ()
{
	if ( getView () == NULL )
		return 0;

	KeyboardState	keys;
	MouseState		mouse;
//	Event         * event;

	/*while ( ( event = getView () -> getEvent () ) != NULL )
	{
		if ( event -> handle ( this ) == controllerQuit )
		{
			event -> release ();

			return controllerQuit;
		}

		event -> release ();
	}*/

	if ( getView () -> getKeyboardState ( keys ) )
		if ( handleKeyboard ( keys ) == controllerQuit )
			return controllerQuit;

	if ( getView () -> getMouseState ( mouse ) )
		if ( handleMouse ( mouse ) == controllerQuit )
			return controllerQuit;

	model -> update ( this, timer -> getTime () );

	getView () -> lock ();

	draw ();

	getView () -> unlock ();

	return controllerContinue;
}

float	Controller :: timeSinceLastFrame () const
{
	float	curTime = timer -> getTime ();

	if ( numFrames >= 8 )
		return curTime - lastFrameTimes [0];

	return curTime - lastFrameTimes [numFrames - 1];
}
