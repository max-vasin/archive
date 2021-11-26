//==========================================================================================
// Mayhem engine 3DS Max tools - material exportert class implementation
// Coded:	MozG
// Created:	2 june 2006
//==========================================================================================
#include "mhmtool.h"

// Material exporter constructor
//==========================================================================================
clsMaterialExporter::clsMaterialExporter()
{
	m_boverw = mmfalse;
	m_asanim = mmfalse;
}

// Material exporter desstructor
//==========================================================================================
clsMaterialExporter::~clsMaterialExporter()
{
	m_boverw = mmfalse;
	m_asanim = mmfalse;
}


// SetRootPath: sets exporter root path
//==========================================================================================
mmRes clsMaterialExporter::SetRootPath(clsString root)
{
	m_root = root;
	return mmOk;
}

// SetTexturesRoot: sets textures root-offset path
//==========================================================================================
mmRes clsMaterialExporter::SetTexturesRoot(clsString root)
{
	m_texroot = root;
	return mmOk;
}

// AllowOwerwrite: allows exporter to owerwrite existing files
//==========================================================================================
mmRes clsMaterialExporter::AllowOverwrite(mmBool allow)
{
	m_boverw = allow;
	return mmOk;
}

// UVOffsetAsAnimation: switch UV map offset meaning
//==========================================================================================
mmRes clsMaterialExporter::UVOffsetAsAnimation(mmBool allow)
{
	m_asanim = allow;
	return mmOk;
}



