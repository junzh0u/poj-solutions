#include <iostream.h>

typedef struct NODE
{
	  int nodeNo;
	  NODE * next;
}NODE;

class ROPE{
public:
    NODE *begin;
	ROPE(int[], int size);
	NODE *findNode(int n);
	NODE *preNode(int n);
	bool deleteNode(int n);
    bool isEmpty();
	bool isSelfKnoted();
	bool deleteSelfNotedNodes();
	bool isPairKnoted(int &,int &,int &);
    bool exchangeRopePeriodsOfDifferentDirection(int b1, int e1);
    bool exchangeRopePeriodsOfSameDirection(int b1, int e1);
};

bool ROPE::isSelfKnoted(){
	NODE * tmp = begin;
	while(tmp->next != begin)
	{
        int no = tmp->nodeNo;
	    NODE *tmp2 = tmp->next;
		int sign = tmp2->nodeNo;
		while(tmp2->nodeNo != -no && (sign * tmp2->nodeNo>0))
			tmp2 = tmp2->next;
		if(tmp2->nodeNo == -no)
		{
			begin = tmp;
			return true;
		}
		tmp = tmp->next;
	}
	if(tmp->nodeNo == - begin->nodeNo)
	{
		begin = tmp;
		return true;
	}else return false;
}

bool ROPE::deleteSelfNotedNodes(){
     int no = begin->nodeNo;
     int nodes[100];
     int s=0;
     NODE *tmp=begin;
     do{
	     nodes[s++] = tmp->nodeNo;
	     tmp = tmp->next;
     }while(tmp->nodeNo != -no);

     for(int i=0; i<s; i++)
	     deleteNode(nodes[i]);
     for(i=0; i<s; i++)
	     deleteNode(-nodes[i]);
     return true;
}

bool ROPE::isPairKnoted(int &b1, int &e1, int &dir){
    NODE *tmp = begin;
    while(tmp->next != begin){
             b1=tmp->nodeNo;
		NODE * tmp1, *tmp2;
		tmp2 = tmp->next;
		while((tmp->nodeNo * tmp2->nodeNo < 0) && tmp2!=tmp) 
                     tmp2 = tmp2->next;
		if(tmp2 != tmp) e1=tmp2->nodeNo;
		tmp1 = findNode(-b1);      tmp2 = findNode(-e1);
		while(tmp1->nodeNo * tmp2->nodeNo >0 && tmp1!=tmp2) 
                    tmp1 = tmp1->next;
		if(tmp1 == tmp2) { dir = +1; return true;	}
		tmp1 = findNode(-e1);      tmp2 = findNode(-b1);
		while(tmp1->nodeNo * tmp2->nodeNo>0 && tmp1!=tmp2) 
                     tmp1 = tmp1->next;
		if(tmp1 == tmp2){ dir = -1; return true; }
		tmp = tmp->next;
	}	return false;
}

bool ROPE::exchangeRopePeriodsOfDifferentDirection(int b1, int e1)
{
	NODE *begin1 = findNode(b1);
	NODE *end1 = findNode(e1);
	NODE *begin2 = findNode(-e1);
	NODE *end2 = findNode(-b1);
	NODE *afterBegin1, *beforeEnd1, *afterBegin2, *beforeEnd2;

    if(begin2->next == end2) 
		 afterBegin1 = beforeEnd1 =NULL;
    else{
		 NODE *t1, *t2;

		 beforeEnd1 = begin2->next;
		 afterBegin1 = beforeEnd1;
		 t1 = afterBegin1->next;
		 t2 = t1->next;
		 while(t1 != end2){
			 t1->next = afterBegin1; 
			 afterBegin1 = t1;
			 t1 = t2;
			 t2 = t2->next;
		 }
    }
	if(begin1->next == end1) 
		 afterBegin2 = beforeEnd2 =NULL;
    else{
		 NODE *t1, *t2;

		 beforeEnd2 = begin1->next;
		 afterBegin2 = beforeEnd2;
		 t1 = afterBegin2->next;
		 t2 = t1->next;
		 while(t1 != end1){
			 t1->next = afterBegin2; 
			 afterBegin2 = t1;
			 t1 = t2;
			 t2 = t2->next;
		 }
    }
	if(afterBegin1 != NULL){
		begin1->next = afterBegin1;
	    beforeEnd1->next = end1;
	}else begin1->next = end1;

	if(afterBegin2 != NULL){
		begin2->next = afterBegin2;
	    beforeEnd2->next = end2;
	}else begin2->next = end2;
	
	deleteNode(b1);
	deleteNode(-b1);
	deleteNode(e1);
	deleteNode(-e1);
	return true;
}

