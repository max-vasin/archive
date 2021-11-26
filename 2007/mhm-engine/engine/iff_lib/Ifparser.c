// AIfparser.cpp: implementation of the AIfparser class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "Ifparser.h"

 
IFILE Rover;  
IFileStruct Files[MAX_IFILE];
UBYTE Header[12];


//Ifparser * CreateIFParser()
//{
//	register Ifparser *r;

//	r = new Ifparser();

//	return r;
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Ifparser::Ifparser()
void WINAPI IF_Init()
{
	register i;
	*(ULONG*)&Header[0] = ID_FORM;
	SET_SIZE((ULONG*)&Header[4],4);
	*(ULONG*)&Header[8] = ID_MHM2;
	for(i = 0; i < MAX_IFILE; i++) { Files[i].status = 0; }
	Rover = 0;
}

//Ifparser::~Ifparser()
void WINAPI IF_Release()
{
	register i;

	
	for(i=1;i<=MAX_IFILE;i++)
	{
		IF_Close(i);
	}
}

//void Ifparser::Release()
//{
//	delete this;
//}

//IFILE Ifparser::FindSlot()
IFILE WINAPI IF_FindSlot()
{
	IFILE r=-1,i;

	if(Files[Rover].status == 0)
	{
		r = Rover;
	}
	else
	{
		i = Rover;
		do{
			Rover++; if(Rover>=MAX_IFILE) Rover=0;
			if(Files[Rover].status == 0){r = Rover; break;}
		}while(Rover!=i);
	}
	return r+1;
}

//IFILE Ifparser::OpenMem(void *p)
IFILE WINAPI IF_OpenMem(void *p)
{
	IFILE r = 0;


	if(GET_ID(p) == ID_FORM)
	{
		if(r = IF_FindSlot())
		{
			r--;
			Files[r].First.size = GET_SIZE((char*)p + 4);
			Files[r].First.id   = GET_ID((char*)p + 8);
			Files[r].First.start = 8;
			Files[r].Select = &Files[r].First;
			Files[r].First.Next = NULL;
			Files[r].First.Own = NULL;
			Files[r].mem = p;
			Files[r].pos = 0;

			Files[r].status = 1;
			r++;
		}
	}

	return r;
}

//void Ifparser::Close(IFILE f)
void WINAPI IF_Close(IFILE f)
{
	f--;
	if( (f<MAX_IFILE) && (Files[f].status) )
	{
		switch(Files[f].status)
		{
		case 1:
			while(((struct IFileFORM*)Files[f].Select)->Own) IF_Parent(f+1);
			break;
		case 2:
			while(((struct IFileFORM*)Files[f].Select)->Own) IF_Parent(f+1);
			break;
		}
		Files[f].status = 0;
	}
}

//IFILE Ifparser::Create(char *name)
IFILE WINAPI IF_Create(char *name)
{
	IFILE r=0;
/*	FILE *f;

	if(name && name[0])
	{
		if(r = IF_FindSlot())
		{
			r--;
			if(f = fopen(name,"wb"))
			{
				fwrite(Header,sizeof(Header),1,f);
				fclose(f);
				Files[r].First.size = 4;
				Files[r].First.id   = ID_MHM2;
				Files[r].First.start = 8;
				Files[r].Select = &Files[r].First;
				Files[r].First.Next = NULL;
				Files[r].First.Own = NULL;
				
				strcpy(Files[r].name,name);
				Files[r].status = 2; // open for write
				r++;
			}
			else r = 0;
		}
	}
*/
	return r;
}


//int Ifparser::WriteChunk(IFILE f, ULONG id, ULONG size, void *mem)
int WINAPI IF_WriteChunk(IFILE f, ULONG id, ULONG size, void *mem)
{
	int r = 0;
/*	UBYTE b[12];
	FILE *fi;
	IFileFORM *t,*nif;

	f--;
	if( (f<MAX_IFILE) && (Files[f].status==2))
	{
		if(fi = fopen(Files[f].name,"rb+"))
		{
			if(size)
			{
				r = size;
				size = (((size-1)|3)+1);
				*(ULONG*)&b[0] = id;
				SET_SIZE((ULONG*)&b[4],size);
				fseek(fi,0,SEEK_END);
				fwrite(b,8,1,fi);
				fwrite(mem,r,1,fi);
				r = size - r;
				*(ULONG*)&b[0] = 0;
				if(r) fwrite(b,r,1,fi);
				size += 8;
				nif = NULL;
			}
			else
			{
				*(ULONG*)&b[0] = ID_FORM;
				SET_SIZE((ULONG*)&b[4],4);
				*(ULONG*)&b[8] = id;
				fseek(fi,0,SEEK_END);
				fwrite(b,12,1,fi);
				size = 12;
				nif = (IFileFORM*)malloc(sizeof(IFileFORM));
			}
			t = Files[f].Select;
			do{
				t->size += size;
				fseek(fi,t->start-4,SEEK_SET);
				fread(b,4,1,fi);
				r = GET_SIZE(b);
				r += size;
				SET_SIZE((ULONG*)&b[0],r);
				fseek(fi,t->start-4,SEEK_SET);
				fwrite(b,4,1,fi);
				t = t->Own;
			}while(t);
			fclose(fi);
			if(nif)
			{
				t = Files[f].Select;
				nif->Own = t;
				nif->size = 4;
				nif->start = t->start + t->size - 4;
				Files[f].Select = nif;
			}
			r = 1;
		}
	}
*/	return r;
}

