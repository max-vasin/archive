#include "mhmbase.h"
#include "iff_lib\MeshFile.h"

struct SFVF__2
{
	FLOAT x,y,z;
	FLOAT nx,ny,nz;
};
#define D3DFVF__2 (D3DFVF_XYZ|D3DFVF_NORMAL)


struct SFVF_TT2
{
	FLOAT u,v;
};

#define D3DFVF_TT2 (D3DFVF_TEX1)


struct SFVF_T2
{
	FLOAT x,y,z;
	FLOAT nx,ny,nz;
	FLOAT u,v;
};
#define D3DFVF_T2 (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


struct SFVF_PTC2
{
	FLOAT x,y,z,r;
	D3DCOLOR c;
	FLOAT u,v;
};
#define D3DFVF_PTC2 (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)




struct SFVF_PC2
{
	FLOAT x,y,z,r;
	D3DCOLOR c;
};
#define D3DFVF_PC2  (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


struct SFVF_TP2
{
	float x,y,z;
	float d;
	float u,v;
};

#define D3DFVF_TP2 (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)


/*
void setcolor(D3DCOLORVALUE *c, mmDword d)
{
	c->r=((float)(d>>24))/255.0f;
	c->g=((float)((d>>16)&255))/255.0f;
	c->b=((float)((d>>8)&255))/255.0f;
	c->a=((float)((d)&255))/255.0f;
}
*/

clsStaticMesh::clsStaticMesh(clsResourceManager *presman, clsString name):
						   clsBaseMesh(presman, name, enResStaticMesh)
{
	mode = 0;
	n_bones = 0;
	BoneNames = mmnull;
}

clsStaticMesh::~clsStaticMesh(void)
{
		Destroy();
}

	
mmVoid clsStaticMesh::Destroy()
{
	register mmUint i;

	if(mode)
	{
		if(frames)
		{
			for(i=0;i<n_frames;i++)
			{
				if(frames[i].vb)
				{
					m_presman->DestroyVertexBuffer(frames[i].vb);
					//frames[i].vb = mmnull;
				}
			}
			mmSafeDelete(frames);
		}
		
		//m_presman->DestroyVertexBuffer(vb);
		//vb = mmnull;
		//n_verteces = 0;

		//m_presman->DestroyIndexBuffer(ib);
		//ib = mmnull;

		if(BoneNames)
		{
			delete [] BoneNames;
			//BoneNames = mmnull;
			//bones = mmnull;
			//phys = mmnull;
		}

		DestroyBaseMesh();
		mode = 0;
	}
}


mmRes clsStaticMesh::OnCreateMesh(clsResourceParams* params)
{
	Destroy();

	//LoadIF(this->m_name);
	mmRes res = LoadIF();
	if (MHMFAIL(res))
		return res;
	return res;
}

/*
mmRes clsStaticMesh::Render()
{
	MHMDirect3DDevice* pdev = *m_presman->GetDevice();	
	if (pdev == mmnull)
		return mmOk;
	
	pdev->SetFVF(fvf);
	pdev->SetStreamSource(0, *vb, 0, size);
	pdev->SetIndices(*ib);
	//pdev->Draw
	return mmOk;
}
*/

mmRes clsStaticMesh::getIndeces(mmUint n, mmInt *i1, mmInt *i2, mmInt *i3)
{
	if(m_baseibuf)
	{
		if(n<n_indexes)
		{
			n = n*3;
			mmWord *by;

			if(MHMSUCC(m_baseibuf->Lock((mmVoid**)&by)))
			{
				*i1 = (mmInt)by[n];
				*i2 = (mmInt)by[n+1];
				*i3 = (mmInt)by[n+2];
				m_baseibuf->Unlock();
				return mmOk;
			}
		}
	}
	return MHMBUF_NO_BUFFER;
}


mmRes clsStaticMesh::getCoord(mmUint i, mmVector *v)
{
	mmFloat *fl;

	if(m_basevbuf)
	{
		if(i<n_verteces)
		{
			if(MHMSUCC(m_basevbuf->Lock( (mmVoid**)&fl )))
			{
				fl = fl + (m_basevbuf->GetStride()>>2) * i;
				v->x = *fl; fl++;
				v->y = *fl; fl++;
				v->z = *fl;
				m_basevbuf->Unlock();
				return mmOk;
			}
		}
	}
	return MHMBUF_NO_BUFFER;
}