// ExportMaterial: exports material
//==========================================================================================
mmRes clsMaterialExporter::ExportMaterial(Mtl *pmtl)
{
	if (pmtl == mmnull)
		return mmFail;
	
	clsString ext = GetString(IDS_MATERIAL_EXT);
	clsString fullname = m_root + pmtl->GetName() + "." + ext;
	clsFile file(fullname);
	if (file.IsExist() && !m_boverw)
		return mmOk;
	
	if (MHMFAIL(file.Create()))
		return mmFail;
	
	if (MHMFAIL(file.Open()))
		return mmFail;
	
//	stMaterialExp expmtl;
	
	WriteParam(file, "Opacity",       1.0f - pmtl->GetXParency());
	WriteParam(file, "Shininess",     pmtl->GetShininess());
	WriteParam(file, "Strength",	  pmtl->GetShinStr());
	WriteParam(file, "DiffuseColor",  pmtl->GetDiffuse());
	WriteParam(file, "AmbientColor",  pmtl->GetAmbient());
	WriteParam(file, "SpecularColor", pmtl->GetSpecular());
	WriteParam(file, "EmissionColor", pmtl->GetSelfIllumColor());
	StdMat* std = (StdMat*)pmtl;
	if (std->GetTwoSided() == TRUE)
		WriteParam(file, "TwoSided", "true", mmtrue);
	else
		WriteParam(file, "TwoSided", "false", mmtrue);
	
	if (std->GetWire() == TRUE)
		WriteParam(file, "Wireframe", "true", mmtrue);
	else
		WriteParam(file, "Wireframe", "false", mmtrue);
	
	
	switch(std->GetTransparencyType())
	{
		case TRANSP_FILTER:
			WriteParam(file, "TransparencyType", "Filter");
			break;
		case TRANSP_SUBTRACTIVE:
			WriteParam(file, "TransparencyType", "Subtractive");
			break;
		case TRANSP_ADDITIVE:
			WriteParam(file, "TransparencyType", "Additive");
			break;
	}
	
	for (mmInt i = 0; i < pmtl->NumSubTexmaps(); i++)
	{
		if (pmtl->SubTexmapOn(i))
		{
			Texmap* pmap = pmtl->GetSubTexmap(i);
			clsString maptype;
			switch(pmtl->MapSlotType(i))
			{
			case MAPSLOT_ENVIRON:
				maptype = "Environment";
				break;
			case MAPSLOT_TEXTURE:
				maptype = "Explicit";
				break;
			}	
			
			if (pmap != mmnull)
			{
				if (pmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0) )
				{
					clsString slotname = pmtl->GetSubTexmapSlotName(i).data();	
					slotname = GetMapName(slotname);
					BitmapTex* pbmp = (BitmapTex*)pmap;
					clsString alphasource;
					switch(pbmp->GetAlphaSource())
					{
					case ALPHA_FILE:
						alphasource = "Texture";
						break;
					case ALPHA_RGB:
						alphasource = "RGB";
						break;
					case ALPHA_NONE:
						alphasource = "None";
						break;
					}
					
					ICustAttribContainer* cc = pbmp->GetCustAttribContainer();
					CustAttrib* ca = mmnull;
					mmBool ok = mmfalse;
					if (cc)
						for(int i=0; i < cc->GetNumCustAttribs();i++)
						{
							ca = cc->GetCustAttrib(i);
							if(ca->ClassID() == MHM_MAP_CUSTOM_ATTRIB_CLASS_ID)
							{
								ok = mmtrue;
								break;
							}
						}
					
					IParamBlock2* pblock = mmnull;
					if (ok)
					{
						pblock = ca->GetParamBlock(0);
						if (!pblock)
							ok = !ok;	
					}
					
					//pblock->
					
					clsString mapname  = RootRelative(pbmp->GetMapName());
					
					Interval valid_interval;
					
					mmFloat map_speed_u			= 0.0f;
					mmFloat map_speed_v			= 0.0f;
					mmFloat map_speed_r			= 0.0f;
					mmInt	map_miplevel		= 0;
					mmInt	map_colorkey_enable = 0;
					AColor	map_colorkey_low(0, 0, 0, 0);
					AColor  map_colorkey_hi(0, 0, 0, 0);
					mmInt	map_colorkey_alpha	= 0;
					mmInt	map_alphatest_value	= 0;
					mmInt	map_alphablend_enable = 0;
					mmInt	map_additive_enable = 0;
					
					
					
					if (ok)
					{
						for (mmInt z = 0; z < pblock->NumParams(); z++)
						{
							mmInt     type = pblock->GetParameterType(z);
							clsString name = (mmString)pblock->GetLocalName(z);
							if		(name == "map_speed_u")
							{
								pblock->GetValue(z, 0, map_speed_u, valid_interval); 
							}
							else if (name == "map_speed_v")
							{
								pblock->GetValue(z, 0, map_speed_v, valid_interval); 
							}
							else if (name == "map_speed_r")
							{
								pblock->GetValue(z, 0, map_speed_r, valid_interval); 
							}
							else if (name == "map_mipmaps")
							{
								pblock->GetValue(z, 0, map_miplevel, valid_interval); 
							}
							else if (name == "map_colorkey_enable")
							{
								pblock->GetValue(z, 0, map_colorkey_enable, valid_interval); 
							}
							else if (name == "map_colorkey_low")
							{
								pblock->GetValue(z, 0, map_colorkey_low, valid_interval); 
							}
							else if (name == "map_colorkey_hi")
							{
								pblock->GetValue(z, 0, map_colorkey_hi, valid_interval); 
							}
							else if (name == "map_colorkey_alpha")
							{
								pblock->GetValue(z, 0, map_colorkey_alpha, valid_interval); 
							}
							else if (name == "map_alphatest_value")
							{
								pblock->GetValue(z, 0, map_alphatest_value, valid_interval); 
							}
							else if (name == "map_alphablend_enable")
							{	
								pblock->GetValue(z, 0, map_alphablend_enable, valid_interval); 
							}
							else if (name == "map_additive_enable")
							{	
								pblock->GetValue(z, 0, map_additive_enable, valid_interval); 
							}
						}
					}
#pragma warning(disable:4800)
					ArrayOpen(file, slotname);
					ArrayAdd(file, mapname,		 mmfalse);
					ArrayAdd(file, map_speed_u,	 mmfalse);
					ArrayAdd(file, map_speed_v,	 mmfalse);
					ArrayAdd(file, map_speed_r,	 mmfalse);
					ArrayAdd(file, map_miplevel, mmfalse);
					ArrayAdd(file, (mmBool)map_colorkey_enable,	 mmfalse);
					ArrayAdd(file, map_colorkey_low.r,	  mmfalse);
					ArrayAdd(file, map_colorkey_low.g,	  mmfalse);
					ArrayAdd(file, map_colorkey_low.b,	  mmfalse);
					ArrayAdd(file, map_colorkey_hi.r,	  mmfalse);
					ArrayAdd(file, map_colorkey_hi.g,	  mmfalse);
					ArrayAdd(file, map_colorkey_hi.b,	  mmfalse);
					ArrayAdd(file, map_colorkey_alpha,	  mmfalse);
					ArrayAdd(file, map_alphatest_value,	  mmfalse);
					ArrayAdd(file, (mmBool)map_alphablend_enable, mmfalse);
					ArrayAdd(file, alphasource, mmfalse);
					ArrayAdd(file, (mmBool)map_additive_enable, mmtrue);
					ArrayClose(file);
#pragma warning(default:4800)
				}
			}
		}
	}
	return mmOk;
}