//int Ifparser::Parent(IFILE f)
int WINAPI IF_Parent(IFILE f)
{
	int r = 0;
	IFileFORM *t;

	f--;
	if((f<MAX_IFILE))
	{
		if((Files[f].status==2) || (Files[f].status==1))
		{
			t = Files[f].Select;
			if(t->Own)
			{
				t = t->Own;
				free(Files[f].Select);
				Files[f].Select = t;
			}
			r = 1;
		}
	}
	return r;
}

//int Ifparser::Find(IFILE f, ULONG id)
int WINAPI IF_Find(IFILE f, ULONG id)
{
	int r = 0,st,si,sz;
	UBYTE *mem;
	IFileFORM *t;

	f--;
	if((f<MAX_IFILE))
	{
		if(Files[f].status==1)
		{
			mem = (UBYTE*)Files[f].mem;
			if(t = Files[f].Select)
			{
				st = t->start + 4;
				si = t->size - 4;
				if(si>=8)
				{
					do{
						si -= 8;
						sz = GET_SIZE(&mem[st+4]);
						if( ((ULONG)sz)&0xf0000000) break; // safe use size
						if( GET_ID(&mem[st]) == id )
						{
							r = sz;
							if(!r) r++;
							Files[f].pos = st;
							break;
						};
						if(GET_ID(&mem[st]) == ID_FORM)
						{
							if(GET_ID(&mem[st+8]) == id)
							{
								r = 3; // FORM.....id. !!!
								Files[f].pos = st;
								break;
							}
						}
						si -= sz;
						st += (sz+8);
					}while(si>=8);
				}
			}
		}
	}
	return r;
}

//int Ifparser::SelectFound(IFILE f)
int WINAPI IF_SelectFound(IFILE f)
{
	int r = 0,st;
	UBYTE *mem;
	IFileFORM *t,*nif;

	f--;
	if((f<MAX_IFILE))
	{
		if((Files[f].status==1) && (st = Files[f].pos))
		{
			mem = (UBYTE*)Files[f].mem;
			if(GET_ID(&mem[st]) == ID_FORM)
			{
				nif = (IFileFORM*)malloc(sizeof(IFileFORM));
				t = Files[f].Select;
				nif->Own = t;
				nif->size = GET_SIZE(&mem[st + 4]);
				nif->start = st + 8;
				Files[f].Select = nif;
				Files[f].pos = 0;
				r = 1; // OK!!!
			}
		}
	}
	return r;

}

//int Ifparser::LoadFound(IFILE f, void *target, int size)
int WINAPI IF_LoadFound(IFILE f, void *target, int size)
{
	int r = 0;
//	UBYTE b[12];
	UBYTE *mem;
//	IFileFORM *t;

	f--;
	if( (f<MAX_IFILE) && (Files[f].status==1) && (target) && (size))
	{
		if(Files[f].pos)
		{
			mem = (UBYTE*)Files[f].mem;
			memcpy(target,&mem[Files[f].pos+8],size);
			r = 1;
		}
	}
	return r;
}

//void * Ifparser::GetFound(IFILE f)
void * WINAPI IF_GetFound(IFILE f)
{
	UBYTE *mem = NULL;

	f--;
	if( (f<MAX_IFILE) && (Files[f].status==1))
	{
		if(Files[f].pos)
		{
			mem = ((UBYTE*)Files[f].mem) + Files[f].pos + 8;
		}
	}
	return (void*)mem;
}

//int Ifparser::Next(IFILE f, ULONG *id)
int WINAPI IF_Next(IFILE f, ULONG *id)
{
	int r = 0,st,si,sz;
	UBYTE *mem;
	IFileFORM *t;

	f--;
	if((f<MAX_IFILE))
	{
		if(Files[f].status==1)
		{
			mem = (UBYTE*)Files[f].mem;
			if(t = Files[f].Select)
			{
				if((!Files[f].pos)) // begining
				{
					if(t->size >= 12)
					{
						st = t->start + 4;
						sz = GET_SIZE(&mem[st+4]);
						if(!(((ULONG)sz)&0xf0000000)) // safe use size
						{
						Files[f].pos = st;
						if(GET_ID(&mem[st]) == ID_FORM)
						{
							r = 3; // FORM.....id. !!!
							*id = GET_ID(&mem[st+8]);
						}
						else
						{
							r = sz;
							if(!r) r++;
							*id = GET_ID(&mem[st]);
						}
						}
					}
				}
				else
				{
				st = Files[f].pos;
				si = t->size - (st - t->start) - 8;
				sz = GET_SIZE(&mem[st+4]);
				if(!(((ULONG)sz)&0xf0000000))// safe use size
				{
				si -= sz;
				if(si>=8)
				{
					st += sz + 8;
					sz = GET_SIZE(&mem[st+4]);
					Files[f].pos = st;
					if(!(((ULONG)sz)&0xf0000000)) // safe use size
					{
					if(GET_ID(&mem[st]) == ID_FORM)
					{
						r = 3; // FORM.....id. !!!
						*id = GET_ID(&mem[st+8]);
					}
					else
					{
						r = sz;
						if(!r) r++;
						*id = GET_ID(&mem[st]);
					}
					}
				}
				}
				}
			}
		}
	}
	return r;
}