bool ROPE::exchangeRopePeriodsOfSameDirection(int b1, int e1)
{
	NODE *begin1 = findNode(b1);
	NODE *end1 = findNode(e1);
	NODE *begin2 = findNode(-b1);
	NODE *end2 = findNode(-e1);
    
    NODE *afterBegin1, *beforeEnd1, *afterBegin2, *beforeEnd2;

    if(begin2->next == end2) 
		 afterBegin1 = beforeEnd1 =NULL;
    else{
		 afterBegin1 = begin2->next;
		 beforeEnd1 = preNode(end2->nodeNo);
	}
	if(begin1->next == end1) 
		 afterBegin2 = beforeEnd2 = NULL;
	else{
		 afterBegin2 = begin1->next;
		 beforeEnd2 = preNode(end1->nodeNo);
	}
	if(afterBegin1 != NULL){
		begin1->next = afterBegin1;
	    beforeEnd1->next = end1;
	}else begin1->next = end1;

	if(afterBegin2 != NULL){
		begin2->next = afterBegin2;
	    beforeEnd2->next = end2;
	}else begin2->next = end2;
	
	deleteNode(b1);
	deleteNode(-b1);
	deleteNode(e1);
	deleteNode(-e1);
	return true;
}

ROPE::ROPE( int * zz, int size )
{
	NODE * tmp, *tmp2;
	begin = new NODE;
	begin->nodeNo = zz[0];
	tmp = begin;
	for ( int i = 1 ; i < size ; i ++ )
	{
		tmp2 = new NODE;
		tmp2->nodeNo = zz[ i ];
		tmp -> next = tmp2;
		tmp = tmp2;
	}
	tmp -> next = begin;
}

NODE * ROPE::findNode(int n)
{
	NODE * tmp;
	tmp = begin;
	while( tmp->nodeNo != n )
	{
		tmp = tmp->next;
	}
	return tmp;
}

NODE * ROPE::preNode(int n)
{
	NODE * tmp;
	tmp = begin;
	while( tmp->next->nodeNo != n )
	{
		tmp = tmp->next ;
	}
	return tmp;
}

bool ROPE::deleteNode(int n)
{
	if( begin->next == begin )
	{
		delete begin;
		begin = NULL;
	}
	else
	{
		NODE* tmp = findNode(n);
		NODE* tmp2 = preNode(n);
		if( tmp == begin ) 
		{
			tmp2->next = tmp->next;
			begin = tmp->next;
		}
		else tmp2->next = tmp->next;
		delete tmp;
	}
	return true;
}

bool ROPE::isEmpty()
{
	if( begin == NULL )
		return true;
	else return false;
}

int numbers[100];
 
void main()
{
	int n,i,size;
	cin >> n;
	while(n--)
	{
		i = 0;
		do
		{
			cin >> numbers[ i ];
			i ++ ;
		}
		while( numbers[ i - 1 ] != 0 );
		size = i - 1;
        ROPE *aRope = new ROPE(numbers,size);
        while(!aRope->isEmpty())
		{
        	if(aRope->isSelfKnoted())
        		aRope->deleteSelfNotedNodes();
         	else
			{
	               int b1, e1, dir;
        	       if(aRope->isPairKnoted(b1,e1,dir)){ 
        		    if(dir == 1) 
                               aRope->exchangeRopePeriodsOfSameDirection(b1,e1);
	        	    else 
                          aRope->exchangeRopePeriodsOfDifferentDirection(b1,e1);
				   }else break;
			}
		}
        if(aRope->isEmpty()) 	  cout << "Right" << endl;
        else  cout << "Not right" << endl;
        delete aRope;
		for( i = 0 ; i < 100 ; i ++ )
			numbers[ i ] = 0 ;
	}
}
