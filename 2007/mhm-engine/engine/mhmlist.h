//==========================================================================================
// Mayhem engine dynamic list template
// Coded:	MozG
// Created:	12 may 2005
//==========================================================================================

#ifndef _MHMLIST_INCLUDED
#define _MHMLIST_INCLUDED

// Error codes
#define MHMLIST_FAIL			mmOk + 1
#define MHMLIST_OUT_OF_RANGE	mmOk + 2

template <class type>
class tmpPtrList
{
//===========================
// Iternal list node class //
//===========================
protected:
	class clsListNode
	{
	public:
		clsListNode*		m_next;
		clsListNode*		m_prev;
		type				m_data;
				
	public:
		clsListNode(clsListNode* next, clsListNode* prev, type data)
		{
			m_next = next;
			m_prev = prev;
			m_data = data;
		}
	   ~clsListNode()
	    {
			if (m_next != mmnull) m_next->m_prev = m_prev;
			if (m_prev != mmnull) m_prev->m_next = m_next;
	    }
	};

public:
//=============================
// Iternal list cursor class //
//=============================
	class clsCursor
	{
	private:
		clsListNode* m_node;
		tmpPtrList*	 m_list;
		
	public:
		clsCursor(clsListNode* node, tmpPtrList* list)
		{
			m_node = node;
			m_list = list;
		}
		
		clsCursor(tmpPtrList* list)
		{
			m_node = mmnull;
			m_list = list;
		}
	   
	   ~clsCursor()
	    {
			 
	    }
	    
	    operator type()
	    {
			return m_node->m_data;
	    }
	    
	    type operator = (type data)
	    {
			if (_node == mmnull)
			{
				m_list->Add(data);
				return data;
			}
			if (m_list->m_destruct)
				mmSafeDelete(m_node->m_data);
			m_node->m_data = data;
			return data;
	    }
	    
	    type& operator ->()
	    {
			return m_node->m_data;
	    }
	
	};

protected:
	clsListNode*	m_head;
	mmUint			m_count;
	mmBool			m_destruct;
	mmDword			m_id;
		
public:
	tmpPtrList(mmBool destruct = mmtrue, mmDword id = 0)
	{
		m_head     = mmnull;
		m_count    = 0;
		m_destruct = destruct;
		m_id	   = id;
	}
	
    virtual ~tmpPtrList()
    {
		Clear();    
    }
    
    mmVoid Add(type data)
    {
		if (m_head == mmnull)
		{
			m_head = new clsListNode(mmnull, mmnull, data);
			m_count++;
			return;
		}
		clsListNode* ptr = m_head;
		while(ptr->m_next != mmnull)
			ptr = ptr->m_next;
		ptr->m_next = new clsListNode(mmnull, ptr, data);
		m_count++;
    }
    
    mmRes Insert(type data, mmUint index)
    {
    	if ((m_head == mmnull) && (index != 0))
			return MHMLIST_OUT_OF_RANGE;
		if (m_head == mmnull)
		{
			m_head = new clsListNode(mmnull, mmnull, data);
			m_count++;
			return;
		}
		if (index == 0)
		{
			m_head = new clsListNode(m_head, mmnull, data);
			return;
		}
		
		if (index > _count)
			return MHMLIST_OUT_OF_RANGE;
			
		mmUint count = 0;
		clsListNode* ptr = _head;
		while((ptr->m_next != mmnull) && (count != index))
		{
			ptr = ptr->m_next;
			count++;
		}
		ptr->m_prev = new clsListNode(ptr, ptr->m_prev, data);
		m_count++;
    }
    
    mmVoid Clear()
    {
		if (m_head == mmnull) return;
		clsListNode* ptr = m_head;
		while(ptr->m_next != mmnull)
		{
			if ((ptr->m_prev != mmnull) && m_destruct) 
				mmSafeDelete(ptr->m_prev->m_data);
			mmSafeDelete(ptr->m_prev);
			ptr = ptr->m_next;
		}
		if ((ptr->m_prev != mmnull) && m_destruct)
			mmSafeDelete(ptr->m_prev->m_data);
		mmSafeDelete(ptr->m_prev);
		if (m_destruct)
			mmSafeDelete(ptr->m_data);
		mmSafeDelete(ptr);
		m_head  = mmnull;
		m_count = 0;
    }
    
