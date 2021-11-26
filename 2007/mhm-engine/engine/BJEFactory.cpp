//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "mhmbase.h"
#include "BJDefs.h"
#include "BJBrick.h"
#include "BJGameMatrix.h"
#include "BJCursor.h"
#include "BJEFactory.h"

// Entity factory constructor
//--------------------------------------------------
clsBJEntityFactory::clsBJEntityFactory(clsEngine *pEngine):
					clsEntityFactory(pEngine)
{

}

// Entity factory destructor
//--------------------------------------------------
clsBJEntityFactory::~clsBJEntityFactory()
{

}

// Entity factory
//--------------------------------------------------
clsEntity* clsBJEntityFactory::CreateEntity(const clsString &sType, const clsString &sName)
{
	clsString sUType = sType;
	sUType.UpperCase();
	if (sUType == "GAMEMATRIX")
		return new clsBJGameMatrix(this, sType, sName);
	if (sUType == "BRICK")
		return new clsBJBrick(this, sType, sName);
	if (sUType == "CURSOR")
		return new clsBJCursor(this, sType, sName);
	
	return new clsEntity(this, sType, sName);
}


