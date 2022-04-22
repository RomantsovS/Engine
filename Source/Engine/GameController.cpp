#include    "GameController.h"
#include	"../Interface/Model.h"
//#include	"ColDet.h"
#include	"../3D/Camera.h"
//#include	"BoundingBox.h"
#include	"../Foundation/Array.h"
//#include	"Console.h"
#include	"../Interface/Input.h"
#include	"../Interface/DirectXInputReader.h"
//#include	"Timer.h"

GameController :: GameController ( const char * theName, Model * theModel, View * theView, Camera * theCamera, 
								  const Vector3D& defSize) : Controller ( theName, theModel, theView, theCamera ),
								  size ( defSize )
{
	mouseSensitivity = 9.0f;

	forwardKey      = keyW;

    backwardKey     = keyS;

    leftKey         = keyLeft;
	leftKey2        = 0;

    rightKey        = keyRight;
	rightKey2       = 0;

	strafeLeftKey   = keyA;
	strafeLeftKey2  = 0;

	strafeRightKey  = keyD;
	strafeRightKey2 = 0;

	upKey = keyUp;
	downKey = keyDown;

	escapeKey = keyEsc;

	forwardSpeed     = 3;
	backwardSpeed    = 2;
	strafeSpeed      = 2;

	gravity   = Vector3D ( 0, -0.07f, 0 );				// set very low gravity for good animations
	velocity  = Vector3D ( 0, 0, 0 );
}

GameController :: ~GameController ()
{

}

int	GameController :: handleMouse ( const MouseState& mouse )
{

	//if ( console -> getState () == consoleInactive )
	{
		yaw   += mouse.getMouseX() * mouseSensitivity / -1000.0f;
		pitch += mouse.getMouseY() * mouseSensitivity / -1000.0f;

		camera -> setEulerAngles ( yaw, pitch, roll );
	}

	return controllerContinue;
}

int	GameController :: handleKeyboard ( const KeyboardState& keys )
{
/*	if ( console -> getState () != consoleInactive )
		return controllerContinue;*/

	Vector3D	vel ( 0.0f, 0.0f, 0.0f );

	if(keys.isKeyPressed(escapeKey))
		return controllerQuit;

	if ( keys.isKeyPressed ( forwardKey ) || keys.isKeyPressed ( forwardKey2 ) )
		vel += forwardSpeed * camera -> getViewDir ();
    if ( keys.isKeyPressed ( backwardKey ) || keys.isKeyPressed ( backwardKey2 ) )
		vel += -backwardSpeed * camera -> getViewDir ();

	if ( keys.isKeyPressed ( leftKey ) || keys.isKeyPressed ( leftKey2 ) )
		yaw += 0.05f;

	if ( keys.isKeyPressed ( rightKey ) || keys.isKeyPressed ( rightKey2 ) )
		yaw -= 0.05f;

	if ( keys.isKeyPressed ( upKey ) )
		pitch += 0.05f;

	if ( keys.isKeyPressed ( downKey ))
		pitch -= 0.05f;

	if ( keys.isKeyPressed ( strafeLeftKey ) || keys.isKeyPressed ( strafeLeftKey2 ) )
		vel += -strafeSpeed * camera -> getRightDir ();

	if ( keys.isKeyPressed ( strafeRightKey ) || keys.isKeyPressed ( strafeRightKey2 ) )
		vel += strafeSpeed * camera -> getRightDir ();

													// compute step distance based on velocity
//	float	curTime  = timer -> getTime ();
//	float	lastTime = lastFrameTimes [numFrames >= 8 ? 7 : numFrames - 1];

	float	delta = timeSinceLastFrame ();

	/*if(vel.length() != 0)
		vel.normalize();*/

	Vector3D	step ( vel * delta );

	Array		colliders ( "game colliders" );
//	BoundingBox	box       ( pos, pos + step );

	//box.grow ( size );

	//getModel () -> getColliders ( box, colliders );

	//pos = getPosition ( colliders, pos, step, size );
	pos += step;

	camera -> setEulerAngles ( yaw, pitch, roll );
	camera -> setPos         ( pos );

	return controllerContinue;
}
/*
void	GameController :: addConsoleCommand ( ConsoleCommand * com )
{
	console -> addCommand ( com );
}*/

InputReader * GameController :: createInputReader ( View * view )
{
	return new DirectXInputReader ( view, "DirectX input reader" );
}
