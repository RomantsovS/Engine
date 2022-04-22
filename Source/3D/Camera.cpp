//
// Camera class for OpenGL-based rendering
//
// Author: Alex V. Boreskoff
//
// Last change 5/12/2002
//

#include    "Camera.h"
#include "Math1D.h"
//#include	"Transform3D.h"

Camera :: Camera ( const char * theName, const Vector3D& p, float yaw, float pitch, float roll,
            float aFov, float nearZ, float farZ ) : Object ( theName )
{
    fov       = aFov;
	zNear     = nearZ;
	zFar      = farZ;
    pos       = p;
    mirrored  = 0;
	width     = 640;					// default view size
	height    = 480;
	aspect    = (float)width / (float) height;

    setEulerAngles ( yaw, pitch, roll );
}

Camera :: Camera ( const Camera& camera ) : Object ( camera.getName () )
{
    zNear     = camera.zNear;
    zFar      = camera.zFar;
    fov       = camera.fov;
    pos       = camera.pos;
    viewDir   = camera.viewDir;
    upDir     = camera.upDir;
    mirrored  = camera.mirrored;
	width     = camera.width;
	height    = camera.height;
	aspect    = camera.aspect;

    computeMatrix ();
}

void    Camera :: setEulerAngles ( float yaw, float pitch, float roll )
{
	viewDir  = Vector3D ( 0, 0, 1 );
    rightDir = Vector3D ( 1, 0, 0 );
    upDir    = Vector3D ( 0, 1, 0 );

    Matrix3D  rot ( Matrix3D :: getRotateYawPitchRollMatrix ( yaw, pitch, roll ) );

    viewDir  = rot * viewDir;
    //upDir    = rot * upDir;
    rightDir = rot * rightDir;

    computeMatrix ();
}

void    Camera :: setFov ( float newFovAngle )
{
    fov = newFovAngle;

    computeMatrix ();
}

void	Camera :: setViewSize ( int theWidth, int theHeight, float theFov )
{
	width  = theWidth;
	height = theHeight;
	fov    = theFov;
	aspect = (float)width / (float) height;

	computeMatrix ();
}

void	Camera :: setAspect ( float newAspect )
{
	aspect = newAspect;

	computeMatrix ();
}

void    Camera :: computeMatrix ()
{
    viewDir.normalize ();       // orthonormalize viewDir
                                // compute upDir
    upDir -= (upDir & viewDir) * viewDir;

                                // compute rightDir
    rightDir = upDir ^ viewDir;

    upDir.normalize    ();      // orthonormalize upDir and rightDir
    rightDir.normalize ();

    if ( mirrored )
       rightDir = -rightDir;

	float	halfAngle = 0.5f * M_PI * fov / 180.0f;
    float	fovValueY = 1 / (float)tan ( halfAngle );
	float	fovValueX = fovValueY / aspect;
                                // now build transform matrix
    transf [0][0] = (rightDir [0] * fovValueX + viewDir [0]) * 0.5f * width;
    transf [0][1] = (rightDir [1] * fovValueX + viewDir [1]) * 0.5f * width;
    transf [0][2] = (rightDir [2] * fovValueX + viewDir [2]) * 0.5f * width;
    transf [1][0] = (upDir    [0] * fovValueY + viewDir [0]) * 0.5f * height;
    transf [1][1] = (upDir    [1] * fovValueY + viewDir [1]) * 0.5f * height;
    transf [1][2] = (upDir    [2] * fovValueY + viewDir [2]) * 0.5f * height;
    transf [2][0] = viewDir   [0];
    transf [2][1] = viewDir   [1];
    transf [2][2] = viewDir   [2];

                                // rebuild view frustrum
	Vector3D	v  ( viewDir  * zFar );
	Vector3D	hr ( rightDir * (zFar / fovValueX) );
	Vector3D	hu ( upDir    * (zFar / fovValueY) );
	Vector3D	vv [4];

	vv [0] = pos + v + hr + hu;
	vv [1] = pos + v - hr + hu;
	vv [2] = pos + v - hr - hu;
	vv [3] = pos + v + hr - hu;

    viewFrustrum.set         ( pos, 4, vv );
	viewFrustrum.setFarPlane ( Plane ( -viewDir, pos + zFar*viewDir ) );
/*
	for ( int i = 0; i < 4; i++ )
	{
		Vector3D pr ( mapToScreen ( vv [i] ) );

		pr.z = 0;
	}
*/
}