mmRes clsStaticMesh::Morph(mmUint id, mmFloat f, mmInt a, mmInt b)
{
//	if((mode & 0xf0)==0x50)
//	{
//		if( (id != morphid) || (f != morph_f) || (a != morph_a) || (b != morph_b) )
//		{
//			morphid = id; morph_f = f; morph_a = a; morph_b = b;
//			{
//			mmFloat k;
//			register mmInt f1,f2;
///*
//			k=(f)/(mmFloat)n_frames;
//			k=(mmFloat)((mmInt)k*n_frames);
//			f1=(mmInt)(f-k);
//			k=(f-(mmFloat)((mmInt)(f)));
//			if(f1 == (n_frames-1))f2=0; else f2=f1+1;
//*/
//			k = f; f1 = a; f2 = b;
//
//			xmax=(frames[f1].xmax>frames[f2].xmax)?frames[f1].xmax:frames[f2].xmax;
//			ymax=(frames[f1].ymax>frames[f2].ymax)?frames[f1].ymax:frames[f2].ymax;
//			zmax=(frames[f1].zmax>frames[f2].zmax)?frames[f1].zmax:frames[f2].zmax;
//			xmin=(frames[f1].xmin<frames[f2].xmin)?frames[f1].xmin:frames[f2].xmin;
//			ymin=(frames[f1].ymin<frames[f2].ymin)?frames[f1].ymin:frames[f2].ymin;
//			zmin=(frames[f1].zmin<frames[f2].zmin)?frames[f1].zmin:frames[f2].zmin;
//		
//		{
//			unsigned short *a,*b,*c;
//			register mmFloat *fl,*by;
//			mmFloat x1,y1,z1,x2,y2,z2;
//			mmFloat x01,y01,z01,x02,y02,z02;
//			register mmFloat xx,yy,zz;
//			mmInt i,j;
//
//			fl = (mmFloat*) vb->Lock();
//			
//			if(fl)
//			{
//				if(dupl && pack)
//				{
//					a = dupl; by = fl;
//					b = pack + 4*(n_verteces - n_duples)*f1;
//					c = pack + 4*(n_verteces - n_duples)*f2;
//					x1 = (frames[f1].xmax - frames[f1].xmin);
//					y1 = (frames[f1].ymax - frames[f1].ymin);
//					z1 = (frames[f1].zmax - frames[f1].zmin);
//					x2 = (frames[f2].xmax - frames[f2].xmin);
//					y2 = (frames[f2].ymax - frames[f2].ymin);
//					z2 = (frames[f2].zmax - frames[f2].zmin);
//					x01 = frames[f1].xmin;
//					y01 = frames[f1].ymin;
//					z01 = frames[f1].zmin;
//					x02 = frames[f2].xmin;
//					y02 = frames[f2].ymin;
//					z02 = frames[f2].zmin;
//						if(mode & 0x01)
//						{
//							for(i=0;i<(n_verteces);i++)
//							{
//								if(i == *a)
//								{
//									j = *(a + 1); a+=2;
//									memcpy(fl,&by[j*8],6*4);
//									fl+=8;
//								}
//								else
//								{
//									xx = mmFloat(*b++)/65535.0f * x1 + x01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * x2 + x02 - xx)*k;
//									xx = mmFloat(*b++)/65535.0f * y1 + y01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * y2 + y02 - xx)*k;
//									xx = mmFloat(*b++)/65535.0f * z1 + z01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * z2 + z02 - xx)*k;
//									Index2Normal(*b++,fl,fl+1,fl+2);
//									Index2Normal(*c++,&xx,&yy,&zz);
//									*fl++ = (xx - *fl)*k + *fl;
//									*fl++ = (yy - *fl)*k + *fl;
//									*fl++ = (zz - *fl)*k + *fl;
//									fl+=2;
//								}
//							}
//						}
//						else
//						{
//							for(i=0;i<n_verteces;i++)
//							{
//								if(i == *a)
//								{
//									j = *(a + 1); a+=2;
//									memcpy(fl,&by[j*6],6*4);
//									fl+=6;
//								}
//								else
//								{
//									xx = mmFloat(*b++)/65535.0f * x1 + x01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * x2 + x02 - xx)*k;
//									xx = mmFloat(*b++)/65535.0f * y1 + y01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * y2 + y02 - xx)*k;
//									xx = mmFloat(*b++)/65535.0f * z1 + z01;
//									*fl++ = xx + (mmFloat(*c++)/65535.0f * z2 + z02 - xx)*k;
//									Index2Normal(*b++,fl,fl+1,fl+2);
//									Index2Normal(*c++,&xx,&yy,&zz);
//									*fl++ = (xx - *fl)*k + *fl;
//									*fl++ = (yy - *fl)*k + *fl;
//									*fl++ = (zz - *fl)*k + *fl;
//								}
//							}
//						}
//			
//				}
//				vb->Unlock();
//			}
//			}
//			}	
//		}
//		return mmOk;
//	}
	
	return MHMBUF_NO_BUFFER;
}






double cycles=0;
int mcount=0;

#define	_m1	32
#define	_m2	40
#define	_m3	48
#define	_m4	56
#define	_si	64


