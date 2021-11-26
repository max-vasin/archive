//==========================================================================================
// Mayhem engine 3DS Max tools - mesh exportert class implementation
// Coded:	MozG
// Created:	7 june 2006
//==========================================================================================
#include "mhmtool.h"


// Mesh exporter constructor
//==========================================================================================
clsMeshExporter::clsMeshExporter()
{
	m_channel_normals = mmfalse;
	m_channel_mapping = mmfalse;
	m_channel_vcolors = mmfalse;
	m_bpacked         = mmfalse;
}

// Mesh exporter destructor
//==========================================================================================
clsMeshExporter::~clsMeshExporter()
{

}

// SetRoot: sets meshes root
//==========================================================================================
mmVoid clsMeshExporter::SetRoot(clsString root)
{
	m_root = root;
}

// EnableChannels: enables mesh channels export
//==========================================================================================
mmVoid clsMeshExporter::EnableChannels(mmBool normals, mmBool mapping, mmBool vcolors)
{
	m_channel_normals = normals;
	m_channel_mapping = mapping;
	m_channel_vcolors = vcolors;
}

// EnablePacked: enables mesh pack
//==========================================================================================
mmVoid clsMeshExporter::EnablePacked(mmBool packed)
{
	m_bpacked = packed;
}

// ExportMesh: exports mesh from node
//==========================================================================================
mmRes clsMeshExporter::ExportMesh(INode *node)
{
	TriObject* triobj = mmnull;
	mmBool     deltri = mmfalse;	
	ObjectState os = node->EvalWorldState(0);
	if (os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
		return mmFail;
	if (os.obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		triobj = (TriObject*)os.obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
		deltri = triobj != os.obj;
	}
	else
		return mmFail;
	
	Mesh* mesh = &triobj->mesh;
	if (mesh == mmnull)
		return mmFail;
	
	// Check materials
	Mtl* mtl = node->GetMtl();
	mmUint nummtls;
	if (mtl != mmnull)
	{
		
		if (mtl->IsMultiMtl() != TRUE)
			nummtls = 1;
		else
			nummtls = mtl->NumSubMtls();
	}
	else
	{
		nummtls = 0;
	}
	
	// Check channels
	if (m_channel_normals)
		mesh->buildNormals();
		
	if ((mesh->numTVerts == 0) && (m_channel_mapping))
	{
		MessageBox(NULL, "Unmapped object", "Export fail", MB_OK);
		return mmFail;
	}
	
	// buffers
	expSafeArPtr<stMeshExpVertex>	buf_points;
	expSafeArPtr<stMeshExpVertex>	buf_normals;
	expSafeArPtr<stMeshExpUV>		buf_mapping;
	expSafeArPtr<stMeshExpFace16>	buf_faces;
	
	// buffers alloc
	buf_points = new stMeshExpVertex[mesh->numFaces * 3];
	buf_faces  = new stMeshExpFace16[mesh->numFaces];
	if (m_channel_normals)
		buf_normals = new stMeshExpVertex[mesh->numFaces * 3];
	if (m_channel_mapping)
		buf_mapping = new stMeshExpUV[mesh->numFaces * 3];
	
	// Get MAX object transform
	Matrix3	tf;
	tf = node->GetObjectTM(0);
		
	mmDword collapsed;
	if (MHMFAIL(CollapseMesh(mesh, buf_points,
								   buf_normals,
								   buf_mapping,
								   buf_faces,
								   &tf,
								   &collapsed)))
	{
		MessageBox(NULL, "MAX mesh collapsing error", "Export failed", MB_OK);
		return mmFail;
	}
	
	// File structures prepare
	stMeshFileHeader meshhdr;
	
	meshhdr.m_numfaces     = mesh->numFaces;
	meshhdr.m_numverts     = collapsed;
	meshhdr.m_chnl_mapping = m_channel_mapping;
	meshhdr.m_chnl_normals = m_channel_normals;
	
	// Subsets building
	expSubsetList subsets;
	
	if (mtl != mmnull)
	{
		if (mtl->IsMultiMtl() == TRUE)
		{
			BuildSubsets(mesh, buf_faces, nummtls, &subsets);
	
			mmUint nonempty = 0;
			for (mmUint i = 0; i < subsets.Count(); i++)
				if (subsets[i]->Count() > 0)
					nonempty++;
			meshhdr.m_numsubs = nonempty;
		}
		else
		{
			meshhdr.m_numsubs = 1;
		}	
	}
	else
	{
		meshhdr.m_numsubs = 1;
	}
	
	clsFile file(m_root + node->GetName() + "." + GetString(IDS_MESH_EXT));
	
	// Streaming to file
	if (MHMFAIL(file.Create()))
		return mmFail;
		
	if (MHMFAIL(file.Open()))
		return mmFail;
		
	// Header
	if (MHMFAIL(file.Write(&meshhdr, sizeof(meshhdr))))
		return mmFail;
	
	// Material list
	if (mtl != mmnull)
	{
		if (mtl->IsMultiMtl() == TRUE)
		{
			for (mmUint i = 0; i < mtl->NumSubMtls(); i++)
				if (subsets[i]->Count() > 0)
				{
					clsString name = mtl->GetSubMtl(i)->GetName();
					if (MHMFAIL(file.Write(name, mmtrue)))
						return mmFail;
				}
		}
		else
		{
			clsString name = mtl->GetName();
			if (MHMFAIL(file.Write(name, mmtrue)))
				return mmFail;
		}
	}
	
	// Channels
	for (mmUint i = 0; i < meshhdr.m_numverts; i++)
	{
		file.Write(&buf_points[i], sizeof(stMeshExpVertex));
		if (buf_normals != mmnull)
			file.Write(&buf_normals[i], sizeof(stMeshExpVertex));
		if (buf_mapping != mmnull)
			file.Write(&buf_mapping[i], sizeof(stMeshExpUV));
	}
	
	stMeshSubset meshsub;
	if (meshhdr.m_numsubs < 2)
	{
		meshsub.m_numfaces = meshhdr.m_numfaces;
		meshsub.m_mtlindex = 0;	
		file.Write(&meshsub, sizeof(meshsub));
		file.Write(buf_faces, meshhdr.m_numfaces * sizeof(stMeshExpFace16));
	}
	else
	{
		for (i = 0; i < subsets.Count(); i++)
		{
			if (subsets[i]->Count() == 0)
				continue;
			meshsub.m_numfaces = subsets[i]->Count();
			meshsub.m_mtlindex = i;
			file.Write(&meshsub, sizeof(meshsub));
			for (mmUint fi = 0; fi < subsets[i]->Count(); fi++)
			{
				mmDword index = (*subsets[i])[fi];
				file.Write(&buf_faces[index], sizeof(stMeshExpFace16));
			}
		}
	}
	file.Close();
	if (deltri)
		mmSafeDelete(triobj);
	return mmOk;
}

// CollapseMesh: collapsing mesh by selected channels
//==========================================================================================
mmRes clsMeshExporter::CollapseMesh(Mesh *pmesh, stMeshExpVertex *ppos, stMeshExpVertex *pnorm,
								    stMeshExpUV *ptc, stMeshExpFace16 *pfaces,
								    Matrix3 *ptform, mmDword *collapsed)
{
	mmUint collapse = 0;
	UVVert		     tv;
	Point3			vtx, norm;
	Matrix3 onlyrot;
	if (ptform)
	{
		onlyrot = *ptform;
		onlyrot.NoTrans();
	}
	
	for (mmUint faceind = 0; faceind < pmesh->numFaces; faceind++)
	{
		Face   face   = pmesh->faces[faceind];
		TVFace tvface = pmesh->mapFaces(1)[faceind];
		//TVFace tvface = pmesh->tvFace[faceind];
		for (mmUint vertind = 0; vertind < 3; vertind++)
		{
			vtx     = pmesh->verts[face.v[2 - vertind]];
			if (ptc != mmnull)
				tv = pmesh->mapVerts(1)[tvface.t[2- vertind]];
				//tv = pmesh->tVerts[tvface.t[vertind]];
			if (pnorm != mmnull)
				norm = GetVertexNormal(pmesh, faceind, pmesh->getRVertPtr(face.v[2 - vertind]));
			if (ptform != mmnull)
			{
				ptform->TransformPoints(&vtx, 1);
				onlyrot.TransformPoints(&norm, 1);
				norm.Normalize();
			}
			mmBool duplicate = mmfalse;
			for (mmUint i = 0; i < collapse; i++)
			{
				duplicate = mmtrue;
				if (ppos != mmnull)
					if ((ppos[i].x != vtx.x) ||
						(ppos[i].y != vtx.y) ||
						(ppos[i].z != vtx.z))
						duplicate = mmfalse;
				if (pnorm != mmnull)
					if ((pnorm[i].x != norm.x) ||
						(pnorm[i].y != norm.y) ||
						(pnorm[i].z != norm.z))
						duplicate = mmfalse;
				if (ptc != mmnull)
					if ((ptc[i].u != tv.x) ||
						(ptc[i].v != 1.0f-tv.y))
						duplicate = mmfalse;
				
				if (duplicate)
					break;
			}
			if (duplicate)
				pfaces[faceind].f.verts[vertind] = i;
			else
			{
				pfaces[faceind].f.verts[vertind] = collapse;
				if (ppos != mmnull)
				{
					ppos[collapse].x = vtx.x;
					ppos[collapse].y = vtx.y;
					ppos[collapse].z = vtx.z;
				}
				if (pnorm != mmnull)
				{
					pnorm[collapse].x = norm.x;
					pnorm[collapse].y = norm.y;
					pnorm[collapse].z = norm.z;
				}
				if (ptc != mmnull)
				{
					ptc[collapse].u = tv.x;
					ptc[collapse].v = 1.0f-tv.y;
				}
				collapse++;
			}
		}
	}
	if (collapsed != mmnull)
		*collapsed = collapse;
	return mmOk;
}

// GetVertexNormal: returns vertex normal
//==========================================================================================
Point3 clsMeshExporter::GetVertexNormal(Mesh *pmesh, mmUint face, RVertex *prv)
{
	Point3  vnorm;
	int		numnorm;
	mmDword smgroup = pmesh->faces[face].smGroup;
	
	if (prv->rFlags & SPECIFIED_NORMAL) 
	{
		vnorm = prv->rn.getNormal();
	}
	else if ((numnorm = prv->rFlags & NORCT_MASK) && smgroup) 
	{
		if (numnorm == 1) 
		{
			vnorm = prv->rn.getNormal();
		}
		else 
		{
			for (int i = 0; i < numnorm; i++) 
			{
				if (prv->ern[i].getSmGroup() & smgroup) 
				{
					vnorm = prv->ern[i].getNormal();
				}
			}
		}
	}
	else 
	{
		vnorm = pmesh->getFaceNormal(face);
	}
	return vnorm;
}

// BuildSubsets: builds material subsets
//==========================================================================================
mmRes clsMeshExporter::BuildSubsets(Mesh *pmesh, 
									stMeshExpFace16 *pfaces,
									mmDword nummtls,
									expSubsetList *psublist)
{
	for (mmDword mtlind = 0; mtlind < nummtls; mtlind++)
	{
		psublist->Add(new tmpList<mmDword>);
		for (mmDword faceind = 0; faceind < pmesh->numFaces; faceind++)
		{
			mmDword matid = pmesh->faces[faceind].getMatID();
			if ((mtlind == matid) || ((mtlind == 0) && (matid >= nummtls)))
				(*psublist)[mtlind]->Add(faceind);
		}
	}
	return mmOk;
}

// ExportPackedMesh: exports demomaking packed mesh
//==========================================================================================
mmRes clsMeshExporter::ExportPackedMesh(INode *node)
{
	TriObject* triobj = mmnull;
	mmBool     deltri = mmfalse;	
	ObjectState os = node->EvalWorldState(0);
	if (os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
		return mmFail;
	if (os.obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		triobj = (TriObject*)os.obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
		deltri = triobj != os.obj;
	}
	else
		return mmFail;
	
	Mesh* mesh = &triobj->mesh;
	if (mesh == mmnull)
		return mmFail;
	
	// Check channels
	if (m_channel_normals)
		mesh->buildNormals();
		
	if ((mesh->numTVerts == 0) && (m_channel_mapping))
	{
		MessageBox(NULL, "Unmapped object", "Export fail", MB_OK);
		return mmFail;
	}
	
	// buffers
	expSafeArPtr<stMeshExpVertex>	buf_points;
	expSafeArPtr<stMeshExpVertex>	buf_normals;
	expSafeArPtr<stMeshExpUV>		buf_mapping;
	expSafeArPtr<stMeshExpFace16>	buf_faces;
	
	// buffers alloc
	buf_points = new stMeshExpVertex[mesh->numFaces * 3];
	buf_faces  = new stMeshExpFace16[mesh->numFaces];
	if (m_channel_normals)
		buf_normals = new stMeshExpVertex[mesh->numFaces * 3];
	if (m_channel_mapping)
		buf_mapping = new stMeshExpUV[mesh->numFaces * 3];
	
	// Get MAX object transform
	Matrix3	tf;
	tf = node->GetObjectTM(0);
		
	mmDword collapsed;
	if (MHMFAIL(CollapseMesh(mesh, buf_points,
								   buf_normals,
								   buf_mapping,
								   buf_faces,
								   &tf,
								   &collapsed)))
	{
		MessageBox(NULL, "MAX mesh collapsing error", "Export failed", MB_OK);
		return mmFail;
	}
	
	
	
	
	
	if (deltri)
		mmSafeDelete(triobj);
	
	return mmOk;
}







