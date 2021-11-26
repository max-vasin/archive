//==========================================================================================
// Mayhem engine array
// Coded:	Pink
// Created:	15.06.2006
//==========================================================================================

#ifndef _MHMARRAY_INCLUDED
#define _MHMARRAY_INCLUDED

#include "mhmtypes.h"



template <class T>
class tmpBazzArr
{
protected:

	//-------------------------------------------------------------------------------
	T * data;
	mmUint size;
	mmUint max_size;
	mmUint add_size;
	//-------------------------------------------------------------------------------
	mmVoid construct(mmUint siz, mmUint add)
	{
		AutoTruncate = mmtrue;
		size = 0;
		add_size = max(add , 1);
		max_size = add;
		if(max_size > 0)
			data = new T[max_size];
		else
			data = mmnull;
	}
	//-------------------------------------------------------------------------------
	mmVoid free_data()
	{
		if(data != mmnull)
		{
			delete [] data;
			data = mmnull;
		}
	}
	//-------------------------------------------------------------------------------
	mmVoid add(mmUint siz)
	{
		mmUint t_s;
		t_s = size;
		size += siz;
		if(max_size < size)
		{
			while(max_size < size)
			{
				max_size += add_size;
			}
			T * d = new T[max_size];
			if(t_s > 0)
			{
				//memcpy(d,data,sizeof(T)*t_s);
				for(mmUint z=0;z<t_s;z++)
					d[z] = data[z];
			}
			free_data();
			data = d;
		}
	}


	//-------------------------------------------------------------------------------
	/*
	mmVoid quickSort(T * a, mmInt N)
	{
		mmInt i = 0, j = N;
		T temp, p;
		p = a[ N>>1 ];
		do {
			while ( a[i] < p ) i++;
			while ( a[j] > p ) j--;
			if (i <= j)
			{
				temp = a[i]; a[i] = a[j]; a[j] = temp;
				i++; j--;
			}
		} while ( i<=j );
		if ( j > 0 ) quickSort(a, j);
		if ( N > i ) quickSort(a+i, N-i);
	}
	*/
	//-------------------------------------------------------------------------------
	mmVoid autoTruncate()
	{
		if(max_size == size)
			return;
		if((max_size - size)<= add_size)
			return;
		Truncate();
	}

public:

	//-------------------------------------------------------------------------------
	tmpBazzArr(){construct(0,0);}
	tmpBazzArr(mmUint Size){construct(Size,Size);}
	tmpBazzArr(mmUint Size,mmUint Add){construct(Size,Add);}
	tmpBazzArr(tmpBazzArr& arr)
	{
		size = arr.size;
		add_size = arr.add_size;
		max_size = arr.max_size;
		data = new T[max_size];
		if (data == mmnull)
			mmThrow(enExceptOutOfMemory, tmpBazzArr, Copy constructor, new array memory alloc failed);
		memcpy(data, arr.data, size * sizeof(T));
	}
	//-------------------------------------------------------------------------------
	~tmpBazzArr(){free_data();}