mmUint clsStaticMesh::skinning(mmFloat *vbuf, mmUint count, mmFloat *pout, mmUint stride, mmUint16 * dup)
{
	mmUint n = count;
	mmUint cp = g_pEnvironment->m_eCpuType;
	mmUint timemark1,timemark2;
	
	/*
	__m64 mm0,mm1;
	__m128 row0,row1,row2,row3;

	((float*)&mm0)[0] = 1.0f;
	((float*)&mm0)[1] = 2.0f;
	((float*)&mm1)[0] = 3.0f;
	((float*)&mm1)[1] = 4.0f;

	//mm0 = _m_pfacc (mm0, mm1);
	_MM_TRANSPOSE4_PS(row0, row1, row2, row3)
	*/

#ifdef WIN32
	__asm{
			rdtsc
			mov		timemark1,eax
			mov		timemark2,edx
	}
#endif

#pragma region CPP
	if(cp==0) 
	{
	// cpp
		{
			register mmUint i,j,k;
			clsVector3 v,m;
			mmFloat *f,t;
			mmFloat *targ=pout;

			for(i=0;i<count;i++)
			{
				if( ((mmUint16)i) == *dup)
				{
					j = (dup[1] * stride)>>2;

					pout[0] = targ[j+0];
					pout[1] = targ[j+1];
					pout[2] = targ[j+2];
					pout[3] = targ[j+3];
					pout[4] = targ[j+4];
					pout[5] = targ[j+5];

					dup +=2;
				}
				else
				{
					k = *((mmUint *)(&vbuf[7]));

					m.x = 0;m.y = 0;m.z = 0;
					for(j=0; j<k; j++)
					{
						f = (mmFloat*) *((mmFloat**)(&vbuf[8+j*2]));
						v.x = vbuf[0]*f[0] + vbuf[1]*f[1] + vbuf[2]*f[2] + f[3];
						v.y = vbuf[0]*f[4] + vbuf[1]*f[5] + vbuf[2]*f[6] + f[7];
						v.z = vbuf[0]*f[8] + vbuf[1]*f[9] + vbuf[2]*f[10] + f[11];
						m += v * vbuf[9+j*2];
					}
					pout[0] = m.x;
					pout[1] = m.y;
					pout[2] = m.z;

					m.x = 0;m.y = 0;m.z = 0;
					for(j=0; j<k; j++)
					{
						f = (mmFloat*) *((mmFloat**)(&vbuf[8+j*2]));
						v.x = vbuf[4]*f[0] + vbuf[5]*f[1] + vbuf[6]*f[2];
						v.y = vbuf[4]*f[4] + vbuf[5]*f[5] + vbuf[6]*f[6];
						v.z = vbuf[4]*f[8] + vbuf[5]*f[9] + vbuf[6]*f[10];
						m += v * vbuf[9+j*2];
					}
					t = 1.0f / sqrtf(m.x*m.x + m.y*m.y + m.z*m.z);
					vbuf = vbuf + 16;
					pout[3] = m.x*t;
					pout[4] = m.y*t;
					pout[5] = m.z*t;
				}
				pout = (mmFloat*) (((mmUchar*)pout) + stride);
			}

		}
	}
#pragma endregion

#ifdef WIN32
	__asm {
		push	ebx
			push	ecx
			push	edx
			push	esi
			push	edi


			mov	ecx,cp
			//	mov	eax,count
			mov	eax,dup
			dec	ecx
			mov	esi,vbuf
			jnz	_3dnow

			// sse

			mov	edi,pout
			sub	edi,stride
loopsse:
		//	prefetchnta  [esi+64]
		mov	ebx,0xffff
			mov	ecx,[eax]
		and	ebx,ecx
			shr	ecx,16
			add	ebx,count
			imul	ecx,stride
			sub	ebx,n
			jnz	tras
			add	ecx,pout
			add	eax,4
			movups	xmm0,[ecx]
		add	edi,stride
			movups	xmm1,[ecx+8]
		dec	count
			movups	[edi],xmm0
			movups	[edi+8],xmm1
			jnz	loopsse

			jmp	_end

tras:

		add	edi,stride
			xorps	xmm6,xmm6
			mov	ecx,[esi+28]
		xorps	xmm7,xmm7
			movups	xmm4,[esi]		// xyz1
		lea	edx,[esi+_m1]
		movups	xmm5,[esi+16]		// norm
		//	andps	xmm5,xmm3
loops0:
		mov	ebx,[edx]
		movups	xmm0,[ebx]
		mulps	xmm0,xmm4
			add	edx,8
			movups	xmm1,[ebx+16]
		mulps	xmm1,xmm4
			movups	xmm3,xmm1
			unpcklps	xmm1,xmm4
			movups	xmm2,[ebx+32]
		unpckhps	xmm3,xmm4
			mulps	xmm2,xmm4
			addps	xmm3,xmm1     // xmm3 - y.y.

			movups	xmm1,xmm0
			unpcklps	xmm0,xmm2
			unpckhps	xmm1,xmm2
			addps	xmm1,xmm0     // xmm1 - xzxz
			movups	xmm0,[ebx]
		mulps	xmm0,xmm5
			movups	xmm2,xmm1
			unpcklps	xmm1,xmm3
			unpckhps	xmm2,xmm3
			movss	xmm3,[edx-4]
		addps	xmm2,xmm1
			shufps	xmm3,xmm3,0
			movups	xmm1,[ebx+16]
		mulps	xmm2,xmm3
			mulps	xmm1,xmm5
			addps	xmm6,xmm2
			movups	xmm3,xmm1
			unpcklps	xmm1,xmm4
			movups	xmm2,[ebx+32]
		unpckhps	xmm3,xmm4
			mulps	xmm2,xmm5
			addps	xmm3,xmm1
			movups	xmm1,xmm0
			unpcklps	xmm0,xmm2
			unpckhps	xmm1,xmm2
			addps	xmm1,xmm0     // xmm1 - xzxz
			movups	xmm2,xmm1
			unpcklps	xmm1,xmm3
			unpckhps	xmm2,xmm3
			movss	xmm3,[edx-4]
		addps	xmm2,xmm1
			shufps	xmm3,xmm3,0
			dec	ecx
			mulps	xmm2,xmm3
			addps	xmm7,xmm2
			jnz	loops0

			movups	[edi],xmm6
			add	esi,64
			movups	xmm5,xmm7
			mulps	xmm7,xmm7
			mov	ecx,[edi+24]
		movups	xmm4,xmm7
			unpcklps	xmm7,xmm7
			unpckhps	xmm4,xmm4
			movups	xmm6,xmm7
			unpcklps	xmm4,xmm4
			unpcklps	xmm7,xmm7
			unpckhps	xmm6,xmm6
			addps	xmm7,xmm4
			addps	xmm7,xmm6
			dec	count
			rsqrtps	xmm7,xmm7
			mulps	xmm7,xmm5
			movups	[edi+12],xmm7
			mov	[edi+24],ecx
			jnz	loopsse

			jmp	_end


_3dnow:
		dec	ecx
			jnz	_x86

			///////////////////////////////////////////
			// 3dnow
			mov	edi,pout
			sub	edi,stride
loop3dn:
		mov	ebx,0xffff
			mov	ecx,[eax]
		and	ebx,ecx
			shr	ecx,16
			add	ebx,count
			imul	ecx,stride
			sub	ebx,n
			jnz	tra
			add	ecx,pout
			add	eax,4
			movq	mm0,[ecx]
		add	edi,stride
			movq	mm1,[ecx+8]
		movq	mm2,[ecx+16]
		movq	[edi],mm0
			movq	[edi+8],mm1
			dec	count
			movq	[edi+16],mm2
			jnz	loop3dn

			femms
			jmp	_end

tra:
		prefetch  [esi+64]
		add	edi,stride
			pxor	mm4,mm4
			mov	ecx,[esi+28]
		pxor	mm5,mm5
			lea	edx,[esi+_m1]
loopv:
		mov	ebx,[edx]
		movq	mm6,[esi]
		add	edx,8
			movq	mm7,[esi+8]
		movq	mm0,mm6
			pfmul	mm6,[ebx]       ; x
			movq	mm1,mm7
			pfmul	mm7,[ebx+8]	
		movq	mm2,mm0
			pfmul	mm0,[ebx+16]       ; y
			movq	mm3,mm1
			pfmul	mm1,[ebx+24]
		pfadd	mm6,mm7
			pfmul	mm2,[ebx+32]       ; z
			pxor	mm7,mm7
			pfmul	mm3,[ebx+40]
		pfadd	mm0,mm1
			pfadd	mm2,mm3
			movd	mm1,[edx-4]
		pfacc	mm6,mm0
			dec	ecx
			punpckldq  mm1,mm1//[edx-4]//mm1
			pfacc	mm2,mm7
			pfmul	mm6,mm1
			pfmul	mm2,mm1
			pfadd	mm4,mm6
			pfadd	mm5,mm2
			jnz	loopv

			movq	[edi],mm4
			pxor	mm4,mm4
			mov	ecx,[esi+28]
		movd	[edi+8],mm5
			pxor	mm5,mm5
			lea	edx,[esi+_m1]
loopn:
		mov	ebx,[edx]
		movq	mm6,[esi+16]
		add	edx,8
			movq	mm7,[esi+24]
		movq	mm0,mm6
			pfmul	mm6,[ebx]       ; x
			movq	mm1,mm7
			pfmul	mm7,[ebx+8]	
		movq	mm2,mm0
			pfmul	mm0,[ebx+16]       ; y
			movq	mm3,mm1
			pfmul	mm1,[ebx+24]
		pfadd	mm6,mm7
			pfmul	mm2,[ebx+32]       ; z
			pxor	mm7,mm7
			pfmul	mm3,[ebx+40]
		pfadd	mm0,mm1
			pfadd	mm2,mm3
			movd	mm1,[edx-4]
		pfacc	mm6,mm0
			dec	ecx
			punpckldq  mm1,mm1
			pfacc	mm2,mm7
			pfmul	mm6,mm1
			pfmul	mm2,mm1
			pfadd	mm4,mm6
			pfadd	mm5,mm2
			jnz	loopn

			movq	mm0,mm4
			movq	mm1,mm5
			pfmul	mm4,mm4
			pfmul	mm5,mm5
			add	esi,64
			pfacc	mm4,mm5
			pfacc	mm4,mm2
			pfrsqrt	mm4,mm4
			dec	count
			pfmul	mm0,mm4
			movq	[edi+12],mm0
			pfmul	mm1,mm4
			movd	[edi+20],mm1	; // keep bones quantity
		jnz	loop3dn

			femms
;;;;;			jmp	_end
_x86:

_end:
				rdtsc
				sub	eax,timemark1
				mov	timemark1,eax
				sbb edx,timemark2
				mov timemark2,edx

				pop	edi
				pop	esi
				pop	edx
				pop	ecx
				pop	ebx
		}

		count = timemark1;

		mcount++;
		cycles =+ ((double)(count));
#endif

		return	count;
}

