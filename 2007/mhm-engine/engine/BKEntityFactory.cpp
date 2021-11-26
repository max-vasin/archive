//==========================================================================================
// BIOKRIZ entity factory class implementation
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "BKGameCamera.h"
#include "BKCrosshair.h"
#include "BKHero.h"
#include "BKCreature.h"
#include "BKFlyer.h"
#include "BKSucker.h"
#include "BKOrc.h"
#include "BKBug.h"
#include "BKCreatureInstance.h"
#include "BKCreatureEmitter.h"
#include "BKDirector.h"
#include "BKBaseDecal.h"
#include "BKParticle.h"
#include "BKParticleEmitter.h"
#include "BKAllParticles.h"
#include "BKWeapon.h"
#include "BKSpriteParticles.h"
#include "BKBonus.h"
#include "BKBonusManager.h"

#include "BKEntityFactory.h"

// Entity factory constructor
//==========================================================================================
clsBiokrizEntityFactory::clsBiokrizEntityFactory(clsEngine *pEngine):
						 clsEntityFactory(pEngine)
{
}

// Entity factory destructor
//==========================================================================================
clsBiokrizEntityFactory::~clsBiokrizEntityFactory()
{
}

// CreateEntity: creates new entity objects
//==========================================================================================
clsEntity* clsBiokrizEntityFactory::CreateEntity(const clsString &type,
												 const clsString &name)
{
	clsString sType(type);
	sType.UpperCase();
	clsString entname(name);
	entname.UpperCase();
	//g_plog->Out("Entity factory: creates new entity [" + entname + "]");
	clsEntity* pEntity = mmnull;
	if (sType == "GAMECAMERA")
		pEntity = new clsBKGameCamera(this, type, name);
	if (sType == "CROSSHAIR")
		pEntity = new clsBKCrosshair(this, type, name);
	if (sType == "HERO")
		pEntity = new clsBKHero(this, type, name);
	if (sType == "MONSTER")
		pEntity = new clsBKCreature(this, type, name);
	if (sType == "CREATUREEMITTER")
		pEntity = new clsBKCreatureEmitter(this, type, name);
//	if (sType == "CREATUREINSTANCE")
//		pEntity = new clsBKCreatureInstance(this, type, name);
	if (entname.Left(8) == "DIRECTOR")
		pEntity = new clsBKDirector(this, type, name);
	if (entname == "BLOODYPLACE")
		pEntity = new clsBKBaseDecal(this, type, name);
	if (sType == "PARTICLE")
		pEntity = new clsBKParticle(this, type, name);
	if (sType == "PARTICLEEMITTER")
		pEntity = new clsBKParticleEmitter(this, type, name);
	if (sType == "WEAPON")
		pEntity = new clsBKWeapon(this, type, name);
	if (sType == "FLYER")
		pEntity = new clsBKFlyer(this, type, name);
	if (sType == "SUCKER")
		pEntity = new clsBKSucker(this, type, name);
	if (sType == "ORC")
		pEntity = new clsBKOrc(this, type, name);
	if (sType == "BUG")
		pEntity = new clsBKBug(this, type, name);
	if (sType == "BLOODPARTICLES")
		pEntity = new clsBKSpriteParticles(this, type, name);
	if (sType == "SPRITEPARTICLES")
		pEntity = new clsBKSpriteParticles(this, type, name);
	if (sType == "BONUS")
		pEntity = new clsBKBonus(this, type, name);
	if (sType == "BONUSMANAGER")
		pEntity = new clsBKBonusManager(this, type, name);
	
	if (pEntity == mmnull)
		pEntity = new clsEntity(this, type, name);
	
	return pEntity;
}