void    Camera :: mirror ( const Plane& mirror )
{
    mirror.reflectPos   ( pos );
    mirror.reflectDir   ( viewDir );
    mirror.reflectDir   ( upDir );
    mirror.reflectDir   ( rightDir );

    mirrored = !mirrored;

    computeMatrix ();
}

void	Camera :: buildHomogeneousMatrix ( float m [16] ) const
{
	m [0 ] = rightDir.x;
	m [1 ] = upDir.x;
	m [2 ] = -viewDir.x;
	m [3 ] = 0.0f;

	m [4 ] = rightDir.y;
	m [5 ] = upDir.y;
	m [6 ] = -viewDir.y;
	m [7 ] = 0.0f;

	m [8 ] = rightDir.z;
	m [9 ] = upDir.z;
	m [10] = -viewDir.z;
	m [11] = 0.0f;

	m [12] = 0.0f;
	m [13] = 0.0f;
	m [14] = 0.0f;
	m [15] = 1.0f;
}
/*
void	Camera :: transform ( const Transform3D& tr )
{
	pos      = tr.transformPoint ( pos );
	viewDir  = tr.transformDir   ( viewDir );
	upDir    = tr.transformDir   ( upDir );
	rightDir = tr.transformDir   ( rightDir );

							// now check for right/left handedness
	if ( ((upDir ^ viewDir) & rightDir) < -EPS )
		mirrored = true;

	computeMatrix ();
}
*/

void Camera::buidProjectionPerspectiveMatrix(float m[16]) const
{
	float f = ctg(getRadiansFromGradus(fov) / 2);
	m[0] = f / aspect;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = f;
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = (zFar + zNear) / (zNear - zFar);
	m[11] = -1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 2 * zFar * zNear / (zNear - zFar);
	m[15] = 0.0f;
}

void Camera::buildModelViewMatrix(float m[]) const
{
	float hM[16];
	buildHomogeneousMatrix(hM);
	float x = -pos.x;
	float y = -pos.y;
	float z = -pos.z;

	m[ 0] = hM[0] * 1.0f + hM[1] * 0.0f + hM[2] * 0.0f + hM[3] * 0.0f;
	m[ 4] = hM[0] * 0.0f + hM[1] * 1.0f + hM[2] * 0.0f + hM[3] * 0.0f;
	m[ 8] = hM[0] * 0.0f + hM[1] * 0.0f + hM[2] * 1.0f + hM[3] * 0.0f;
	m[12] = hM[0] * x + hM[1] * y + hM[2] * z + hM[3] * 1.0f;

	m[ 1] = hM[4] * 1.0f + hM[5] * 0.0f + hM[6] * 0.0f + hM[7] * 0.0f;
	m[ 5] = hM[4] * 0.0f + hM[5] * 1.0f + hM[6] * 0.0f + hM[7] * 0.0f;
	m[ 9] = hM[4] * 0.0f + hM[5] * 0.0f + hM[6] * 1.0f + hM[7] * 0.0f;
	m[13] = hM[4] * x + hM[5] * y + hM[6] * z + hM[7] * 1.0f;

	m[ 2] = hM[8] * 1.0f + hM[9] * 0.0f + hM[10] * 0.0f + hM[11] * 0.0f;
	m[ 6] = hM[8] * 0.0f + hM[9] * 1.0f + hM[10] * 0.0f + hM[11] * 0.0f;
	m[10] = hM[8] * 0.0f + hM[9] * 0.0f + hM[10] * 1.0f + hM[11] * 0.0f;
	m[14] = hM[8] * x + hM[9] * y + hM[10] * z + hM[11] * 1.0f;

	m[ 3] = hM[12] * 1.0f + hM[13] * 0.0f + hM[14] * 0.0f + hM[15] * 0.0f;
	m[ 7] = hM[12] * 0.0f + hM[13] * 1.0f + hM[14] * 0.0f + hM[15] * 0.0f;
	m[11] = hM[12] * 0.0f + hM[13] * 0.0f + hM[14] * 1.0f + hM[15] * 0.0f;
	m[15] = hM[12] * x + hM[13] * y + hM[14] * z + hM[15] * 1.0f;
}