mmVoid clsStaticMesh::Animate(clsEntity* pBonesOwner)
{
	mmUint bones = getBones();
	clsMatrix Mat;
	clsEntity* Ent;
	//clsString BoneName;
	mmString BoneName = mmnull;
	if(bones>0) // bones !!!
	{
		Mat = pBonesOwner->m_mxFull;
		Mat.Inverse();
		for(mmUint i = 0; i < bones; i++)
		{
			if((Ent = (clsEntity * )getBoneHandle(i)) == mmnull)
			{
				BoneName = getBoneName(i);
				Ent = pBonesOwner->FindChild(BoneName);
			}
			if(Ent)
				setBoneMatrix(i, &(Ent->m_mxFull * Mat));
		}
	}
	else
		return;
	
	mmFloat * VB;

	if(mode)
	{
		if(BoneNames)
		{
			if(MHMSUCC(m_basevbuf->Lock((mmVoid**)&VB)))
			{
				skinning(phys, n_verteces, VB, size, dupl);
				m_basevbuf->Unlock();
			}
		}
		else
		{
			//m->Morph((DWORD)o,o->getBlend(),o->getFrameA(),o->getFrameB());
		}
		return;
	}
	return;
}


mmRes clsStaticMesh::LoadIF()
{
	//mmInt r=0;
	mmUint i,j,a,nm,al,mip;
	mmWord *dup,*pac;
	mmByte *file3dx, *p;
	
	IFileMesh head;
	IFileMeshMaterial *mt;
//	IFileMeshMaterial2 *mt2;
	IFILE f;
	mmByte *by;
	
	mmFloat *bb;

	clsFile MeshFile(m_fullname);
	if(MHMFAIL(MeshFile.Open())) return mmErr;
	if(MHMFAIL(MeshFile.ReadToMemory((mmVoid**)&p))) return mmErr;
	if((file3dx=p)==mmnull) return mmErr;
	m_indexed = mmtrue;

	f = IF_OpenMem(p);
	if(f)
	{
		i = IF_Find(f,ID_HEAD);
		if(i)
		{
			IF_LoadFound(f,&head,sizeof(head));

			n_verteces = head.vertexes;
			n_duples = head.duplicates;
			n_indexes = head.tris;
			nm = head.materials;
			n_bones = head.bones;
			
			frames = NULL;
			dupl = NULL;

			i = IF_Find(f,ID_DUPL);
			dup = new mmWord[n_duples * 2 + 2];
			if(i)
			{
				IF_LoadFound(f,dup,i);
			}

			dup[n_duples * 2] = 0xffff;
			
			i = IF_Find(f,ID_XYZN);
			pac = new unsigned short[i>>1];
			IF_LoadFound(f,pac,i);

			i = IF_Find(f,ID_BBOX);
			bb = (mmFloat*)IF_GetFound(f);

			switch(head.type)
			{
			case MESH_TYPE_PLAIN:
				if(head.flags & MESH_FLAG_DUVM)
				{
					size=sizeof(SFVF_T2);a=D3DFVF_T2; mode = 0x11; // plain IF mesh
				}
				else
				{
					size=sizeof(SFVF__2);a=D3DFVF__2; mode = 0x10;
				}
				n_frames = 0;
				xmin = *bb++;
				ymin = *bb++;
				zmin = *bb++;
				xmax = *bb++;
				ymax = *bb++;
				zmax = *bb++;
				dupl = NULL;
				pack = NULL;
				if(n_bones) dupl =  dup; // for phy !!!!
				break;

			case MESH_TYPE_MORPH:
				if(head.flags & MESH_FLAG_DUVM)
				{
					size=sizeof(SFVF_T2);a=D3DFVF_T2; mode = 0x51; // anim IF mesh
				}
				else
				{
					size=sizeof(SFVF__2);a=D3DFVF__2; mode = 0x50;
				}
				n_frames = head.frames;
				frames= new CFrame[n_frames];
				//a = sizeof(SFVF__2)*n_verteces;

//				frames = new twframe[o->track];
				for(i=0;i<n_frames;i++)
				{
					frames[i].vb = NULL;
					frames[i].xmin = *bb++;
					frames[i].ymin = *bb++;
					frames[i].zmin = *bb++;
					frames[i].xmax = *bb++;
					frames[i].ymax = *bb++;
					frames[i].zmax = *bb++;
				}
				dupl = dup;
				pack = pac;
				break;
			
			default:
				break;
			}
			fvf = a;

			BoneNames = NULL;
			if(n_bones)
			{
			// bones
				j = IF_Find(f,ID_PHYS);
				if(j)
				{
					a = n_bones*132 + (n_verteces-n_duples)*64;
					BoneNames = new mmChar[a];
					bones = ((mmFloat*)(BoneNames)) + n_bones*8;
					phys = bones + n_bones*25;
					IF_LoadFound(f,BoneNames+a-j,j);
					// names
					memcpy(BoneNames,BoneNames+a-j,n_bones*32);

					// weights
					
					{
						register mmFloat *fl,w,l;
						mmByte *bytes;
						mmUint e,b,c,d;
						clsMatrix Id;
						Id.Identity();

						for(e=0; e<n_bones; e++)
						{
							setBoneHandle(e, 0L);
							setBoneInit(e, &Id);
							setBoneMatrix(e, &Id);
						}
					
						fl = phys + 7; // move to count
						bytes = (mmByte *)(BoneNames+a-j+n_bones*32);
						for(i=0;(mmUint)i<(n_verteces-n_duples);i++)
						{
							e = (mmUint)*bytes; bytes++;
							*((mmUint*)fl) = e; e*=2;
							l = 1.0f;
							for(c=0;c<e;c+=2)
							{
								b = (mmUint)*bytes; bytes++;
								*((mmUint**)&fl[1+c]) = (mmUint*)bones + b*25 + 13;
								if(c<(e-2))
								{
								d = (mmUint)*bytes; bytes++;
								d<<=8;
								d += (mmUint)*bytes; bytes++;
								d+=1;
								w = ((mmFloat)d) * (1.0f / 65536.0f);
								l -= w;
								}
								else
								{
									w = l;
								}
								fl[2+c] = w;
							}
							fl+=16;
						}
					}
				}


			}


			if(size)
			{
				i = size * n_verteces;

				//vb = ((CDevice*)Dev)->CreateVerts(n_verteces,fvf,size);
				//m_basevbuf = m_presman->CreateVertexBuffer();
				m_basevbuf = m_presman->CreateVertexBuffer();

				if(m_basevbuf)
				{
					m_basevbuf->Create(n_verteces, fvf);

					if(MHMSUCC(m_basevbuf->Lock((mmVoid**)&by)))
					{
						memset(by,0,size);

						if(mode & 0x01)
						{
							i = IF_Find(f,ID_DUVM);
							if(i)
							{
								bb = (mmFloat*)IF_GetFound(f);
								for(i=0;i<n_verteces;i++)
								{
									((mmFloat*)by)[i*8+6] = *bb++;
									((mmFloat*)by)[i*8+7] = *bb++;
								}
							}
						}

						if( head.type == MESH_TYPE_PLAIN)
						{
							if(dup&&pac)
							{
								{
									unsigned short *a,*b;
									register mmFloat *fl;
									register mmFloat x,y,z;

									x = (xmax - xmin);
									y = (ymax - ymin);
									z = (zmax - zmin);
									b = pac;

									if(n_bones)
									{
										fl = phys;
										for(i=0;(mmUint)i<(n_verteces-n_duples);i++)
										{
											*(fl+0) = mmFloat(*b++)/65535.0f * x + xmin;
											*(fl+1) = mmFloat(*b++)/65535.0f * y + ymin;
											*(fl+2) = mmFloat(*b++)/65535.0f * z + zmin;
											*(fl+3) = 1;
											Index2Normal(*b++,fl+4,fl+5,fl+6);
											fl+=16;	
										}
									}
									else
									{
										al = (mode & 0x01)?32:24;
										a = dup; fl = (mmFloat*)by;
										for(i=0;i<n_verteces;i++)
										{
											if(i == *a)
											{
												j = *(a + 1); a+=2;
												memcpy(fl,&by[j*al],24);
											}
											else
											{
												*(fl+0) = mmFloat(*b++)/65535.0f * x + xmin;
												*(fl+1) = mmFloat(*b++)/65535.0f * y + ymin;
												*(fl+2) = mmFloat(*b++)/65535.0f * z + zmin;
												Index2Normal(*b++,fl+3,fl+4,fl+5);
											}
											fl = (mmFloat*) (((mmByte*)fl) + al);
										}

									}

								}
							}
							if( (n_bones==0) && dup) delete dup;
							if(pac) delete pac;
						}

						m_basevbuf->Unlock();
					}
				}
			}


			i = n_indexes * 6;

			m_baseibuf = m_presman->CreateIndexBuffer();
			
			m_baseibuf->Create(i,D3DFMT_INDEX16); // !!!

			if(MHMSUCC(m_baseibuf->Lock((mmVoid**)&by)))
			{
				j = IF_Find(f,ID_TRIS);
				if(j) IF_LoadFound(f,by,i);
				m_baseibuf->Unlock();
			}

			if(head.type == MESH_TYPE_MORPH)
			{
				Morph(0,0,0,0);
			}

//			n_opacity = 0; n_transparent = 0;

			i = IF_Find(f,ID_MTLS);
			if(i)
			{
				mt = (IFileMeshMaterial*)IF_GetFound(f);
/*
				for(i=0;i<nm;i++)
				{
					if( (mt[i].type) & MMAT_FLAG_ALPHA ) n_transparent++; else n_opacity++;
				}
*/
	//LPMATERIAL ma;
				clsBaseMaterial *ma;
				mmBool	is_material;
	
				m_psubsets = new clsBaseMeshSubset[nm];
				m_materials = new clsResource*[nm];
				m_hassubsets = mmtrue;
				m_pttype  = D3DPT_TRIANGLELIST;
				m_indexed = mmtrue;
				m_numsubs = nm;
				
	/*			opacity = (n_transparent<nm)? (new mmDword*[nm-n_transparent]) : NULL;
				transparent = (n_transparent)? (new mmDword*[n_transparent]) : NULL;*/
				
				clsString	m_matName;
				m_nummtls = nm;

				for(i=0;i<nm;i++)
				{	
					a = mt->type;
					
					m_matName = mt->name;
					m_psubsets[i].m_vstart = mt->i_vtx;
					m_psubsets[i].m_vcount = mt->n_vtx;
					m_psubsets[i].m_start = mt->i_idx;
					m_psubsets[i].m_count = mt->n_idx;
					m_psubsets[i].m_mtlindex = i;

					if(
						(m_matName.FindSubString(".jpg") >= 0) ||
						(m_matName.FindSubString(".JPG") >= 0) ||
						(m_matName.FindSubString(".jpeg") >= 0) ||
						(m_matName.FindSubString(".JPEG") >= 0) ||
						(m_matName.FindSubString(".tga") >= 0) ||
						(m_matName.FindSubString(".TGA") >= 0) ||
						(m_matName.FindSubString(".jp2") >= 0) ||
						(m_matName.FindSubString(".JP2") >= 0) ||
						(mt->name[0] == '\0')
						)		
					{ // old format
						ma = (clsBaseMaterial *)m_presman->GetResource("", enResBaseMaterial,0,&is_material);
					}
					else
					{ // new format
						m_materials[i] = (clsBaseMaterial *)m_presman->GetResource(mt->name + clsString(".MTL"), enResBaseMaterial,0,&is_material);
						if (m_materials[i]->GetBlendType() != enBlendNone)
							SetBlendType(m_materials[i]->GetBlendType());

						mt++;
						continue;
					}

					m_materials[i] = ma;

	/*
					ma = (clsBaseMaterial *)m_presman->GetResource(mt->name + clsString(".MTL"), enResBaseMaterial,0,&is_material);
					if(a & MMAT_FLAG_ALPHA)
					{ al = 8; transparent[i-n_opacity] = (mmDword*)ma;} 
					else 
					{ al = 0; opacity[n_opacity] = (mmDword*)ma; n_opacity++; }*/

					//19july2006 - special for material


					ma->SetColorLong(&ma->m_dxmtl.Ambient, mt->amb);
					if( mt->name[0] )
					{
						ma->SetColorLong(&ma->m_dxmtl.Diffuse, 0xffffffff);
					}
					else
					{
						ma->SetColorLong(&ma->m_dxmtl.Diffuse, mt->dif);
					}
					ma->SetColorLong(&ma->m_dxmtl.Specular, mt->spe);
					ma->SetColorLong(&ma->m_dxmtl.Emissive, mt->emi);
					if(a & MMAT_FLAG_ALPHA){
						ma->SetBlendType(enBlendAlpha);
						SetBlendType(enBlendAlpha);

						if(ma->m_dxmtl.Diffuse.a>0.98f)
						{
							ma->m_dxmtl.Diffuse.a = 0.5f;
						}
					}


					ma->m_dxmtl.Power = 0;//*(mmFloat*)&mt->pow;
					ma->reflection = *(mmFloat*)&mt->ref;

					//ma->mask = 0;
					ma->m_bTwoSided = !((a&MMAT_FLAG_TWOS)==0);

					if( mt->name[0] )
					{
						if( a&MMAT_FLAG_MASK ){
							al = 1;
						} 
						mip=0; // all lods !

						if( a&MMAT_FLAG_NMIP ){
							mip=1;
						}
						ma->m_map_diffuse.m_texture = (clsBaseTexture*)m_presman->GetResource(mt->name, MHMRES_TEXTURE_ID );
						//ma->LoadTexture(mt->name, mip, al);
					}
					//else {ma->m_map_diffuse.m_texture = NULL;}

					//if(mt->type & 0x80)
					//{
					//	mt2 = (IFileMeshMaterial2 *)(mt);	// get 2nd block
					//	if( mt2->detail_name[0] )
					//	{
					//		ma->detail = ((CDevice*)Dev)->CreateTexture(mt2->detail_name,0,mip);
					//	}
					//	if( mt2->dot3_name[0] )
					//	{
					//		ma->dot3 = ((CDevice*)Dev)->CreateTexture(mt2->dot3_name,0,mip);
					//	}

					//	// SHADER CODES HERE
					//	
					//	mt2++;
					//	mt = (IFileMeshMaterial *)(mt2);
					//}
					//else
					//{
						mt++;
					//}

				}
			}
		}
		IF_Close(f);
	}

	MeshFile.FreeMemory(file3dx);

	return mmOk;
}