    clsCursor operator[] (mmUint index)
    {
    	if ((m_head == mmnull) && (index > 0))
			mmThrow(enExceptOutOfBounds, tmpPtrList, operator [], No list items present);
		if (m_head == mmnull)
			return clsCursor(this);
		if (index >= m_count)
			mmThrow(enExceptOutOfBounds, tmpPtrList, operator [], Index exceeded list length);
		clsListNode* ptr = m_head;
		mmUint count = 0;
		while((ptr->m_next != mmnull) && (count != index))
		{
			ptr = ptr->m_next;
			count++;
		}
		return clsCursor(ptr, this);
    }
    
    mmUint Count()
    {
		return m_count;
    }
    
	mmVoid Remove(type data)
    {
		if (m_head == mmnull) 
			return;
		clsListNode* ptr = m_head;
		while((ptr->m_next != mmnull) && (ptr->m_data != data)) 
			ptr = ptr->m_next;
		if (ptr->m_data == data)
		{
			if (ptr == m_head)
				m_head = m_head->m_next;
			mmSafeDelete(ptr);
			m_count--;
			if (m_count == 0)
				m_head = mmnull;
		}
    } 
    
//    mmVoid Sort()
//  {
		
//    }
    
//    virtual mmInt Compare(type A, type B)
//    {
//		return 0;
//    }
};


/*
//==========================================================================================
// Objects list
//==========================================================================================
template <class type>
class tmpList
{
//===========================
// Iternal list node class //
//===========================
private:
	class clsListNode
	{
	public:
		clsListNode*		m_next;
		clsListNode*		m_prev;
		type				m_data;
				
	public:
		clsListNode(clsListNode* next, clsListNode* prev, type data)
		{
			m_next = next;
			m_prev = prev;
			m_data = data;
		}
	   ~clsListNode()
	    {
			if (m_next != mmnull) m_next->m_prev = m_prev;
			if (m_prev != mmnull) m_prev->m_next = m_next;
	    }
	};

public:
//=============================
// Iternal list cursor class //
//=============================
	class clsCursor
	{
	private:
		clsListNode* m_node;
		tmpList*	 m_list;
		
	public:
		clsCursor(clsListNode* node, tmpList* list)
		{
			m_node = node;
			m_list = list;
		}
		
		clsCursor(tmpList* list)
		{
			m_node = mmnull;
			m_list = list;
		}
	   
	   ~clsCursor()
	    {
			 
	    }
	    
//	    operator type&()
//	    {
//			return m_node->m_data;
//	    }
	    
	    operator type&()
	    {
	    	return m_node->m_data;
	    }
	    
	    type operator = (type data)
	    {
			if (_node == mmnull)
			{
				m_list->Add(data);
				return data;
			}
			m_node->m_data = data;
			return data;
	    }
	    
	    type& operator ->()
	    {
			return m_node->m_data;
	    }
	
	};

private:
	clsListNode*	m_head;
	mmUint			m_count;
	

public:
	tmpList()
	{
		m_head     = mmnull;
		m_count    = 0;
	}
	
   ~tmpList()
    {
		Clear();    
    }
    
    mmVoid Add(type data)
    {
		if (m_head == mmnull)
		{
			m_head = new clsListNode(mmnull, mmnull, data);
			m_count++;
			return;
		}
		clsListNode* ptr = m_head;
		while(ptr->m_next != mmnull)
			ptr = ptr->m_next;
		ptr->m_next = new clsListNode(mmnull, ptr, data);
		m_count++;
    }
    
    mmRes Insert(type data, mmUint index)
    {
    	if ((m_head == mmnull) && (index != 0))
			return MHMLIST_OUT_OF_RANGE;
		if (m_head == mmnull)
		{
			m_head = new clsListNode(mmnull, mmnull, data);
			m_count++;
			return;
		}
		if (index == 0)
		{
			m_head = new clsListNode(m_head, mmnull, data);
			return;
		}
		
		if (index > _count)
			return MHMLIST_OUT_OF_RANGE;
			
		mmUint count = 0;
		clsListNode* ptr = _head;
		while((ptr->m_next != mmnull) && (count != index))
		{
			ptr = ptr->m_next;
			count++;
		}
		ptr->m_prev = new clsListNode(ptr, ptr->m_prev, data);
		m_count++;
    }
    
    mmVoid Clear()
    {
		if (m_head == mmnull) return;
		clsListNode* ptr = m_head;
		while(ptr->m_next != mmnull)
		{
			mmSafeDelete(ptr->m_prev);
			ptr = ptr->m_next;
		}
		mmSafeDelete(ptr->m_prev);
		mmSafeDelete(ptr);
		m_head  = mmnull;
		m_count = 0;
    }
    
    clsCursor operator[] (mmUint index)
    {
    	if ((m_head == mmnull) && (index > 0))
			throw "Out of range";
		if (m_head == mmnull)
			return clsCursor(this);
		if (index >= m_count)
			throw "Out of range";
		clsListNode* ptr = m_head;
		mmUint count = 0;
		while((ptr->m_next != mmnull) && (count != index))
		{
			ptr = ptr->m_next;
			count++;
		}
		return clsCursor(ptr, this);
    }
    
    mmUint Count()
    {
		return m_count;
    }
    
	mmVoid Remove(type data)
    {
		if (m_head == mmnull) 
			return;
		clsListNode* ptr = _head;
		while((ptr->m_next != mmnull) && (ptr->m_data != data)) 
			ptr = ptr->m_next;
		if (ptr->m_data == data)
		{
			if (ptr == m_head)
				m_head = m_head->m_next;
			mmSafeDelete(ptr);
			m_count--;
			if (m_count == 0)
				m_head = mmnull;
		}
    } 
};



*/





