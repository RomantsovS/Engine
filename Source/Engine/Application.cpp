//
// Application class of Arwen project
//
// Author: Alex V. Boreskoff
//

#include	<stdio.h>
#include	<stdarg.h>
#include	<locale.h>

#include	"Application.h"
#include	"../Foundation/AutoreleasePool.h"
#include	"../3D/View.h"
#include	"../3D/OpenGlView.h"
#include	"../Interface/Controller.h"
#include	"GameController.h"
#include	"../Interface/Model.h"
#include	"World.h"
#include	"../3D/Camera.h"
#include    "../Lua/MyLua.h"
/*#include	"Log.h"
#include	"ResourceManager.h"
#include	"FileSystem.h"

#include	"SceneDecoder.h"
									// here are image decoders
#include	"BmpDecoder.h"
#include	"PcxDecoder.h"
#include	"TgaDecoder.h"
#include	"WalDecoder.h"
#include	"RgbDecoder.h"
#include	"PngDecoder.h"
#include	"GifDecoder.h"
#include	"JpegDecoder.h"
									// here are models decoder
#include	"AseDecoder.h"
#include	"Md2Decoder.h"
#include	"Md3Decoder.h"*/

//Log         * sysLog = new Log ( "Arwen.log" );
Application * Application :: instance = NULL;

Application :: Application ( const char * theName, const char * argsStr ) : Object ( theName )
{
	hInstance       = :: GetModuleHandle ( NULL );
	instance        = this;
	model           = NULL;
	view            = NULL;
	controller      = NULL;
	lua = NULL;
//	resourceManager = NULL;

	setlocale ( LC_CTYPE, "" );			// set current locale as default ANSI 
										// code page obtained from operating system

										// create and prepare resource manager
/*	resourceManager = new ResourceManager ( "ResourceManager" );

	resourceManager -> registerSource  ( new OsFileSystem () );
	resourceManager -> registerDecoder ( new SceneDecoder ( resourceManager ) );

										// register image decoders
	resourceManager -> registerDecoder ( new TgaDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new WalDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new PcxDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new BmpDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new RgbDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new PngDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new GifDecoder  ( resourceManager ) );
	resourceManager -> registerDecoder ( new JpegDecoder ( resourceManager ) );

										// register models decoders
	resourceManager -> registerDecoder ( new AseDecoder ( resourceManager ) );
	resourceManager -> registerDecoder ( new Md2Decoder ( resourceManager ) );
	resourceManager -> registerDecoder ( new Md3Decoder ( resourceManager ) );*/

										// parse args
										// possible args:
										// -lightmaps
										// map name
	Array * args  = String ( argsStr ).split ();
	String	mapName;

	if ( args != NULL )
	{
		for ( Array :: Iterator it = args -> getIterator (); !it.end (); ++it )
		{
			String	* str = (String *) it.value ();

			/*if ( *str == "-lightmaps" )	// request to build lightmaps
				buildLightmaps = true;
			else
				mapName = *str;*/
		}
	}

	int		 width = 640;
	int		 height = 480;
	int		 bpp = 32;
	int	 fullScreen = 0;
	float	 fov = 90;
	String	 map;
	float	 fwVel = 3;
	float	 bkVel = 2;
	float	 stVel = 2;
	Vector3D size;
	String	 detailTexture;
/*	float	 detailDist;
	float	 detailScale;*/

	lua = new Lua("lua");
	if(lua->isOk())
	{
		if(lua->doFile("Data/config.lua"))
		{
			width = lua->getGlobalParametr("width");
			height = lua->getGlobalParametr("height");
			fullScreen = lua->getGlobalParametr("fullscreen");
		}
	}

	Camera * camera;

	camera    = new Camera     ( "camera", Vector3D ( 0, 0, -10 ), 0, 0, 0, fov, EPS, 200 );
    view      = new OpenGlView ( "Main view", hInstance, camera, width, height, bpp, fullScreen );

	if ( !view -> isOk () )
	{
		okBox ( "Error", "Cannot create main view" );

		camera -> release ();
		view   -> release ();

		return;
	}

	controller = new GameController ( "controller", NULL, view, camera, size );

	controller -> setCamera        ( camera );					// setup camera
	
	GameController *con = dynamic_cast <GameController *>(controller);
	con -> setForwardSpeed  ( fwVel );
	con -> setBackwardSpeed ( bkVel );
	con -> setStrafeSpeed   ( stVel );

//	model = dynamic_cast <Model *> ( resourceManager -> getObject ( map ) );
	model = new World("world");

	if ( model == NULL || !model -> isOk () )
	{
		//if ( model == NULL )
//			(*sysLog) << "Model NULL" << logEndl;
		//else
		//	(*sysLog) << "Model invalid" << logEndl;

		okBox ( "Error", "Cannot load scene %s", map.c_str () );

//		controller -> release ();

		return;
	}

	controller -> setModel ( model );

	/*World :: detailDistance     = detailDist;
	World :: detailTextureScale = detailScale;*/
//	World :: detailTexture      = resourceManager -> getTexture ( detailTexture );

	//(*sysLog) << "Application successfully initialized" << logEndl;
}

Application ::~Application ()
{
	if ( controller != NULL )
		controller -> release ();
	if(lua != NULL)
		lua->release();

	/*if ( resourceManager != NULL )
		resourceManager -> release ();*/
}

bool	Application :: isOk () const
{
	if ( controller == NULL || model == NULL || view == NULL || lua == NULL )
		return false;

	return controller -> isOk () && model -> isOk () && view -> isOk () && lua->isOk();
}

int	Application :: run ()
{
	if ( !isOk () )
		return 0;

    MSG		msg;
	int bRet;
    
    for ( ; ; )
    {
		autoreleasePool -> releaseAll ();

        if ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != 0)
        {
            bRet = GetMessage(&msg, NULL, 0, 0);
			if(bRet == 0)
			{
				return msg.wParam;
			}
			else if(bRet == -1)
			{
			}
			else
			{
				TranslateMessage ( &msg );
				DispatchMessage  ( &msg );
			}
        }
        else
        if ( view -> isVisible () )
		{
			view -> lock ();

			if ( controller -> update () == controllerQuit )
				return 0;

			view -> unlock ();
		}
    }
}

void	Application :: okBox ( const char * caption, const char * format, ... ) const
{
	va_list	argptr;
	char	buf [1024];

	va_start ( argptr, format );
	vsprintf ( buf, format, argptr );
	va_end   ( argptr );

	MessageBox ( NULL, buf, caption, MB_ICONEXCLAMATION | MB_OK );
}

