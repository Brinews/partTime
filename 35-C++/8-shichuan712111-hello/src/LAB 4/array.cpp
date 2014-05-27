
#ifndef   ARRAY_CLASS 
#define   ARRAY_CLASS 

#include   <iostream.h> 
#include   <stdlib.h> 
#include   <string.h> 

#ifndef   NULL 
const   int   NULL   =   0; 
#endif     //   NULL 

enum   ErrorType 
{invalidArraySize,   memoryAllocationError,   indexOutOfRange}; 

char   *errorMsg[]   = 
{ 
	"Invalid   array   size ",   "Memory   allocation   error ", 
	"Invalid   index:   " 
}; 

template   <class   T>   
class   Array 
{ 
	private: 
		//   a   dynamically   allocated   list   containing   size   items 
		//һ����̬����İ���size��Ԫ�صı� 
		T*     alist; 
		int   size; 

		//   error   handling   method 
		//�������� 
		void   Error(ErrorType   error,int   badIndex=0)   const; 

	public: 
		//   constructors   and   destructor 
		Array(int   sz   =   1); 
		//Array(const   Array <T> &   A); 
		~Array(void); 

		//   assignment,   indexing   and   pointer   conversion 
		//��ֵ,�±�,��ָ��ת������ 
		Array <T> &   operator=   (const   Array <T> &   rhs); 
		T&   operator[](int   i); 
		operator   T*   (void)   const; 

		//   size   operations 
		//�йر��С�ĺ��� 
		int   ListSize(void)   const;       //   read   the   size 
		void   Resize(int   sz);                 //   modify   the   size 

}; 


//   prints   the   message   corresponding   to   error 
template   <class   T> 
void   Array <T> ::Error(ErrorType   error,   int   badIndex)   const 
{ 
	cerr   < <   errorMsg[error]; 
	//   for   indexOutOfRange,   print   the   bad   index 
	if   (error   ==   indexOutOfRange) 
		cerr   < <   badIndex; 
	cerr   < <   endl; 
	exit(1); 
} 

//   constructor 
	template   <class   T> 
Array <T> ::Array(int   sz) 
{ 
	//   check   for   an   invalid   size   parameter 
	//��������С�����Ƿ�Ϸ� 
	if   (sz   <=0) 
		Error(invalidArraySize); 
	//   assign   the   size   and   dynamically   allocate   memory 
	//��size��ֵ����̬�����ڴ� 
	size   =   sz; 
	alist   =   new   T[size];         
	//   make   sure   that   system   allocates   the   desired   memory,   
	//ȷ��ϵͳ������������ڴ� 
	if   (alist   ==   NULL) 
		Error(memoryAllocationError); 
} 

//   destructor 
	template   <class   T> 
Array <T> ::~Array(void) 
{   
	delete   []   alist; 
} 

//   copy   constructor 
//���ƹ��캯�� 
/*template   <class   T> 
  Array <T> ::Array(const   Array <T> &   X) 
  { 
//   get   size   from   object   X   and   assign   to   current   object 
//ȡ�ö���X�Ĵ�С�����丳ֵ����ǰ���� 
int   n   =   X.size; 

size   =   n; 

//   allocate   new   memory   for   object   and   do   error   checking 
//Ϊ�����������ڴ沢���г����� 
alist   =   new   T[n];                       //   allocate   dynamic   array 
if   (alist   ==   NULL) 
Error(memoryAllocationError); 

//   copy   array   items   from   x   to   current   object 
//��X�п�������Ԫ�ص���ǰ���� 
T*   srcptr   =   X.alist;         //   address   at   start   of   X.alist 
T*   destptr   =   alist;           //   address   at   start   of   alist 
while   (n--)                           //   copy   list 
 *destptr++   =   *srcptr++; 
 }*/ 

//   assignment   operator.   assign   rhs   to   the   current   object 
//��ֵrhs����ǰ���� 
	template   <class   T> 
Array <T> &   Array <T> ::operator=   (const   Array <T> &   rhs) 
{ 
	//   record   the   size   of   rhs��¼rhs�Ĵ�С 
	int   n   =   rhs.size; 
	//   if   sizes   not   the   same,   delete   memory   and   reallocate 
	//�����С����ͬ��ɾ�������·����ڴ� 
	if   (size   !=   n) 
	{ 
		delete   []   alist;                 //   destroy   original   memory 
		alist   =   new   T[n];               //   allocate   a   new   array 
		if   (alist   ==   NULL) 
			Error(memoryAllocationError); 
		size   =   n; 
	} 
	//   copy   array   items   from   rhs   to   current   object 
	//   ����rhs����ǰ���� 
	T*   destptr   =   alist;//T*Ϊ��������������������׵�ַ 
	T*   srcptr   =   rhs.alist; 
	while   (n--) 
		*destptr++   =   *srcptr++; 
	//   return   reference   to   the   current   object 
	return   *this; 
} 

//   overloaded   index   operator 
//�����±������ 
	template   <class   T> 
T&   Array <T> ::operator[](int   n) 
{ 
	//   do   array   bounds   checking 
	//����Խ���� 
	if   (n   <   0   ||   n   >   size-1) 
		Error(indexOutOfRange,n); 
	//   return   the   element   from   the   private   array   list 
	//��˽�������з���Ԫ��ֵ 
	return   alist[n]; 
} 

//   pointer   conversion   operator 
template   <class   T> 
Array <T> ::operator   T*   (void)   const 
{ 
	//   return   address   of   private   array   in   the   current   object 
	//���ص�ǰ������˽��������׵�ַ 
	return   alist; 
} 

template   <class   T> 
int   Array <T> ::ListSize(void)   const 
{ 
	return   size; 
} 

//   resize   operator 
	template   <class   T> 
void   Array <T> ::Resize(int   sz) 
{ 
	//   test   new   size   parameter;   terminate   if   size   <=   0 
	//����µĴ�С����;����С�ڵ���0,���˳����� 
	if   (sz   <=0) 
		Error(invalidArraySize); 
	//   nothing   to   do   if   size   hasn 't   changed 
	//����С����,��򵥷��� 
	if   (sz   ==   size) 
		return; 

	//   request   new   memory   and   verify   system   response 
	//�������µ��ڴ�;ȷ��ϵͳ�ѷ��������ڴ� 
	T*   newlist   =   new   T[sz]; 
	if   (newlist   ==   NULL) 
		Error(memoryAllocationError); 

	//   declare   n   with   value   sz   (truncating   list) 
	//   or   otherwise   declare   n   to   be   the   current   size 
	//nΪ�追��Ԫ�صĸ��� 
	int   n   =   (sz   <=   size)   ?   sz   :   size; 

	//   copy   n   array   items   from   old   to   new   memory 
	//�Ӿɱ��п���n������Ԫ�ص��±� 
	T*   srcptr   =   alist;             //   alist���׵�ַaddress   at   start   of   alist 
	T*   destptr   =   newlist;       //   newlist�׵�ַaddress   at   start   of   newlist 
	while   (n--)                           //   ������Ԫ��copy   list 
		*destptr++   =   *srcptr++; 

	//   delete   old   listɾ���ɱ� 
	delete[]   alist; 

	//   reset   alist   to   point   at   newlist   and   update   the   size 
	//��alistָ��ָ��newlist���ı��Сֵ 
	alist   =   newlist; 
	size   =   sz; 
} 

#endif     //   ARRAY_CLASS 

ʵ���ж����磺Array <char   *>   abcde; 