template <class T>
class tmpList
{
protected:
	class clsListNode
	{
	protected:
	public:
		clsListNode*		prev;
		clsListNode*		next;
		T					data;
		clsListNode(clsListNode* Prev, clsListNode* Next)
		{
			next = Next; prev = Prev;
			if(Prev != mmnull) Prev->next = this;
			if(Next != mmnull) Next->prev = this;
		}
		clsListNode(clsListNode* Prev, clsListNode* Next, const T & Data)
		{
			next = Next; prev = Prev;
			data = *((T*)(&Data));
			if(Prev != mmnull) Prev->next = this;
			if(Next != mmnull) Next->prev = this;
		}
		~clsListNode()
		{
			if (next != mmnull) next->prev = prev;
			if (prev != mmnull) prev->next = next;
		}
	};

	clsListNode * start;
	clsListNode * end;
	clsListNode * current;

	mmUint count;

	mmInt selectedID;

public:

	tmpList()
	{
		start = mmnull;
		end = mmnull;
		current = mmnull;
		count = 0;
	}
	~tmpList()
	{
		Clear();    
	}
	//-------------------------------------------------------------------
	mmVoid Add(const T & data)
	{
		count++;
		end = current = new clsListNode(end, mmnull, data);
		if (start == mmnull)
			start = end;
	}
	//-------------------------------------------------------------------
	mmVoid Add()
	{
		count++;
		end = current = new clsListNode(end, mmnull);
		if (start == mmnull)
			start = end;
	}
	//-------------------------------------------------------------------
	mmVoid AddBack(const T & data)
	{
		Add(T data);
	}
	//-------------------------------------------------------------------
	mmVoid AddFront(const T & data)
	{
		count++;
		start = current = new clsListNode(mmnull, start, data);
		if (end == mmnull)
			end = start;
	}
	//-------------------------------------------------------------------
	mmVoid Insert(const T & data, mmUint index)
	{
		if(index == 0)
		{
			AddFront(data);
			return;
		}
		if(index >= count)
		{
			Add(data);
			return;
		}
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(i == index)
			{
				current = new clsListNode(n->prev, n, data);
				count++;
				return;
			}
			i++;
		}
	}
	//-------------------------------------------------------------------
	mmVoid Clear()
	{
		if (start == mmnull) return;
		clsListNode * n;
		for(n = start->next; n != mmnull; n = n->next)
			delete n->prev;
		delete n;
		start = mmnull;
		end = mmnull;
		current = mmnull;
		count = 0;
	}
	//-------------------------------------------------------------------
	mmUint Count()
	{
		return count;
	}
	//-------------------------------------------------------------------
	mmVoid Remove(mmUint index)
	{
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(i == index)
			{
				if(n == start) start = n->next;
				if(n == end) end = n->prev;
				if(n == current) current = mmnull;
				delete n;
				count--;
				return;
			}
			i++;
		}
	}
	//-------------------------------------------------------------------
	mmInt Find(const T & data)
	{
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(data == n->data)
				return i;
			i++;
		}
		return -1;
	}
	//-------------------------------------------------------------------
	mmVoid Move(mmUint i1, mmUint i2)
	{
		if((i1 >= count)||(i2 >= count))
		{
			mmThrow(enExceptOutOfBounds, tmpList, Move, One of items has invalid index);
			return;
		}
		T d;
		d = (*this)[i1];
		(*this)[i1] = (*this)[i2];
		(*this)[i1] = d;
	}
	//-------------------------------------------------------------------
	mmVoid Sort()
	{
		T d;
		for(clsListNode * z = start; z != mmnull; z = z->next)
			for(clsListNode * x = z->next; x != mmnull; x = x->next)
				if( z->dat > x->dat )
				{
					d = z->dat;
					z->dat = x->dat;
					x->dat = d;
				}
	}
	//-------------------------------------------------------------------
	T & operator[] (mmUint index)
	{
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(i == index)
			{
				return n->data;
			}
			i++;
		}
		mmThrow(enExceptOutOfBounds, tmpList, operator[], Index exceeds list length);
	}
	//-------------------------------------------------------------------
	mmVoid ResetSelect()
	{
		current = mmnull;
		selectedID = -1;
	}
	//-------------------------------------------------------------------
	mmBool SelectNext()
	{
		if(count == 0)
		{
			ResetSelect();
			return mmfalse;
		}
		if(current == mmnull)
		{
			selectedID = 0;
			current = start;
			return mmtrue;
		}
		current = current->next;
		if(current == mmnull)
		{
			selectedID = -1;
			return mmfalse;
		}
		selectedID++;
		return mmtrue;
	}
	//-------------------------------------------------------------------
	mmBool SelectPrev()
	{
		if(count == 0)
		{
			ResetSelect();
			return mmfalse;
		}
		if(current == mmnull)
		{
			selectedID = count-1;
			current = end;
			return mmtrue;
		}
		current = current->prev;
		if(current == mmnull)
		{
			selectedID = -1;
			return mmfalse;
		}
		selectedID--;
		return mmtrue;
	}
	//-------------------------------------------------------------------
	T & Selected()
	{
		if(current == mmnull)
		{
			mmThrow(enExceptOutOfBounds, tmpList, Selected, List has no selected items);
		}
		return current->data;
	}
	//-------------------------------------------------------------------
	T & First()
	{
		if(start == mmnull)
		{
			mmThrow(enExceptOutOfBounds, tmpList, First, List has no start item);
		}
		return start->data;
	}
	//-------------------------------------------------------------------
	T & Last()
	{
		if(end == mmnull)
		{
			mmThrow(enExceptOutOfBounds, tmpList, Last, List has no last item);
		}
		return end->data;
	}
	//-------------------------------------------------------------------
	mmVoid Selected(const T & data)
	{
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(n->data == data)
			{
				current = n;
				selectedID = i;
				return;
			}
			i++;
		}
		mmThrow(enExceptOutOfBounds, tmpList, Selected, List has no such data item);
	}
	//-------------------------------------------------------------------
	mmInt SelectedID()
	{
		return selectedID;
	}
	//-------------------------------------------------------------------
	mmVoid SelectedID(mmUint index)
	{
		mmUint i = 0;
		for(clsListNode * n = start; n != mmnull; n = n->next)
		{
			if(i == index)
			{
				current = n;
				selectedID = i;
				return;
			}
			i++;
		}
		mmThrow(enExceptOutOfBounds, tmpList, Selected, Index exceeds list length);
	}
	//-------------------------------------------------------------------



};

#endif // _MHMLIST_INCLUDED