#include "stdafx.h"
#include "SceneShared.h"
#include "wallz/WallPieces.h"
#include "wallz/WallPhysics.h"
#include "wallz/WallFracturer.h"
#include <dingus/utils/FixedRateProcess.h>



const float PHYS_UPDATE_FREQ = 60.0f;
const float PHYS_UPDATE_DT = 1.0f / PHYS_UPDATE_FREQ;


class CPhysicsProcess : public CFixedRateProcess {
public:
	CPhysicsProcess() : CFixedRateProcess( PHYS_UPDATE_FREQ, 50 ) { }
protected:
	virtual void performProcess() {
		wall_phys::update();
	};
};

CPhysicsProcess	gPhysProcess;



CSceneSharedStuff::CSceneSharedStuff()
{
	// walls
	const float ELEM_SIZE = 0.1f; // 0.1f

	mWalls[CFACE_PX] = new CWall3D( SVector2(ROOM_SIZE.z,ROOM_SIZE.y), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_PX] );
	mWalls[CFACE_NX] = new CWall3D( SVector2(ROOM_SIZE.z,ROOM_SIZE.y), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_NX] );
	mWalls[CFACE_PY] = new CWall3D( SVector2(ROOM_SIZE.x,ROOM_SIZE.z), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_PY] );
	mWalls[CFACE_NY] = new CWall3D( SVector2(ROOM_SIZE.x,ROOM_SIZE.z), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_NY] );
	mWalls[CFACE_PZ] = new CWall3D( SVector2(ROOM_SIZE.x,ROOM_SIZE.y), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_PZ] );
	mWalls[CFACE_NZ] = new CWall3D( SVector2(ROOM_SIZE.x,ROOM_SIZE.y), ELEM_SIZE, gNoPixelShaders ? NULL : WALL_TEXS[CFACE_NZ] );

	SMatrix4x4 wm;
	wm.identify();
	
	wm.getAxisX().set( 0, 0, 1 );
	wm.getAxisY().set( 0, 1, 0 );
	wm.getAxisZ().set( -1, 0, 0 );
	wm.getOrigin().set( ROOM_MAX.x, ROOM_MIN.y, ROOM_MIN.z );
	mWalls[CFACE_PX]->setMatrix( wm );
	wm.getAxisX().set( 0, 0, -1 );
	wm.getAxisY().set( 0, 1, 0 );
	wm.getAxisZ().set( 1, 0, 0 );
	wm.getOrigin().set( ROOM_MIN.x, ROOM_MIN.y, ROOM_MAX.z );
	mWalls[CFACE_NX]->setMatrix( wm );
	wm.getAxisX().set( 1, 0, 0 );
	wm.getAxisY().set( 0, 0, 1 );
	wm.getAxisZ().set( 0, -1, 0 );
	wm.getOrigin().set( ROOM_MIN.x, ROOM_MAX.y, ROOM_MIN.z );
	mWalls[CFACE_PY]->setMatrix( wm );
	wm.getAxisX().set( 1, 0, 0 );
	wm.getAxisY().set( 0, 0, -1 );
	wm.getAxisZ().set( 0, 1, 0 );
	wm.getOrigin().set( ROOM_MIN.x, ROOM_MIN.y, ROOM_MAX.z );
	mWalls[CFACE_NY]->setMatrix( wm );
	wm.getAxisX().set( -1, 0, 0 );
	wm.getAxisY().set( 0, 1, 0 );
	wm.getAxisZ().set( 0, 0, -1 );
	wm.getOrigin().set( ROOM_MAX.x, ROOM_MIN.y, ROOM_MAX.z );
	mWalls[CFACE_PZ]->setMatrix( wm );
	wm.getAxisX().set( 1, 0, 0 );
	wm.getAxisY().set( 0, 1, 0 );
	wm.getAxisZ().set( 0, 0, 1 );
	wm.getOrigin().set( ROOM_MIN.x, ROOM_MIN.y, ROOM_MIN.z );
	mWalls[CFACE_NZ]->setMatrix( wm );

	int i;

	for( i = 0; i < CFACE_COUNT; ++i )
		wallFractureCompute( mWalls[i]->getWall2D() );

	wall_phys::initialize( PHYS_UPDATE_DT, ROOM_MIN-SVector3(1.0f,1.0f,1.0f), ROOM_MAX+SVector3(1.0f,1.0f,1.0f) );

	for( i = 0; i < CFACE_COUNT; ++i ) {
		wall_phys::addWall( *mWalls[i] );
	}

	for( i = 0; i < CFACE_COUNT; ++i )
		mWalls[i]->update( 0.0f );
}

CSceneSharedStuff::~CSceneSharedStuff()
{
	wall_phys::shutdown();
	for( int i = 0; i < CFACE_COUNT; ++i )
		delete mWalls[i];
}


void CSceneSharedStuff::renderWalls( eRenderMode rm )
{
	for( int i = 0; i < CFACE_COUNT; ++i ) {
		mWalls[i]->render( rm );
	}
	wall_phys::render( rm );
}


void CSceneSharedStuff::updatePhysics()
{
	gPhysProcess.perform();
}

void CSceneSharedStuff::updateFracture( float demoTimeS )
{
	for( int i = 0; i < CFACE_COUNT; ++i ) {
		mWalls[i]->update( demoTimeS );
	}
}
