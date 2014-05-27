// Genetic Algorithm for nonlinear programming  �����Թ滮���Ŵ��㷨
// Written by Microsoft Visual C++

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*��������*/

static void  initialization ( void ); 
static void  evaluation ( int gen );
static void  selection ( void );
static void  crossover ( void );
static void  mutation ( void );
static void  objective_function ( void );
static int   constraint_check ( double x[] ); 

/*�궨��*/

#define N 20  // number of variables(����)
#define M 1  // number of objectives(Ŀ�����)
#define TYPE 1 // 1=max;-1=min
#define GEN 500 // maximum generation number(�������ֵ)
#define POP_SIZE 30
#define P_MUTATION 0.2
#define P_CROSSOVER 0.3

/*ȫ�ֱ���*/
double  CHROMOSOME[POP_SIZE+1][N+1];//��ά����
double  OBJECTIVE[POP_SIZE+1][M+1];//��ά����
double  q[POP_SIZE+1];

/*��������*/
double myu ( double a, double b ) // Uniform Distribution(���ȷ���)
{
   double y;

   if ( a > b )//�������Ĳ�����������a<b����������㣬��ӡ��ʾ��Ϣ������ֱ���˳�
   {
      printf ( "\nThe first parameter should be less than the second!" );
      exit ( 1 );
   }

   y = ( double ) rand() / ( RAND_MAX );//������������������������ֵ��y

   return ( a + ( b - a ) *y ); //ʹ�ô������Ĳ��������ɵ�y����һ�£�Ȼ�󷵻�
}

static void objective_function ( void )
{
   double x1, x2, x3;
   int i;

   for ( i = 1; i <= POP_SIZE; i++ )
   {
      x1 = CHROMOSOME[i][1];
      x2 = CHROMOSOME[i][2];
      x3 = CHROMOSOME[i][3];/*Ϊ�˲��ı�����CHROMOSOME�ڵ�ֵ����Ԫ�ظ�ֵ��x1,x2,x3*/
      OBJECTIVE[i][1] = sqrt ( x1 ) + sqrt ( x2 ) + sqrt ( x3 ); /*��������CHROMOSOME�ڵ�ֵ�����ż���������ֵ������OBJECTIVEÿһ�еĵڶ���Ԫ�� */
   }

   for ( i = 1;i <= POP_SIZE;i++ )
      OBJECTIVE[i][0] = OBJECTIVE[i][1];//����OBJECTIVEÿһ�еĵڶ���Ԫ�ظ�ֵ������OBJECTIVEÿһ�еĵ�һ��Ԫ�أ���ʹ����OBJECTIVEÿһ�еĵ�һ��Ԫ�غ͵ڶ���Ԫ�����
}

static int constraint_check ( double x[] )
{
   double a = 0;
   int n;

   for ( n = 1;n <= N;n++ ) //��������x[]�����Ԫ��С��0��У�鲻ͨ����ֱ�ӷ���0
   	if ( x[n] < 0 )
		return 0;

   for (int i = 1; i <= N; i++)
	   a += x[i]*x[i]*i;
   //a = x[1] * x[1] + 2 * x[2] * x[2] + 3 * x[3] * x[3];//������Ԫ�ص�ֵ���������ʽ���㣬�����ֵ��a

   if ( a > 1 ) return 0;//���a>1��У�鲻ͨ��������0

   return 1;//У��ͨ��������1
}

static void initialization ( void )
{
   double x[N+1]; // N is the number of variables(N�ǿɱ����)
   int i, j;

   for ( i = 1; i <= POP_SIZE; i++ )/*������CHROMOSOME[][]�����ά���鵱��һ��i*j�ľ���
   								��ô���forѭ��ÿִ��һ�ξ�ֱ�ӹ�ȥһ��
   								�൱��ÿһ����һ��һά���飬������һ��Ԫ��һ��*/
   {
mark:
      for ( j = 1; j <= N; j++ ) /*���forѭ�����Ǵ���ÿһ���Ǹ�һά����
      						ÿ�ζ���������һ��Ԫ�ؽ��в���*/
	  	x[j] = myu ( 0, 1 );//���ú���myu()������ֵ���θ�ֵ��x��ÿ��Ԫ��

      if ( constraint_check ( x ) == 0 ) //����У�麯��
	  	goto mark; //�������ֵΪ0��У�鲻ͨ������������forѭ������һ�ε�ѭ������ִ��

      for ( j = 1; j <= N; j++ ) 
	  	CHROMOSOME[i][j] = x[j];/*ÿ�������һ������x��ͨ��У��
	  							��CHROMOSOME[][]����һ�о�������x��ֵ   */
   }
}

int main()//�����￪ʼ���������ǳ�������
{
   int i, j;
   double a;

   q[0] = 0.05;//����q�ĵ�һ��Ԫ�ظ���ֵ
   a = 0.05;
   /*���forѭ���Ǹ�����q��ÿ��Ԫ�ظ�ֵ
   ����Ԫ���±�Ļ���q[n] = q[n-1]+0.05*0.95^n ����n�ķ�ΧΪ[1,POP_SIZE]   */
   for ( i = 1; i <= POP_SIZE; i++ )
   {
      a = a * 0.95;
      q[i] = q[i-1] + a;
   }

   initialization();//���ó�ʼ������

   evaluation ( 0 );//������ֵ����������Ϊ0

   for ( i = 1; i <= GEN; i++ )
   {
      selection();
      crossover();
      mutation();
      evaluation ( i );
      printf ( "\nGeneration NO.%d\n", i );//�����ǰ�ǵڼ���(�����%d��ʾ��ǰλ��Ҫ�����ʾ�˸�����)
      printf ( "x=(" );

      for ( j = 1; j <= N; j++ )
      {
         if ( j < N ) printf ( "%3.4f,", CHROMOSOME[0][j] );//�����%f��ʾ��ǰλ��Ҫ�����ʾһ��������������ʾ�ĸ�ʽΪС�������4λ��Ч����
         else printf ( "%3.4f", CHROMOSOME[0][j] );
      }

      if ( M == 1 ) printf ( ")\nf=%3.4f\n", OBJECTIVE[0][1] );//�����/n��ʾ����
      else
      {
         printf ( ")\nf=(" );

         for ( j = 1; j <= M; j++ )
         {
            if ( j < M ) printf ( "%3.4f,", OBJECTIVE[0][j] );
            else printf ( "%3.4f", OBJECTIVE[0][j] );
         }

         printf ( ")  Aggregating Value=%3.4f\n", OBJECTIVE[0][0] );
      }
   }

   printf ( "\n" );

   return 1;
}

