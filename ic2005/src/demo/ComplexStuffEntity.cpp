#include "stdafx.h"
#include "ComplexStuffEntity.h"


// --------------------------------------------------------------------------


CComplexStuffEntity::CComplexStuffEntity( const char* defaultAnim )
{
	mAnimator = new CCharacterAnimator();
	CAnimationBunch& anim = *RGET_ANIM(defaultAnim);
	float animDuration = gGetAnimDuration(anim,false);
	mAnimator->setDefaultAnim( anim, animDuration, 0.1f );
	mAnimator->playDefaultAnim();

	CSkinMesh* skinMesh = RGET_SKIN( "BicasLow" );
	mSkinUpdater = new CSkinUpdater( skinMesh->getSkeleton(), *mAnimator );
	
	for( int i = 0; i < RMCOUNT; ++i ) {
		mMesh[i] = NULL;
		// some objects don't get all rendermodes
		CModelDesc* desc = RTRYGET_MDESC( RMODE_PREFIX[i] + std::string("Bicas") );
		if( !desc )
			continue;

		CRenderableSkin* rskin = new CRenderableSkin( *skinMesh, 0,
				mSkinUpdater->getSkinMatrices(), "mSkin", "iBones",
				&mAnimator->getRootMatrix().getOrigin(), desc->getRenderPriority(0) );
		CEffectParams& ep = rskin->getParams();
		desc->fillFxParams( 0, ep );
		mMesh[i] = rskin;
	}

	//
	// position

	SMatrix4x4& m = mAnimator->getRootMatrix();
	m.identify();
}

CComplexStuffEntity::~CComplexStuffEntity()
{
	for( int i = 0; i < RMCOUNT; ++i )
		safeDelete( mMesh[i] );
	safeDelete( mSkinUpdater );
	safeDelete( mAnimator );
}

void CComplexStuffEntity::render( eRenderMode renderMode )
{
	if( mMesh[renderMode] )
		G_RENDERCTX->attach( *mMesh[renderMode] );
}

void CComplexStuffEntity::update()
{
	// animate
	mAnimator->updateLocal();
	// any local animations here
	mAnimator->updateWorld();
	mSkinUpdater->update();
}