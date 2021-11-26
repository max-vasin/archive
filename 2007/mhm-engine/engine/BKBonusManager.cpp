//==========================================================================================
// BIOKRIZ bonus manager
//==========================================================================================
#include "mhmbase.h"
#include "BKBonusManager.h"

// Bonus manager constructor
//=======================================================
clsBKBonusManager::clsBKBonusManager(clsEntityFactory *pFactory, 
									 const clsString &sType,
									 const clsString &sName):
				   clsEntity(pFactory, sType, sName),
				   m_arBonuses(200, 100)
{
	m_bNeedLoad		    = mmtrue;
	m_nDeathCounter     = 0;
	m_nTotalQualifier   = 0;
	m_nScore		    = 0;
	m_pText			    = mmnull;
	m_bEnableSphereTest = mmfalse;
	m_bRenderable		= mmtrue;
}

// Bonus manager destructor
//=======================================================
clsBKBonusManager::~clsBKBonusManager()
{

}

// Bonus manager info
//=======================================================
mmRes clsBKBonusManager::OnRender()
{
/*	clsString sStats;
	sStats = "SCORE: " + clsString(m_nScore);
	sStats += "\n\brFRAGS: " + clsString(m_nDeathCounter);
	m_pText->SetText(sStats);
	m_pText->Render();*/
	return mmOk;
	
}

// Bonus manager initializing
//=======================================================
mmVoid clsBKBonusManager::OnInitialize(mmBool bClone)
{
	m_pText = (clsText*)Renderer->GetResourceManager()->GetResource("myfont.mhmFont2D",
																	  MHMRES_MISC_ID);
	m_pText->SetAlignX(enLeft);
	m_pText->SetAlignY(enTop);
	m_pText->SetColor(0xFF0000FF);
	// collect all bonuses
	EnumChilds("Bonus", this);
	
	// calulate total appearance
	for (mmUint i = 0; i < m_arBonuses.Size(); i++)
		m_nTotalQualifier += ((clsBKBonus*)m_arBonuses[i])->GetAppearanceFactor();
}

// Bonus manager loading
//=======================================================
mmVoid clsBKBonusManager::OnPostLoading()
{
	LoadMJ("meshes\\bonuziz.mj");
}

// Bonuses collecting
//=======================================================
mmVoid clsBKBonusManager::OnEnumChild(clsEntity *pEntity)
{
	m_arBonuses.Add(pEntity);	
}

// Bonuses collecting
//=======================================================
mmVoid clsBKBonusManager::RegisterDeath(clsEntity* pEntity)
{
	m_nDeathCounter++;
	m_nScore += 10;
	if ((m_nDeathCounter % 10) == 0)
		GenerateBonus(pEntity->m_mxFull.GetTranslation());
	
}

// Bonuses generation
//=======================================================
mmVoid clsBKBonusManager::AddPoints(mmInt nCount)
{
	m_nScore += nCount;
}

// Bonuses generation
//=======================================================
mmVoid clsBKBonusManager::GenerateBonus(const clsVector3& vWhere)
{
	mmInt iDescriptor = (mmInt)(g_pEnvironment->m_cRand.frand(1.0f) * (mmFloat)m_nTotalQualifier);
	clsEntity* pBonus = mmnull;
	mmInt	   nPrevValue = 0;
	clsMatrix mxWhere;
	mxWhere.Translation(vWhere);
	for (mmUint i = 0; i < m_arBonuses.Size(); i++)
	{
		
		if ((iDescriptor > nPrevValue) && (iDescriptor <= ((clsBKBonus*)m_arBonuses[i])->GetAppearanceFactor()))
			pBonus = ((clsBKBonus*)m_arBonuses[i])->Clone("Bonus", mmtrue, &mxWhere);
	}
	if (pBonus == mmnull)
		return;
	pBonus->SetOwner(FindRoot());
	((clsBKBonus*)pBonus)->m_pBonusManager = this;
}




