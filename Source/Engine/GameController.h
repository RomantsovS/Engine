//
// Basic class to manage game (or walkthrough)
// Processes user input and rendering of the scene
//
// Author: Alex V. Boreskoff
//
// Last modified: 2/04/2003
//

#ifndef __GAME_CONTROLLER__
#define __GAME_CONTROLLER__

#include	"../Interface/Controller.h"
#include	"../3D/Vector3D.h"

class	Console;
class	ResourceManager;
class	ConsoleCommand;

class    GameController : public Controller
{
protected:
    Vector3D	size;						// size of player (used for collision detection)
	float		mouseSensitivity;			// mouse sensitivity

	Vector3D		  gravity;					// gravity vector
	Vector3D		  velocity;
	float			  forwardSpeed;				// speed for moving forward
	float		      backwardSpeed;
	float		      strafeSpeed;				// speed for strafe

	int        forwardKey, forwardKey2;
    int        backwardKey, backwardKey2;
    int        leftKey, leftKey2;
    int        rightKey, rightKey2;
	int        upKey, downKey;
	int		   strafeLeftKey, strafeLeftKey2;
	int		   strafeRightKey, strafeRightKey2;
	int        escapeKey;

public:
    GameController ( const char * theName, Model * theModel, View * theView, Camera * theCamera, const Vector3D& defSize );
    ~GameController ();

	void	addConsoleCommand ( ConsoleCommand * com );

	void	setMouseSensitivity ( float val )
	{
		mouseSensitivity = val;
	}

	virtual	InputReader * createInputReader ( View * );

	virtual	int		handleKeyboard ( const KeyboardState& keys  );
	virtual	int		handleMouse    ( const MouseState&    mouse );

	const Vector3D& getGravity () const
	{
		return gravity;
	}

	void	setGravity ( float g )
	{
		gravity.y = -g;
	}

	void	setForwardSpeed ( float val )
	{
		forwardSpeed = val;
	}

	void	setBackwardSpeed ( float val )
	{
		backwardSpeed = val;
	}

	void	setStrafeSpeed ( float val )
	{
		strafeSpeed = val;
	}
};

#endif