mmUint clsStaticMesh::getBones()
{
	if(mode)
	{
		if(BoneNames)
		{
			return n_bones;
		}
		else return 0;
	}
	else return -1;
}

mmChar * clsStaticMesh::getBoneName(mmUint i)
{
	if(mode && BoneNames && (i<n_bones))
	{
		return &BoneNames[i*32];
	}
	else return 0;
}

mmDword clsStaticMesh::getBoneHandle(mmUint i)
{
	if(mode && BoneNames && (i<n_bones))
	{
		return *((mmDword*)&bones[i*25]);
	}
	else return 0L;
}

mmVoid clsStaticMesh::setBoneHandle(mmUint i, mmUint h)
{
	if(mode && BoneNames && (i<n_bones))
	{
		*((mmUint*)&bones[i*25]) = h;
	}
}

mmVoid clsStaticMesh::setBoneInit(mmUint i, clsMatrix *m)
{

	mmFloat *f = &bones[1+i*25];

	if(mode && BoneNames && (i<n_bones))
	{
		*f++  = m->_11;
		*f++  = m->_21;
		*f++  = m->_31;
		*f++  = m->_41;
		*f++  = m->_12;
		*f++  = m->_22;
		*f++  = m->_32;
		*f++  = m->_42;
		*f++  = m->_13;
		*f++  = m->_23;
		*f++  = m->_33;
		*f++  = m->_43;
	}
}

mmVoid clsStaticMesh::setBoneMatrix(mmUint i, clsMatrix *m)
{
	clsMatrix mm;
	mmFloat *f;

	if(mode && BoneNames && (i<n_bones))
	{
		f = &bones[1+i*25];
		mm._11 = *f++;
		mm._21 = *f++;
		mm._31 = *f++;
		mm._41 = *f++;
		mm._12 = *f++;
		mm._22 = *f++;
		mm._32 = *f++;
		mm._42 = *f++;
		mm._13 = *f++;
		mm._23 = *f++;
		mm._33 = *f++;
		mm._43 = *f++;
		mm._14 = 0;
		mm._24 = 0;
		mm._34 = 0;
		mm._44 = 1;

		mm *= *m;

		*f++ = mm._11;
		*f++ = mm._21;
		*f++ = mm._31;
		*f++ = mm._41;
		*f++ = mm._12;
		*f++ = mm._22;
		*f++ = mm._32;
		*f++ = mm._42;
		*f++ = mm._13;
		*f++ = mm._23;
		*f++ = mm._33;
		*f++ = mm._43;
	}
}

	