// WriteParam: writes param to file
//==========================================================================================
mmRes clsMaterialExporter::WriteParam(clsFile &file, clsString name, Color val)
{
	clsString flt;
	clsString fullout = name + "={";
	flt.FromFloat(val.r);
	fullout += flt + ":";
	flt.FromFloat(val.g);
	fullout += flt + ":";
	flt.FromFloat(val.b);
	fullout += flt + "}\n";
	return file.Write(fullout);
}

// WriteParam: writes float param to file
//==========================================================================================
mmRes clsMaterialExporter::WriteParam(clsFile &file, clsString name, mmFloat val)
{
	clsString flt;
	clsString fullout = name + "=";
	flt.FromFloat(val);
	fullout += flt + "\n";
	return file.Write(fullout);
}

// WriteParam: writes string param to file
//==========================================================================================
mmRes clsMaterialExporter::WriteParam(clsFile &file, clsString name, clsString val, mmBool bNoSemicolons)
{
	clsString fullout;
	if (bNoSemicolons)
		fullout = name + "=" + val + "\n";
	else
		fullout = name + "=\"" + val + "\"\n";
	return file.Write(fullout);
}

// ArrayOpen: opens array parameters
//==========================================================================================
mmRes clsMaterialExporter::ArrayOpen(clsFile& file, clsString name)
{
	return file.Write(name + "={");
}

// ArrayAdd: adds new item to array
//==========================================================================================
mmRes clsMaterialExporter::ArrayAdd(clsFile &file, clsString val, mmBool last)
{
	clsString out = "\"" + val + "\"";
	if (!last)
		out += ":";
	return file.Write(out);
}	

// ArrayAdd: adds new item to array
//==========================================================================================
mmRes clsMaterialExporter::ArrayAdd(clsFile &file, mmBool val, mmBool last)
{
	clsString out;
	if (val)
		out = "true";
	else
		out = "false";
	if (!last)
		out += ":";
	return file.Write(out);
}	

// ArrayAdd: adds new item to array
//==========================================================================================
mmRes clsMaterialExporter::ArrayAdd(clsFile &file, mmFloat val, mmBool last)
{
	clsString out;
	out.FromFloat(val);
	if (!last)
		out += ":";
	return file.Write(out);
}	

// ArrayAdd: adds new item to array
//==========================================================================================
mmRes clsMaterialExporter::ArrayAdd(clsFile &file, mmInt val, mmBool last)
{
	clsString out(val);
	if (!last)
		out += ":";
	return file.Write(out);
}	


// ArrayAdd: adds new item to array
//==========================================================================================
mmRes clsMaterialExporter::ArrayClose(clsFile& file)
{
	return file.Write(clsString("}\n"));
}

// GetMapName: returns map name by slot name
//==========================================================================================
clsString clsMaterialExporter::GetMapName(clsString slotname)
{
	clsString mapname = "UnknownMap";
	if		(slotname == "Ambient Color")
		mapname = "AmbientMap";
	else if	(slotname == "Diffuse Color")
		mapname = "DiffuseMap";
	else if (slotname == "Specular Color")
		mapname = "SpecularMap";
	else if (slotname == "Specular Level")
		mapname = "SpecularLevelMap";
	else if (slotname == "Glossiness")
		mapname = "GlossinesMap";
	else if (slotname == "Self-Illumination")
		mapname = "EmissionMap";
	else if (slotname == "Opacity")
		mapname = "OpacityMap";
	else if (slotname == "Filter Color")
		mapname = "FilterMap";
	else if (slotname == "Bump")
		mapname = "BumpMap";
	else if (slotname == "Reflection")
		mapname = "ReflectionMap";
	else if (slotname == "Refraction")
		mapname = "RefractionMap";
	else if (slotname == "Displacement")
		mapname = "DisplacementMap";
	return mapname;
}

// RootRelative: returns texture-root relative path
//==========================================================================================
clsString clsMaterialExporter::RootRelative(clsString path)
{
	if (m_texroot.IsEmpty())
		return path;
	
	if (m_texroot.Length() >= path.Length())
		return path;
	mmInt pos = path.FindSubString(m_texroot);
	if (pos == -1)
		return path;
	
	return path.Right(path.Length() - m_texroot.Length() - pos);
}