	//-------------------------------------------------------------------------------
	mmBool AutoTruncate;
	//-------------------------------------------------------------------------------
	mmVoid Add(const T & n)
	{
		add(1);
		data[size-1] = n;
	}
	//-------------------------------------------------------------------------------
	mmVoid PushBack(const T & n)
	{
		add(1);
		data[size-1] = n;
	}
	//-------------------------------------------------------------------------------
	mmVoid PushFront(const T & n)
	{
		mmUint t_s = size;
		add(1);
		for(mmUint z = t_s; z > 0; z--)
			data[z] = data[z-1];
		data[0] = n;
	}
	//-------------------------------------------------------------------------------
	mmVoid Truncate()
	{
		if(max_size == size)
			return;
		max_size = size;
		if(size == 0)
		{
			free_data();
		}
		T * d = new T[max_size];
		memcpy(d,data,size);
		free_data();
		data = d;
	}
	//-------------------------------------------------------------------------------
	mmVoid DeleteBack()
	{
		if(size == 0)
			return;
		size--;
		if(AutoTruncate)autoTruncate();
	}
	//-------------------------------------------------------------------------------
	mmVoid Delete(mmUint i)
	{
		if(i>=size)
			return;
		size--;
		for(mmUint z = i; z < size; z++)
			data[z] = data[z+1];
		if(AutoTruncate)autoTruncate();
	}
	//-------------------------------------------------------------------------------
	mmVoid FastDelete(mmUint i)
	{
		if(i>=size)
			return;
		size--;
		data[i] = data[size];
	}
	//-------------------------------------------------------------------------------
	mmVoid Delete(mmUint start, mmUint end)
	{
		if(((start+1)>size)||(start > end))
			return;
		mmUint n = size - end - 1;
		for(mmUint z = 0; z < n; z++)
			data[start+z] = data[end+z+1];
		size -= (end - start +1);
		if(AutoTruncate)autoTruncate();
	}
	//-------------------------------------------------------------------------------
	mmUint Size()
	{
		return size;
	}
	//-------------------------------------------------------------------------------
	mmUint Count()
	{
		return size;
	}
	//-------------------------------------------------------------------------------
	mmVoid Size(mmUint Siz)
	{
		if(Siz <= max_size)
		{
			size = Siz;
			if(AutoTruncate)autoTruncate();
			return;
		}
		add(Siz-size);
	}
	//-------------------------------------------------------------------------------
	mmUint MaxSize()
	{
		return max_size;
	}
	//-------------------------------------------------------------------------------
	mmVoid MaxSize(mmUint Max_Size)
	{
		if(Max_Size == max_size)
			return;
		max_size = Max_Size;
		size = min(size,Max_Size);
		T * d = new T[Max_Size];
		for(mmUint z=0;z<size;z++)
			d[z] = data[z];
		free_data();
		data = d;
	}
	//-------------------------------------------------------------------------------
	mmUint GetAddSize()
	{
		return add_size;
	}
	//-------------------------------------------------------------------------------
	mmVoid SetAddSize(mmUint Siz)
	{
		add_size = max(Siz , 1);
	}
	const tmpBazzArr& operator = (tmpBazzArr& arr)
	{
		free_data();
		size = arr.size;
		add_size = arr.add_size;
		max_size = arr.max_size;
		data = new T[max_size];
		if (data == mmnull)
			mmThrow(enExceptOutOfMemory, tmpBazzArr, Copy constructor, new array memory alloc failed);
		memcpy(data, arr.data, size * sizeof(T));
		return *this;
	}
	//-------------------------------------------------------------------------------
	/*
	T & operator[](mmUint i)
	{
		if(i >= max_size)
			throw "Out of range";
		if(i >= size)
			add(size-i+1);
		return data[i];
	}*/
	//-------------------------------------------------------------------------------
	/*
	mmInt Find(const T & n)
	{
		for(mmUint z = 0; z<size;z++)
			if(data[z] == n)
				return z;
		return -1;
	}
	*/
	//-------------------------------------------------------------------------------
	/*
	mmVoid Sort()
	{         
		if(size == 0)
			return;
		quickSort(data,size-1);
	} 
	*/
	//-------------------------------------------------------------------------------
	/*
	mmInt FindInSorted(const T & n)
	{
		if(size == 0)
			return -1;
		if(n > data[size-1])
			return -1;
		else
			if(n < data[0])
				return -1;
			else
				if(n == data[0])
					return 0;
				else
					if(n == data[size-1])
						return (size-1);
		mmUint i1,i2,i;
		i1=0;
		i2=size-1;
		i= -1;                        
		while( (i1 != i2))
		{
			i = i1+( (i2-i1)>>1 );

			if( n > data[i] )
			{
				i1 = i;
			}else
				if( n < data[i] )
				{
					i2 = i;
				}else
					if( n == data[i] )
					{
						return i;
					}
		}
		return -1;
	}
	*/

	//-------------------------------------------------------------------------------
};
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
template <class T>
class tmpRefArr : public tmpBazzArr<T>
{
protected:

	//-------------------------------------------------------------------------------
	mmVoid quickSort(T * a, mmInt N)
	{
		mmInt i = 0, j = N;
		T temp, p;
		p = a[ N>>1 ];
		do {
			while ( a[i] < p ) i++;
			while ( a[j] > p ) j--;
			if (i <= j)
			{
				temp = a[i]; a[i] = a[j]; a[j] = temp;
				i++; j--;
			}
		} while ( i<=j );
		if ( j > 0 ) quickSort(a, j);
		if ( N > i ) quickSort(a+i, N-i);
	}
	//-------------------------------------------------------------------------------

public:

	//-------------------------------------------------------------------------------
	tmpRefArr():tmpBazzArr(){}
	tmpRefArr(mmUint Size):tmpBazzArr(Size){}
	tmpRefArr(mmUint Size,mmUint Add):tmpBazzArr(Size, Add){}
	tmpRefArr(tmpRefArr& arr):tmpBazzArr(arr){}
	//-------------------------------------------------------------------------------