static void evaluation ( int gen )
{
   double a;
   int   i, j, k, label;
   objective_function();//���ü��㺯��

   if ( gen == 0 )/*����Ϊ0ʱ������������ִ������Ĳ���
   				�ѵڶ��е�ֵȫ�����Ƶ���һ�У�
   				���������������ǰ������һ����*/
   {
      for ( k = 0; k <= M; k++ ) OBJECTIVE[0][k] = OBJECTIVE[1][k];

      for ( j = 1; j <= N; j++ ) CHROMOSOME[0][j] = CHROMOSOME[1][j];
   }

   for ( i = 1; i < POP_SIZE; i++ )  // i is staring from 1
   {
      label = 0;
      a = OBJECTIVE[i][0]; //a��ֵΪ����OBJECTIVEÿһ�еĵ�һ��Ԫ��

      for ( j = i + 1; j <= POP_SIZE; j++ )
         if ( ( TYPE*a ) < ( TYPE*OBJECTIVE[j][0] ) )
         {
            a = OBJECTIVE[j][0];
            label = j;
         }

      if ( label != 0 )
      {
         for ( k = 0; k <= M; k++ )
         {
            a = OBJECTIVE[i][k];
            OBJECTIVE[i][k] = OBJECTIVE[label][k];
            OBJECTIVE[label][k] = a;
         }

         for ( j = 1; j <= N; j++ )
         {
            a = CHROMOSOME[i][j];
            CHROMOSOME[i][j] = CHROMOSOME[label][j];
            CHROMOSOME[label][j] = a;
         }
      }
   }

   //add the following 2 line, add i=0
   for ( k = 0; k <= M; k++ ) OBJECTIVE[0][k] = OBJECTIVE[1][k];

   for ( j = 1; j <= N; j++ ) CHROMOSOME[0][j] = CHROMOSOME[1][j];
}

static void selection()
{
   double r, temp[POP_SIZE+1][N+1];
   int   i, j, k;

   for ( i = 1; i <= POP_SIZE; i++ )
   {
      r = myu ( 0, q[POP_SIZE] );

      for ( j = 0; j <= POP_SIZE; j++ )
      {
         if ( r <= q[j] )
         {
            for ( k = 1; k <= N; k++ ) temp[i][k] = CHROMOSOME[j][k];

            break;
         }
      }
   }

   for ( i = 1; i <= POP_SIZE; i++ )
      for ( k = 1; k <= N; k++ )
         CHROMOSOME[i][k] = temp[i][k];
}

static void crossover()
{
   int   i, j, jj, k, pop;
   double r, x[N+1], y[N+1];
   pop = POP_SIZE / 2;

   for ( i = 1; i <= pop; i++ )
   {
      if ( myu ( 0, 1 ) > P_CROSSOVER ) 
		  continue;  //���myu ( 0, 1 )�Ľ������P_CROSSOVER��ֱ��ִ����һ��ѭ�������˴�ѭ�����к�������䲻��ִ��

      j = ( int ) myu ( 1, POP_SIZE );

      jj = ( int ) myu ( 1, POP_SIZE );

      r = myu ( 0, 1 );

      for ( k = 1; k <= N; k++ )
      {
         x[k] = r * CHROMOSOME[j][k] + ( 1 - r ) * CHROMOSOME[jj][k];
         y[k] = r * CHROMOSOME[jj][k] + ( 1 - r ) * CHROMOSOME[j][k];
      }

      if ( constraint_check ( x ) == 1 )
         for ( k = 1; k <= N; k++ ) CHROMOSOME[j][k] = x[k];

      if ( constraint_check ( y ) == 1 )
         for ( k = 1; k <= N; k++ ) CHROMOSOME[jj][k] = y[k];
   }
}

static void mutation ( void )
{
   int i, j, k;
   double x[N+1], y[N+1], infty, direction[N+1];
   double INFTY = 10, precision = 0.0001;

   for ( i = 1; i <= POP_SIZE; i++ )
   {
      if ( myu ( 0, 1 ) > P_MUTATION ) continue;

      for ( k = 1; k <= N; k++ ) x[k] = CHROMOSOME[i][k];

      for ( k = 1; k <= N; k++ )
         if ( myu ( 0, 1 ) < 0.5 ) direction[k] = myu ( -1, 1 );
         else direction[k] = 0;

      infty = myu ( 0, INFTY );

      while ( infty > precision )
      {
         for ( j = 1; j <= N; j++ ) y[j] = x[j] + infty * direction[j];

         if ( constraint_check ( y ) == 1 )
         {
            for ( k = 1; k <= N; k++ ) CHROMOSOME[i][k] = y[k];

            break;
         }

         infty = myu ( 0, infty );
      }
   }
}
