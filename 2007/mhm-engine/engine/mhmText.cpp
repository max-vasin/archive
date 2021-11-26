
#include "mhmbase.h"

#define _rhw (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define _norhw (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/*
clsFontCharInfo::clsFontCharInfo()
{
	Width = Height = 0.0f;
	UV.left = UV.top = UV.right = UV.bottom = 0.0f;
	MatID = -1;
}
clsFontCharInfo::~clsFontCharInfo()
{
}
//------------------------------------------------------------------------
clsFontInfo::clsFontInfo(clsResourceManager* presman, clsString name):
				  clsResource(presman, name, enResFont)
{
	m_chars.Size(256);
	m_chars.AutoTruncate = mmfalse;
}
clsFontInfo::~clsFontInfo()
{
	for(mmUint z=0;z<m_map.Size();z++)
		mmSafeRelease(m_map[z]);
}
mmRes clsFontInfo::Create(clsResourceParams *params)
{
	mmByte * dat;

	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);
	
	if (!file.IsExist())
	{
		g_plog->Out("clsText:Create[" + m_name + "] >> file not exist or could not be opened", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsText:Create[" + m_name + "] >> cant open file", enLCRed);
		return mmFail;
	}
	mmInt f_size;
	f_size = file.GetSize();
	if (f_size < 9)
	{
		g_plog->Out("clsText:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}
	dat = new mmByte[f_size];
	if (MHMFAIL(file.Read(dat, f_size)))
	{
		delete [] dat;
		g_plog->Out("clsText:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}
	if(strcmp((mmString)dat,"mhmFont1")!=0)
	{
		delete [] dat;
		g_plog->Out("clsText:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	//--------
	clsFontCharInfo * f = new clsFontCharInfo();

	mmInt i = 9;
	mmInt ic;
	mmFloat w,h;
	mmInt i_i = 0;
	mmFloat asp;
	clsString name;
	mmFloat HeightUV;
	clsBaseMaterial * mat;
	while(i < f_size-1)
	{

		//f = new clsItemFont;
		

		name = (mmString)(dat+i);
		i += name.Length()+1;

		i += 4;

		m_map.Add((clsBaseMaterial*)m_presman->GetResource(name, enResBaseMaterial));
		f->MatID = m_map.Size()-1;
		mat = m_map[f->MatID];

		if(mat == mmnull)
		{
			delete [] dat;
			g_plog->Out("clsText:Create[" + name + "] >> file header reading error", enLCRed);
			return mmFail;
		}

		clsBaseTextureParams tp;
		tp.m_alpha     =  ((*(dat+i)) != 0);
		i++;
		tp.m_mipgen    = mmfalse;
		tp.m_levels    = 1;
		tp.m_rentarget = mmfalse;
		tp.m_key_enable = ((*(dat+i)) != 0);
		i++;
		if (tp.m_key_enable)
		{
			// copy color key
			tp.m_key_low.r = *(dat+i); i++;  
			tp.m_key_low.g = *(dat+i); i++; 
			tp.m_key_low.b = *(dat+i); i++; 
			tp.m_key_hi    = tp.m_key_low;
			tp.m_key_alpha = 0;
		}else{ i+=3; }

		mat->m_map_diffuse.m_texture = (clsBaseTexture*)m_presman->GetResource(name, MHMRES_TEXTURE_ID,&tp);

		if (tp.m_alpha)
			mat->SetBlendType(enBlendAlpha);

		if(mat->m_map_diffuse.m_texture == mmnull)
		{
			delete [] dat;
			g_plog->Out("clsText:Create[" + name + "] >> file header reading error", enLCRed);
			return mmFail;
		}

		mat->m_map_diffuse.m_filtering_enable = mmfalse;
		
		

		mat->m_dxmtl.Diffuse.a = 1.0f;
		mat->m_dxmtl.Diffuse.r = 1.0f;
		mat->m_dxmtl.Diffuse.g = 1.0f;
		mat->m_dxmtl.Diffuse.b = 1.0f;
		mat->m_dxmtl.Ambient = mat->m_dxmtl.Diffuse;

		w = (mmFloat)mat->m_map_diffuse.m_texture->GetWidth();
		h = (mmFloat)mat->m_map_diffuse.m_texture->GetHeight();
		asp = h/w;

	//	f->m_HeightUV = (mmFloat)(*(dat+i)) * asp / h;
		HeightUV = (mmFloat)(*(dat+i)) * asp / h;
		i++;
	//	f->m_Height = (mmFloat)(*(dat+i)) * asp / h;
		f->Height = (mmFloat)(*(dat+i)) * asp / h;
		i++;
	//	f->m_x = (mmFloat)(*(dat+i)) / w;
		i++;
	//	f->m_y = (mmFloat)(*(dat+i)) * asp / h;
		i++;

		while( (*(dat+i)) != 0 )
		{
			
			ic = (mmInt)(*(dat+i));
			//m_chars[ic].



		//	f->m_chars[ic].en = mmtrue;
			i++;
		//	f->m_chars[ic].x1 = (mmFloat)(*((mmWord*)(dat+i))) / w;
			f->UV.left = (mmFloat)(*((mmWord*)(dat+i))) / w;
			i+=2;
		//	f->m_chars[ic].y1 = ((mmFloat)(*(dat+i))) * f->m_HeightUV / asp;
			f->UV.top = ((mmFloat)(*(dat+i))) * HeightUV / asp;
			i++;
		//	f->m_chars[ic].x2 = f->m_chars[ic].x1 +  (((mmFloat)(*(dat+i)))/w);
			f->UV.right = f->UV.left +  (((mmFloat)(*(dat+i)))/w);
			i++;
		//	f->m_chars[ic].y2 = f->m_chars[ic].y1 + f->m_HeightUV / asp;
			f->UV.bottom = f->UV.top + HeightUV / asp;

			f->Width = (((mmFloat)(*(dat+i)))/w);
			i++;

			m_chars[ic] = *f;
		}
		i++;

		//items.Add(f);
		//f->id = i_i;
		i_i++;
	}
	delete f;
	delete [] dat;
	g_plog->Out("clsText:Create[" + m_name + "] >> loaded successfully", enLCGreen);
	return mmOk;
}
//***********************************************************--------------
clsMegaText::clsMegaText(clsResourceManager* presman, clsString name):
				  clsBaseMesh(presman, name, enResStaticMeshMMH)
{
	m_items.AutoTruncate = mmfalse;
	m_font = mmnull;	
	m_ch_fvf = _rhw;
}
//----------------------------------------------------------------------------
clsMegaText::~clsMegaText(void)
{
	mmSafeRelease(m_font);
}
//----------------------------------------------------------------------------
mmRes clsMegaText::OnCreateMesh(clsResourceParams *params)
{
	m_font = (clsFontInfo*)m_presman->GetResource(m_name, enResFont);
	
	if (m_font == mmnull)
	{
		g_plog->Out("clsText:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	g_plog->Out("clsText:Create[" + m_name + "] >> loaded successfully", enLCGreen);


	mmInt count_m=m_font->m_map.Size();

	//m_Count_FC.Size(m_font->items.Count());
	//m_Pos_V.Size(m_font->items.Count());

	m_materials  = new clsResource*[count_m];
	m_psubsets = new clsBaseMeshSubset[count_m];
	m_numsubs = count_m;
	m_nummtls = m_numsubs;
	for(mmInt z=0;z<count_m;z++)
	{
		m_psubsets[z].m_count = 0;
		m_psubsets[z].m_start = 0;
		m_psubsets[z].m_mtlindex = z;

		mmBool fail = mmfalse;
		
		m_materials[z] = (clsBaseMaterial*)m_presman->GetResource(m_font->m_map[z]->GetName(), enResBaseMaterial, mmnull, &fail);;//(clsBaseMaterial*)m_presman->GetResource(m_font->m_chars[, enResBaseMaterial, mmnull, &fail);

		if (!fail)
			m_size = (mmFloat)((clsBaseMaterial*)m_materials[z])->m_map_diffuse.m_texture->GetWidth();
		else
			m_size = 0;
	}

	m_basevbuf = m_presman->CreateVertexBuffer();
	m_baseibuf = m_presman->CreateIndexBuffer();

	m_basevbuf->Create(4,m_ch_fvf);
	m_baseibuf->Create(6);

	return mmOk;
}
mmVoid clsMegaText::SetText(const mmString str)
{
	m_txt = str;
}
mmVoid clsMegaText::Build()
{
	clsFontCharInfo chi;
	mmChar ch;
	mmInt ItemI = 0;
	m_items.Size(1);
	for(mmUint z = 0;z < m_txt.Length();z++)
	{
		ch = m_txt[z];
		if( ch == '\n' )
		{
			ItemI++;
		}


		chi = m_font->Find(ch);
		if(chi.MatID == -1)
			continue;
	
		if(ch != ' ')
		{
			m_items[ItemI].CharsWidth += chi.Width;
			m_items[ItemI].WidthPostLastSpace += chi.Width;
		}
		else
		{
			m_items[ItemI].LastSpace = z;
			m_items[ItemI].SpaceWidth += chi.Width;
			m_items[ItemI].SpaceCount ++;
			m_items[ItemI].WidthPostLastSpace = chi.Width;
		}
		if(m_items[ItemI].ItemHeight < chi.Height)
			m_items[ItemI].ItemHeight = chi.Height;

		if( m_wrap & m_use_rect & ((m_items[ItemI].CharsWidth + m_items[ItemI].SpaceWidth) > m_rect.Width() ) )
		{
			if(m_items[ItemI].SpaceCount == 0)
			{
				m_items[ItemI].CharsWidth -= chi.Width;
				ItemI++;
				z--;
			}else{
				m_items[ItemI].CharsWidth -= m_items[ItemI].WidthPostLastSpace;			
				z = m_items[ItemI].LastSpace+1;
				ItemI++;
			}
		}





	}







}
























*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//----------------------------------------------------------------------------
clsFont::clsFont(clsResourceManager* presman, clsString name):
				  clsResource(presman, name, enResFont)
{
}
//----------------------------------------------------------------------------
clsFont::~clsFont(void)
{
	items.ResetSelect();
	while(items.SelectNext())
		delete items.Selected();
}
//----------------------------------------------------------------------------
mmRes clsFont::Create(clsResourceParams *params)
{
	mmByte * dat;

	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);
	
	if (!file.IsExist())
	{
		g_plog->Out("clsFont:Create[" + m_name + "] >> file not exist or could not be opened", enLCRed);
		return mmFail;
	}
	
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsFont:Create[" + m_name + "] >> cant open file", enLCRed);
		return mmFail;
	}

	mmInt f_size;
	f_size = file.GetSize();

	if (f_size < 9)
	{
		g_plog->Out("clsFont:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	dat = new mmByte[f_size];
	
	if (MHMFAIL(file.Read(dat, f_size)))
	{
		delete [] dat;
		g_plog->Out("clsFont:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	if(strcmp((mmString)dat,"mhmFont1")!=0)
	{
		delete [] dat;
		g_plog->Out("clsFont:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	//--------
	clsItemFont * f;

	mmInt i = 9;
	mmInt ic;
	mmFloat w,h;
	mmInt i_i = 0;
	mmFloat asp;

	while(i < f_size-1)
	{

		f = new clsItemFont;
		

		f->m_name = (mmString)(dat+i);
		i += f->m_name.Length()+1;

		i += 4;

	//	clsString nnn;
	//	nnn = "123";
		f->m_mat = (clsBaseMaterial*)m_presman->GetResource(f->m_name, enResBaseMaterial);
//		f->m_mat = (clsBaseMaterial*)m_presman->GetResource("", enResBaseMaterial);
		

		if(f->m_mat == mmnull)
		{
			delete [] dat;
			g_plog->Out("clsFont:Create[" + f->m_name + "] >> file header reading error", enLCRed);
			return mmFail;
		}



		clsBaseTextureParams tp;
		tp.m_alpha     =  ((*(dat+i)) != 0);
		i++;
		tp.m_mipgen    = mmfalse;
		tp.m_levels    = 1;
		tp.m_rentarget = mmfalse;
		tp.m_key_enable = ((*(dat+i)) != 0);
		i++;
		if (tp.m_key_enable)
		{
			// copy color key
			tp.m_key_low.r = *(dat+i); i++;  
			tp.m_key_low.g = *(dat+i); i++; 
			tp.m_key_low.b = *(dat+i); i++; 
			tp.m_key_hi    = tp.m_key_low;
			tp.m_key_alpha = 0;
		}else{ i+=3; }
		//i+=3;

/*
		f->m_mat->m_need_alphablend = ((*(dat+i)) != 0);
		i++;
		//color key
		i++;
		//color
		i+=3;
*/
		tp.m_levels = 1;
		f->m_mat->m_map_diffuse.m_texture = (clsBaseTexture*)m_presman->GetResource(f->m_name, MHMRES_TEXTURE_ID,&tp);

		if (tp.m_alpha)
			f->m_mat->SetBlendType(enBlendAlpha);

		if(f->m_mat->m_map_diffuse.m_texture == mmnull)
		{
			delete [] dat;
			g_plog->Out("clsFont:Create[" + f->m_name + "] >> file header reading error", enLCRed);
			return mmFail;
		}

		f->m_mat->m_map_diffuse.m_filtering_enable = mmfalse;
		
		

		f->m_mat->m_dxmtl.Diffuse.a = 1.0f;
		f->m_mat->m_dxmtl.Diffuse.r = 1.0f;
		f->m_mat->m_dxmtl.Diffuse.g = 1.0f;
		f->m_mat->m_dxmtl.Diffuse.b = 1.0f;
		f->m_mat->m_dxmtl.Ambient = f->m_mat->m_dxmtl.Diffuse;


		w = (mmFloat)f->m_mat->m_map_diffuse.m_texture->GetWidth();
		h = (mmFloat)f->m_mat->m_map_diffuse.m_texture->GetHeight();
		asp = h/w;

		//i += 5;

		f->m_HeightUV = (mmFloat)(*(dat+i)) * asp / h;
		i++;
		f->m_Height = (mmFloat)(*(dat+i)) * asp / h;
		i++;
		f->m_x = (mmFloat)(*(dat+i)) / w;
		i++;
		f->m_y = (mmFloat)(*(dat+i)) * asp / h;
		i++;

		while( (*(dat+i)) != 0 )
		{
			
			ic = (mmInt)(*(dat+i));
			f->m_chars[ic].en = mmtrue;
			i++;
			f->m_chars[ic].x1 = (mmFloat)(*((mmWord*)(dat+i))) / w;
			i+=2;
			f->m_chars[ic].y1 = ((mmFloat)(*(dat+i))) * f->m_HeightUV / asp;
			i++;
			f->m_chars[ic].x2 = f->m_chars[ic].x1 +  (((mmFloat)(*(dat+i)))/w);
			i++;
			f->m_chars[ic].y2 = f->m_chars[ic].y1 + f->m_HeightUV / asp;

			f->m_chars[ic].width = (((mmFloat)(*(dat+i)))/w);
			i++;
		}
		i++;

		items.Add(f);
		f->id = i_i;
		i_i++;
	}


	delete [] dat;
	g_plog->Out("clsFont:Create[" + m_name + "] >> loaded successfully", enLCGreen);
	return mmOk;
}
//----------------------------------------------------------------------------
clsItemFont * clsFont::Find(mmChar ch, clsCharFont ** ch_info)
{
	items.ResetSelect();
	while(items.SelectNext())
	{
		if(!items.Selected()->m_chars[(mmByte)ch].en)
			continue;
		*ch_info = &(items.Selected()->m_chars[(mmByte)ch]);
		return items.Selected();
	}
	return mmnull;
}
//----------------------------------------------------------------------------
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//----------------------------------------------------------------------------
clsText::clsText(clsResourceManager* presman, clsString name):
				  clsBaseMesh(presman, name, enResStaticMeshMMH)
{
	ok = mmfalse;

	m_rebuild=true;
	m_x=m_y=0;
	m_z = 0.0f;
	m_align_x = enLeft;
	m_align_y = enTop;
	m_use_rect = mmfalse;
	m_size = 1.0f;

	m_tokens.MaxSize(10);
	m_tokens.SetAddSize(10);
	m_tokens.Size(0);
	m_tokens.AutoTruncate = mmfalse;


	m_info.MaxSize(10);
	m_info.SetAddSize(10);
	m_info.Size(0);
	m_info.AutoTruncate = mmfalse;

	m_ch_fvf = _rhw;
	m_indexed = mmtrue;
	m_color = 0xFFFFFFFF;
	m_colored = mmtrue;

	m_aligned = mmtrue;

	m_wrap = mmfalse;

	m_width = 0;
	m_height = 0;

	m_min_y = 0;
	m_max_y = 0;
	m_k_y = 0;
}
//----------------------------------------------------------------------------
clsText::~clsText(void)
{
	mmSafeRelease(m_font);
}
//----------------------------------------------------------------------------
mmRes clsText::OnCreateMesh(clsResourceParams *params)
{
	m_font = (clsFont*)m_presman->GetResource(m_name, enResFont);

	this->m_eBlendType = enBlendAlpha;
	
	if (m_font == mmnull)
	{
		g_plog->Out("clsText:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}

	g_plog->Out("clsText:Create[" + m_name + "] >> loaded successfully", enLCGreen);

	
	

	m_Count_FC.Size(m_font->items.Count());
	m_Pos_V.Size(m_font->items.Count());


	m_materials  = new clsResource*[m_font->items.Count()];
	m_psubsets = new clsBaseMeshSubset[m_font->items.Count()];
	m_numsubs = m_font->items.Count();
	m_nummtls = m_numsubs;
	for(mmUint z=0;z<m_font->items.Count();z++)
	{
		m_psubsets[z].m_count = 0;
		m_psubsets[z].m_start = 0;
		m_psubsets[z].m_mtlindex = z;

		mmBool fail = mmfalse;
		m_materials[z] = (clsBaseMaterial*)m_presman->GetResource(m_font->items[z]->m_name, enResBaseMaterial, mmnull, &fail);

		if (!fail)
			m_size = (mmFloat)((clsBaseMaterial*)m_materials[z])->m_map_diffuse.m_texture->GetWidth();
		else
			m_size = 0;
	}

	m_basevbuf = m_presman->CreateVertexBuffer();
	m_baseibuf = m_presman->CreateIndexBuffer();

	m_basevbuf->Create(4,m_ch_fvf);
	m_baseibuf->Create(6);

	return mmOk;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetFiltering(mmBool f)
{
	for(mmUint z=0;z<m_font->items.Count();z++)
	{
		((clsBaseMaterial*)m_materials[z])->m_map_diffuse.m_filtering_enable = f;
	}
}
//----------------------------------------------------------------------------
mmVoid clsText::SetWordWrap(mmBool w)
{
	m_wrap = w;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::UseRHW(mmBool rhw)
{
	if(rhw)
		m_ch_fvf = _rhw;
	else
		m_ch_fvf = _norhw;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetX(mmFloat X)
{
	m_x=X;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetY(mmFloat Y)
{
	m_y=Y;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetZ(mmFloat Z)
{
	m_z=Z;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetX()
{
	return m_x;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetY()
{
	return m_y;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetZ()
{
	return m_z;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetPos(mmFloat X, mmFloat Y)
{
	m_y=Y;
	m_x=X;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetPos(mmFloat X, mmFloat Y, mmFloat Z)
{
	m_y=Y;
	m_x=X;
	m_z=Z;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetPos(const clsVector3 v)
{
	m_y=v.y;
	m_x=v.x;
	m_z=v.z;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetAlignX(enAlign al)
{
	switch ( al )
	{
	case enTop:
	case enBottom:
	case enTopRect:
	case enBottomRect:
		mmThrow(enExceptObjectMissed, clsText, SetAlignX, Incorrect);
	}

	m_align_x = al;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetAlignY(enAlign al)
{
	switch ( al )
	{
	case enLeft:
	case enRight:
	case enLeftRect:
	case enRightRect:
		mmThrow(enExceptObjectMissed, clsText, SetAlignY, Incorrect);
	}
	m_align_y = al;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetText(const clsString & str)
{
	m_txt = str;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetText(const mmString str)
{
	m_txt = str;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetRect(const clsRect & Rect)
{
	if(m_rect != Rect)
		if(m_k_y != 0)
			SetYk(m_k_y);
	

	m_use_rect = mmtrue;
	m_rect = Rect;
	m_rebuild = mmtrue;

	

}
//----------------------------------------------------------------------------
mmVoid clsText::SetYk(mmFloat k)
{
	

	m_min_y = 0;
	m_max_y = max(0, GetHeight() - m_rect.Height() );
	
	m_y =  -(m_max_y-m_min_y) * k;
	m_k_y = k;	

	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetYk()
{
	return m_k_y;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetRect()
{
	m_use_rect = mmfalse;
	m_rebuild = mmtrue;

	m_min_y = 0;
	m_max_y = 0;
	m_k_y = 0;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetSize(mmFloat s)
{
	m_size = s;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmVoid clsText::SetColor(mmUint Color)
{
	m_color = Color;
	m_rebuild = mmtrue;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetWidth()
{
	if(m_rebuild)
		Build();
	return m_width;
}
//----------------------------------------------------------------------------
mmFloat clsText::GetHeight()
{
	if(m_rebuild)
		Build();
	return m_height;
}
//----------------------------------------------------------------------------
mmInt clsText::getColor(clsString & str,mmInt i, mmUint & co)
{
	clsString c = " ";
	if(str[i+1] != '(')
	{
		c[0] = str[i+1];
		c.LowerCase();
		switch ( ((mmChar)(c[0])) )
		{
		case 'r':
			co = 0xFFFF0000;
			break;
		case 'g':
			co = 0xFF00FF00;
			break;
		case 'b':
			co = 0xFF0000FF;
			break;
		case 'y':
			co = 0xFFFFFF00;
			break;
		case 'a':
			co = 0xFF00FFFF;
			break;
		case 'f':
			co = 0xFFFF00FF;
			break;
		case '0':
			co = 0xFF000000;
			break;
		case 'w':
			co = 0xFFFFFFFF;
			break;
		default:
			return -1;
		}
		return i+1;
	}
	mmInt pos;
	pos = str.FindSubString(")",i+2);
	if(pos == -1)
		return -1;
	c = str.Mid(i+2,pos - i-2);
	c.LowerCase();
	if(c == "def")
	{
		co = m_color;
	}else{
	sscanf(c.Data(),"%x",&co);
	}
	return pos;
}
//----------------------------------------------------------------------------
mmInt clsText::getAlign(clsString & str,mmInt i, enAlign & al, enAlign & def)
{

	if(str[i+1] != '(')
		return -1;
	//if((str[i+2] != 'x')&&(str[i+2] != 'y'))
	//	return -1;
	mmInt pos;
	pos = str.FindSubString(")",i+2);
	if(pos == -1)
		return -1;
	clsString c;
	c = str.Mid(i+2,pos - i-2);
	c.LowerCase();
	if(c == "def")
	{
		al = def;
	}else{
		if(c == "left")
			al = enLeft;
		else if(c == "right")
			al = enRight;
		else if(c == "top")
			//al = enTop;
			mmThrow(enExceptObjectMissed, clsText, SetAlignX, Incorrect)
		else if(c == "bottom")
			//al = enBottom;
			mmThrow(enExceptObjectMissed, clsText, SetAlignX, Incorrect)
		else if(c == "center")
			al = enCenter;
		else if(c == "fillrect")
		{
			al = enFillRect;
			m_wrap = mmtrue;
		}
		else if(c == "leftrect")
			al = enLeftRect;
		else if(c == "rightrect")
			al = enRightRect;
		else if(c == "toprect")
			//al = enTopRect;
			mmThrow(enExceptObjectMissed, clsText, SetAlignX, Incorrect)
		else if(c == "bottomrect")
			//al = enBottomRect;
			mmThrow(enExceptObjectMissed, clsText, SetAlignX, Incorrect)
		else if(c == "centerrect")
			al = enCenterRect;
	}
	return pos;
}
//----------------------------------------------------------------------------
mmVoid clsText::getInfo()
{
	mmUint co,col;
	mmInt c_pos = -1,ax_pos = -1,ay_pos = -1;
	enAlign al_x,ali_x,al_y;//,ali_y;

	al_x = m_align_x;
	al_y = m_align_y;

	co = m_color;
	m_info.Size(m_txt.Length());
	
	clsItemFont * f_i;
	clsCharFont * c_i;

	for(mmUint z=0;z<m_Count_FC.Size();z++)
		m_Count_FC[z] = 0;

	for(mmUint z=0 ; z < m_txt.Length();z++)
	{
		if((m_colored)&&(m_txt[z] == '\b'))
		{
			c_pos = getColor(m_txt, z ,col);
			if(c_pos != -1)
				co=col;
		}

		if((m_aligned)&&(m_txt[z] == '\a'))
		{
			//if(m_txt[z+2] == 'x')
			//{
				ax_pos = getAlign(m_txt, z ,ali_x, m_align_x);
				if(ax_pos != -1)
					al_x=ali_x;
			//}else{
			//	ay_pos = getAlign(m_txt, z ,ali_y, m_align_y);
			//	if(ay_pos != -1)
			//		al_y=ali_y;
			//}

		}

		if(c_pos != -1)
		{
			m_info[z].f = mmnull;
			m_info[z].c = mmnull;
			if(c_pos == z)
				c_pos = -1;
			continue;
		}

		if(ax_pos != -1)
		{
			m_info[z].f = mmnull;
			m_info[z].c = mmnull;
			if(ax_pos == z)
				ax_pos = -1;
			continue;
		}

		if(ay_pos != -1)
		{
			m_info[z].f = mmnull;
			m_info[z].c = mmnull;
			if(ay_pos == z)
				ay_pos = -1;
			continue;
		}

		if( (f_i = m_font->Find(m_txt[z],&c_i)) == mmnull)
		{
			m_info[z].f = mmnull;
			m_info[z].c = mmnull;
		}else{
			m_info[z].f = f_i;
			m_info[z].c = c_i;

			m_info[z].color = co;
			m_info[z].aligh_x = al_x;
			m_info[z].aligh_y = al_y;

			m_Count_FC[f_i->id] = m_Count_FC[f_i->id]+1;
		}
	}
}
//----------------------------------------------------------------------------
mmVoid clsText::OnRender()
{
	if(m_rebuild)
		Build();

/*
	if((m_use_rect) && ( (m_rect.Width() == 0)||(m_rect.Height()==0)   ))
	{

	}
*/
//	IDirect3DDevice9* pddev = *m_presman->GetDevice();
//	pddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
}
//----------------------------------------------------------------------------
mmVoid clsText::calculateXY(mmFloat & x, mmFloat & y, enAlign a_x, enAlign a_y)
{
	x = m_x; y = m_y;

	if(m_use_rect)
	{
		switch ( a_y )
		{
		case enTopRect:
			y = 0;
			break;
		case enBottomRect:
			y = m_rect.Height();
			break;
		case enCenterRect:
			y = m_rect.Height() * 0.5f;
			break;
		}

		switch ( a_x )
		{
		case enLeftRect:
			x = 0;
			break;
		case enRightRect:
			x = m_rect.Width();
			break;
		case enFillRect:
			x = 0;
			m_wrap = mmtrue;
			break;
		case enCenterRect:
			x = m_rect.Width() * 0.5f;
			break;
		}
	}
}
//----------------------------------------------------------------------------
mmVoid clsText::Build()
{
	if((m_use_rect) && ( (m_rect.Width() == 0)||(m_rect.Height()==0)   ))
	{
		TextRect.left = 0;
		TextRect.top = 0;
		TextRect.right = 0;
		TextRect.bottom = 0;
		return;
	}
	TextRect.left = (mmFloat)99999999;
	TextRect.top = (mmFloat)99999999;
	TextRect.right = (mmFloat)-99999999;
	TextRect.bottom = (mmFloat)-99999999;


	mmFloat t_x,t_y;
//	calculateXY(t_x,t_y,m_align_x,m_align_y);
	//======================================
	mmUint i;
	mmChar ch;
	mmInt nSpace;
	mmFloat wLastW;
	mmInt nLastW;

	m_width = 0;
	m_height = 0;

	enAlign _align_x;
	enAlign _align_y;



	m_rebuild = mmfalse;

	

	getInfo();

	m_tokens.Size(0);

	mmBool firct_c = mmtrue;

	for(mmUint z=0 ; z < m_txt.Length();z++)
	{
		_align_x = m_info[z].aligh_x;
		_align_y = m_info[z].aligh_y;

		m_tokens.Size(m_tokens.Size()+1);

		m_tokens[m_tokens.Size()-1].count_space = 0;
		m_tokens[m_tokens.Size()-1].count_words = 0;
		m_tokens[m_tokens.Size()-1].width_space = 0;
		m_tokens[m_tokens.Size()-1].width_words = 0;
		m_tokens[m_tokens.Size()-1].width_1space = 0;

		nSpace = -1;
		wLastW = 0;
		nLastW = 0;
		firct_c = mmtrue;
		for(i=z ; i < m_txt.Length();i++)
		{


			ch = m_txt[i];
			if(ch != '\n')
			{
				if(m_info[i].f == mmnull)
				{
					continue;
				}
			}else{
				break;
			}

			_align_x = m_info[i].aligh_x;
			_align_y = m_info[i].aligh_y;

			if(firct_c)
			{
				m_tokens[m_tokens.Size()-1].nChar = i;
				firct_c = mmfalse;
			}

			if(ch == ' ')
			{
				m_tokens[m_tokens.Size()-1].count_space ++;
				m_tokens[m_tokens.Size()-1].width_space += m_info[i].c->width * m_size;
				nSpace = i;
				wLastW = 0;
				nLastW = 0;
				continue;
			}	

			if((m_use_rect)&&(m_wrap))
			{
				if( (m_tokens[m_tokens.Size()-1].width_words + m_info[i].c->width * m_size + m_tokens[m_tokens.Size()-1].width_space) > m_rect.Width() )
				{
					if(nSpace != -1)
					{
						i = nSpace+1;
						m_tokens[m_tokens.Size()-1].width_words -= wLastW;
						m_tokens[m_tokens.Size()-1].count_words -= nLastW;
						if(_align_x == enFillRect)
						{
							if(m_tokens[m_tokens.Size()-1].count_space != 1)
								m_tokens[m_tokens.Size()-1].width_1space = (m_rect.Width() - m_tokens[m_tokens.Size()-1].width_words) / (m_tokens[m_tokens.Size()-1].count_space - 1);
						}
					}
					i--;
					break;
				}
			}

			m_tokens[m_tokens.Size()-1].width_words += m_info[i].c->width * m_size;
			m_tokens[m_tokens.Size()-1].count_words ++;
			wLastW += m_info[i].c->width * m_size;
			nLastW ++;
		}
		if(_align_x == enFillRect)
		{
			if(m_tokens[m_tokens.Size()-1].width_1space == 0)
				if(m_tokens[m_tokens.Size()-1].count_space != 0)
					m_tokens[m_tokens.Size()-1].width_1space = m_tokens[m_tokens.Size()-1].width_space / m_tokens[m_tokens.Size()-1].count_space;
		}
		z=i;
	}

	for(i=0;i<m_tokens.Size();i++)
	{
		if((m_tokens[i].count_words + m_tokens[i].count_space ) != 0)
		{
			_align_x = m_info[m_tokens[i].nChar].aligh_x;
			_align_y = m_info[m_tokens[i].nChar].aligh_y;

			calculateXY(t_x,t_y,_align_x,_align_y);

			if((_align_y == enBottom)||(_align_y == enBottomRect))
				m_tokens[i].y = t_y - m_info[m_tokens[i].nChar].f->m_Height * (m_tokens.Size()-i) * m_size;
			else if((_align_y == enCenter)||(_align_y == enCenterRect))
				m_tokens[i].y = t_y + m_info[m_tokens[i].nChar].f->m_Height * i * m_size - (m_info[m_tokens[i].nChar].f->m_Height * m_tokens.Size() * m_size * 0.5f);
			else
				m_tokens[i].y = t_y + m_info[m_tokens[i].nChar].f->m_Height * i * m_size;

			if((_align_x == enRight)||(_align_x == enRightRect))
				m_tokens[i].x = t_x - (m_tokens[i].width_space+m_tokens[i].width_words);
			else if((_align_x == enCenter)||(_align_x == enCenterRect))
				m_tokens[i].x = t_x - (m_tokens[i].width_space+m_tokens[i].width_words)*0.5f;
			else
				m_tokens[i].x = t_x;
		}
	}
	//========================================================
	mmUint coun = 0;
	mmUint c_v = 0;
	mmUint c_i = 0;
	for(mmUint z=0;z<m_font->items.Count();z++)
	{
		m_psubsets[z].m_count = m_Count_FC[z]*2;
		m_psubsets[z].m_start = coun;
		m_psubsets[z].m_mtlindex = z;
		coun += m_psubsets[z].m_count*3;

		c_i += m_psubsets[z].m_count*3;
		c_v += m_Count_FC[z]*4;
	}

	if(m_basevbuf->GetNumVertices() < c_v)
		m_basevbuf->Create(c_v,m_ch_fvf);
	if(m_baseibuf->GetNumIndicies() < c_i)
		m_baseibuf->Create(c_i);

	mmUint ii;
	mmFloat _x,_y;
	
	for(mmUint z=0;z<m_Pos_V.Size();z++)
	{
		if(z==0)
			m_Pos_V[z] = 0;
		else
		{
			m_Pos_V[z] = (mmInt)((mmFloat)m_psubsets[z-1].m_count*0.5f);
		}
	}

	mmFloat* vb;
	mmWord* ib;
	m_basevbuf->Lock((mmVoid**)&vb);
	m_baseibuf->Lock((mmVoid**)&ib);
	mmUint zi = 0;
	mmUint cc = 0;
	mmInt sizV;
	mmFloat isSp,_w;
	for(mmUint z=0 ; z < m_txt.Length();z++)
	{
		ii=z;
		if(zi >= m_tokens.Size())
			break;
		_x = m_tokens[zi].x;
		_y = m_tokens[zi].y;
		cc = (mmUint)(m_tokens[zi].count_space + m_tokens[zi].count_words) ;

		_w=0.0f;
		for(i=0 ; i < cc;i++)
		{		
			if(m_info[z+i].f == mmnull)
			{
				cc++;
				ii++;
				continue;
			}
			_w += m_info[z+i].c->width*m_size;

			_align_x = m_info[z+i].aligh_x;
			_align_y = m_info[z+i].aligh_y;

			if(m_ch_fvf == _rhw)
				sizV = 7;
			else
				sizV = 9;
			if((m_txt[z+i] == ' ')&&(m_use_rect)&&(m_wrap)&&(_align_x == enFillRect))
				isSp = 1;
			else
				isSp = 0;
			

			m_Pos_V[m_info[z+i].f->id] +=	BuildChar(	_x,_y,z+i,
														vb+m_Pos_V[m_info[z+i].f->id] * 4 * sizV ,
														ib+m_Pos_V[m_info[z+i].f->id] * 6, 
														m_Pos_V[m_info[z+i].f->id],
														m_tokens[zi].width_1space * isSp);


			if(isSp == 0)
				_x += m_info[z+i].c->width*m_size ;
			else
				_x += m_tokens[zi].width_1space;
				
			

		//	m_Pos_V[m_info[z+i].f->id] ++;

			ii++;
			
		}
		zi++;
		z=ii-1;


		m_width = max(m_width,_w);
		m_height += m_info[z].f->m_Height*m_size;
	}
	m_basevbuf->Unlock();
	m_baseibuf->Unlock();

	for(mmUint z=0;z<m_font->items.Count();z++)
	{
		m_psubsets[z].m_count = m_Pos_V[z]*2;
	}


}
//----------------------------------------------------------------------------
mmInt clsText::BuildChar(mmFloat X, mmFloat Y, mmUint i, mmFloat* vb,mmWord* ib, mmInt nc, mmFloat wSp)
{
	clsVector2 p1,p2,u1,u2;

//	m_width = 0;
//	m_height = 0;
	
	//m_width += m_info[i].c->width*m_size;
	//m_height += m_info[i].f->m_Height*m_size;

	if(m_use_rect)
	{
		X += m_rect.left;
		Y += m_rect.top;
	}

	clsCharInfo* pInfo = &m_info[i];
	
	//clsCharFont* pFont = m_info[i].c;
	
	p1.x =	X - m_info[i].f->m_x*m_size ;
	p1.y =	Y - m_info[i].f->m_y*m_size ;
//	p1.z =	m_z;
	u1.x = m_info[i].c->x1;
	u1.y = m_info[i].c->y1;

//	p2.x =	p1.x + (m_info[i].c->x2-m_info[i].c->x1) * m_size;
	p2.y =	p1.y + m_info[i].f->m_HeightUV*m_size;
//	p2.z =	m_z;
	u2.x = m_info[i].c->x2;
	u2.y = m_info[i].c->y2;

	if(wSp != 0)
	{
		p2.x = p1.x + wSp;
	}else{
		p2.x = p1.x + (m_info[i].c->x2-m_info[i].c->x1)*m_size;
	}

	if(m_use_rect)
	{
		if( (m_rect.right < p1.x)||(m_rect.left > p2.x)||(m_rect.top > p2.y)||(m_rect.bottom < p1.y))
		{
			return 0;
		}

		mmFloat k;

		if( m_rect.right < p2.x )
		{
			k = p2.x - p1.x;
			p2.x = m_rect.right;
			k = (p2.x - p1.x)/k;
			u2.x = u1.x + (u2.x - u1.x)*k;
		}

		if( m_rect.left > p1.x )
		{
			k = p2.x - p1.x;
			p1.x = m_rect.left;
			k = (p2.x - p1.x)/k;
			u1.x = u2.x - (u2.x - u1.x)*k;
		}

		if( m_rect.bottom < p2.y )
		{
			k = p2.y - p1.y;
			p2.y = m_rect.bottom;
			k = (p2.y - p1.y)/k;
			u2.y = u1.y + (u2.y - u1.y)*k;
		}

		if( m_rect.top > p1.y )
		{
			k = p2.y - p1.y;
			p1.y = m_rect.top;
			k = (p2.y - p1.y)/k;
			u1.y = u2.y - (u2.y - u1.y)*k;
		}


	}

	TextRect.left = min(TextRect.left,X);
	TextRect.top = min(TextRect.top,Y);
	TextRect.right = max(TextRect.right, X + m_info[i].c->width*m_size);
	TextRect.bottom = max(TextRect.bottom,Y + m_info[i].f->m_Height*m_size);
	
		

	if(m_ch_fvf == _rhw)
	{

		vb[0]= p1.x -0.5f;
		vb[1]= p1.y -0.5f;
		vb[2]= m_z;
		vb[3] = 1.0f;
		*((mmUint*)(&vb[4])) = m_info[i].color;
		vb[5]= u1.x;
		vb[6]= u1.y;

		vb[7]= vb[0];
		vb[8]= p2.y -0.5f;
		vb[9]= m_z;
		vb[10] = 1.0f;
		*((mmUint*)(&vb[11])) = m_info[i].color;
		vb[12]= vb[5];
		vb[13]= u2.y;

		vb[14]= p2.x -0.5f;
		vb[15]= vb[1];
		vb[16]= m_z;
		vb[17] = 1.0f;
		*((mmUint*)(&vb[18])) = m_info[i].color;
		vb[19]= u2.x;
		vb[20]= vb[6];

		vb[21]= vb[14];
		vb[22]= vb[8];
		vb[23]= m_z;
		vb[24] = 1.0f;
		*((mmUint*)(&vb[25])) = m_info[i].color;
		vb[26]= vb[19];
		vb[27]= vb[13];

	}else{

		vb[0]= p1.x ;
		vb[1]= p1.y ;
		vb[2]= m_z;
		vb[3]= 0.0f;
		vb[4]= 0.0f;
		vb[5]= 1.0f;
		*((mmUint*)(&vb[6])) = m_info[i].color;
		vb[7]= u1.x;
		vb[8]= u1.y;

		vb[9]= vb[0];
		vb[10]= p2.y ;
		vb[11]= m_z;
		vb[12]= 0.0f;
		vb[13]= 0.0f;
		vb[14]= 1.0f;
		*((mmUint*)(&vb[15])) = m_info[i].color;
		vb[16]= vb[7];
		vb[17]= u2.y;

		vb[18]= p2.x ;
		vb[19]= vb[1];
		vb[20]= m_z;
		vb[21]= 0.0f;
		vb[22]= 0.0f;
		vb[23]= 1.0f;
		*((mmUint*)(&vb[24])) = m_info[i].color;
		vb[25]= u2.x;
		vb[26]= vb[8];

		vb[27]= vb[18];
		vb[28]= vb[10];
		vb[29]= m_z;
		vb[30]= 0.0f;
		vb[31]= 0.0f;
		vb[32]= 1.0f;
		*((mmUint*)(&vb[33])) = m_info[i].color;
		vb[34]= vb[25];
		vb[35]= vb[17];


	}


		mmInt in;
		in = nc*4;

		ib[0] = in+1;
		ib[1] = in+0;
		ib[2] = in+2;
		ib[3] = in+2;
		ib[4] = in+3;
		ib[5] = in+1;

	
		return 1;
}
//----------------------------------------------------------------------------

mmVoid clsText::UpdateEffect(mmFloat k, mmBool p_reset)
{
	
	

	if(p_reset == mmtrue)
	{
		inter.SetT(0.0f);
		ok = mmtrue;
	}

	if(ok == mmfalse)
		return;

	if( (inter.GetT()+k) > 1.0f )
		ok = mmfalse;


	Build();

	mmFloat* vb;
	m_basevbuf->Lock((mmVoid**)&vb);

	clsVector3 c;
	clsVector3 v1;
	clsVector3 v2;
	clsVector3 v3;
	clsVector3 v4;

	
	inter.v1 = 1.5f;
	inter.v2 = 1;

	mmFloat siz;


	inter.Looping = enHold;
	inter.Interpolation = enInterDoun;

	inter.SetDeltaTime(k);

	siz = inter.v;

	

	for(mmUint i = 0; i < m_txt.Length(); i++)
	{
		v1.x = vb[0];
		v1.y = vb[1];
		v1.z = vb[2];

		v2.x = vb[7];
		v2.y = vb[8];
		v2.z = vb[9];

		v3.x = vb[14];
		v3.y = vb[15];
		v3.z = vb[16];

		v4.x = vb[21];
		v4.y = vb[22];
		v4.z = vb[23];

		c = (v1 + v2 + v3 + v4 ) * 0.25f;

		v1 = (v1-c) * siz + c;
		v2 = (v2-c) * siz + c;
		v3 = (v3-c) * siz + c;
		v4 = (v4-c) * siz + c;


		//-

		vb[0] = v1.x;
		vb[1] = v1.y;
		vb[2] = v1.z;

		vb[7] = v2.x;
		vb[8] = v2.y;
		vb[9] = v2.z;

		vb[14] = v3.x;
		vb[15] = v3.y;
		vb[16] = v3.z;

		vb[21] = v4.x;
		vb[22] = v4.y;
		vb[23] = v4.z;

		vb += 28;//7*4


	}

	m_basevbuf->Unlock();

}