	mmInt Find(const T & n)
	{
		for(mmUint z = 0; z<size;z++)
			if(data[z] == n)
				return z;
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmInt FindInSorted(const T & n)
	{
		if(size == 0)
			return -1;
		if(n > data[size-1])
			return -1;
		else
			if(n < data[0])
				return -1;
			else
				if(n == data[0])
					return 0;
				else
					if(n == data[size-1])
						return (size-1);
		mmUint i1,i2,i;
		i1=0;
		i2=size-1;
		i= -1;                        
		while( (i1 != i2))
		{
			i = i1+( (i2-i1)>>1 );

			if( n > data[i] )
			{
				i1 = i;
			}else
				if( n < data[i] )
				{
					i2 = i;
				}else
					if( n == data[i] )
					{
						return i;
					}
		}
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmVoid Sort()
	{         
		if(size == 0)
			return;
		quickSort(data,size-1);
	} 
	//-------------------------------------------------------------------------------
	inline T & operator[](mmUint i)
	{
		if(i >= max_size)
			mmThrow(enExceptOutOfBounds, tmpRefArr, operator [], Index reached max size);
		if(i >= size)
			add(size-i+1);
		return data[i];
	}
	//-------------------------------------------------------------------------------
};
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
template <class T>
class tmpPtrArr : public tmpBazzArr<T>
{
protected:

	//-------------------------------------------------------------------------------
	mmVoid quickSort(T * a, mmInt N)
	{
		mmInt i = 0, j = N;
		T temp, p;
		p = a[ N>>1 ];
		do {
			while ( *a[i] < *p ) i++;
			while ( *a[j] > *p ) j--;
			if (i <= j)
			{
				temp = a[i]; a[i] = a[j]; a[j] = temp;
				i++; j--;
			}
		} while ( i<=j );
		if ( j > 0 ) quickSort(a, j);
		if ( N > i ) quickSort(a+i, N-i);
	}
	//-------------------------------------------------------------------------------

public:

	//-------------------------------------------------------------------------------
	tmpPtrArr():tmpBazzArr(){}
	tmpPtrArr(mmUint Size):tmpBazzArr(Size){}
	tmpPtrArr(mmUint Size,mmUint Add):tmpBazzArr(Size, Add){}
	tmpPtrArr(tmpPtrArr& arr):tmpBazzArr(arr){}
	//-------------------------------------------------------------------------------
	mmInt Find(const T n)
	{
		for(mmUint z = 0; z<size;z++)
			if(*data[z] == *n)
				return z;
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmInt FindPTR(const T n)
	{
		for(mmUint z = 0; z<size;z++)
			if(data[z] == n)
				return z;
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmInt FindInSorted(const T n)
	{
		if(size == 0)
			return -1;
		if(*n > *data[size-1])
			return -1;
		else
			if(*n < *data[0])
				return -1;
			else
				if(*n == *data[0])
					return 0;
				else
					if(*n == *data[size-1])
						return (size-1);
		mmUint i1,i2,i;
		i1=0;
		i2=size-1;
		i= -1;                        
		while( (i1 != i2))
		{
			i = i1+( (i2-i1)>>1 );

			if( *n > *data[i] )
			{
				i1 = i;
			}else
				if( *n < *data[i] )
				{
					i2 = i;
				}else
					if( *n == *data[i] )
					{
						return i;
					}
		}
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmInt FindInSortedPTR(const T n)
	{
		if(size == 0)
			return -1;
		if(n > data[size-1])
			return -1;
		else
			if(n < data[0])
				return -1;
			else
				if(n == data[0])
					return 0;
				else
					if(n == data[size-1])
						return (size-1);
		mmUint i1,i2,i;
		i1=0;
		i2=size-1;
		i= -1;                        
		while( (i1 != i2))
		{
			i = i1+( (i2-i1)>>1 );

			if( n > data[i] )
			{
				i1 = i;
			}else
				if( n < data[i] )
				{
					i2 = i;
				}else
					if( n == data[i] )
					{
						return i;
					}
		}
		return -1;
	}
	//-------------------------------------------------------------------------------
	mmVoid Sort()
	{         
		if(size == 0)
			return;
		quickSort(data,size-1);
	} 
	//-------------------------------------------------------------------------------
	inline T & operator[](mmUint i)
	{
		if(i >= max_size)
			mmThrow(enExceptOutOfBounds, tmpRefArr, operator [], Index reached max size);
		if(i >= size)
			add(size-i+1);
		return data[i];
	}
	//-------------------------------------------------------------------------------
};
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
typedef tmpRefArr<mmInt> clsIntArr;
typedef tmpRefArr<mmFloat> clsFloatArr;
typedef tmpRefArr<mmByte> clsByteArr;
typedef tmpRefArr<mmUint> clsmmUintArr;

#endif // _MHMARRAY_INCLUDED