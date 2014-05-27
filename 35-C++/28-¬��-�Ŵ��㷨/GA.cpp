// Genetic Algorithm for nonlinear programming  非线性规划的遗传算法
// Written by Microsoft Visual C++

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*函数声明*/

static void  initialization ( void ); 
static void  evaluation ( int gen );
static void  selection ( void );
static void  crossover ( void );
static void  mutation ( void );
static void  objective_function ( void );
static int   constraint_check ( double x[] ); 

/*宏定义*/

#define N 20  // number of variables(变量)
#define M 1  // number of objectives(目标变量)
#define TYPE 1 // 1=max;-1=min
#define GEN 500 // maximum generation number(代数最大值)
#define POP_SIZE 30
#define P_MUTATION 0.2
#define P_CROSSOVER 0.3

/*全局变量*/
double  CHROMOSOME[POP_SIZE+1][N+1];//二维数组
double  OBJECTIVE[POP_SIZE+1][M+1];//二维数组
double  q[POP_SIZE+1];

/*函数定义*/
double myu ( double a, double b ) // Uniform Distribution(均匀分配)
{
   double y;

   if ( a > b )//传进来的参数必须满足a<b，如果不满足，打印提示信息，函数直接退出
   {
      printf ( "\nThe first parameter should be less than the second!" );
      exit ( 1 );
   }

   y = ( double ) rand() / ( RAND_MAX );//调用随机函数，生成随机数赋值给y

   return ( a + ( b - a ) *y ); //使用传进来的参数和生成的y计算一下，然后返回
}

static void objective_function ( void )
{
   double x1, x2, x3;
   int i;

   for ( i = 1; i <= POP_SIZE; i++ )
   {
      x1 = CHROMOSOME[i][1];
      x2 = CHROMOSOME[i][2];
      x3 = CHROMOSOME[i][3];/*为了不改变数组CHROMOSOME内的值，将元素赋值给x1,x2,x3*/
      OBJECTIVE[i][1] = sqrt ( x1 ) + sqrt ( x2 ) + sqrt ( x3 ); /*根据数组CHROMOSOME内的值开根号计算出结果赋值给数组OBJECTIVE每一行的第二个元素 */
   }

   for ( i = 1;i <= POP_SIZE;i++ )
      OBJECTIVE[i][0] = OBJECTIVE[i][1];//数组OBJECTIVE每一行的第二个元素赋值给数组OBJECTIVE每一行的第一个元素，致使数组OBJECTIVE每一行的第一个元素和第二个元素相等
}

static int constraint_check ( double x[] )
{
   double a = 0;
   int n;

   for ( n = 1;n <= N;n++ ) //遍历数组x[]如果有元素小于0则校验不通过，直接返回0
   	if ( x[n] < 0 )
		return 0;

   for (int i = 1; i <= N; i++)
	   a += x[i]*x[i]*i;
   //a = x[1] * x[1] + 2 * x[2] * x[2] + 3 * x[3] * x[3];//数组内元素的值根据这个公式计算，结果赋值给a

   if ( a > 1 ) return 0;//如果a>1则校验不通过，返回0

   return 1;//校验通过，返回1
}

static void initialization ( void )
{
   double x[N+1]; // N is the number of variables(N是可变的量)
   int i, j;

   for ( i = 1; i <= POP_SIZE; i++ )/*如果你把CHROMOSOME[][]这个二维数组当成一个i*j的矩阵
   								那么这个for循环每执行一次就直接过去一行
   								相当于每一行是一个一维数组，被当成一个元素一样*/
   {
mark:
      for ( j = 1; j <= N; j++ ) /*这个for循环就是代表每一行那个一维数组
      						每次对这个数组的一个元素进行操作*/
	  	x[j] = myu ( 0, 1 );//调用函数myu()，返回值依次赋值给x的每个元素

      if ( constraint_check ( x ) == 0 ) //调用校验函数
	  	goto mark; //如果返回值为0则校验不通过，则最外层的for循环的这一次的循环重新执行

      for ( j = 1; j <= N; j++ ) 
	  	CHROMOSOME[i][j] = x[j];/*每随机生成一个数组x并通过校验
	  							则CHROMOSOME[][]的那一行就是数组x的值   */
   }
}

int main()//从这里开始看，这里是程序的起点
{
   int i, j;
   double a;

   q[0] = 0.05;//数组q的第一个元素赋初值
   a = 0.05;
   /*这个for循环是给数组q的每个元素赋值
   按照元素下标的话是q[n] = q[n-1]+0.05*0.95^n 其中n的范围为[1,POP_SIZE]   */
   for ( i = 1; i <= POP_SIZE; i++ )
   {
      a = a * 0.95;
      q[i] = q[i-1] + a;
   }

   initialization();//调用初始化函数

   evaluation ( 0 );//调用求值函数，参数为0

   for ( i = 1; i <= GEN; i++ )
   {
      selection();
      crossover();
      mutation();
      evaluation ( i );
      printf ( "\nGeneration NO.%d\n", i );//输出当前是第几代(这里的%d表示当前位置要输出显示了个整数)
      printf ( "x=(" );

      for ( j = 1; j <= N; j++ )
      {
         if ( j < N ) printf ( "%3.4f,", CHROMOSOME[0][j] );//这里的%f表示当前位置要输出显示一个浮点数，且显示的格式为小数点后保留4位有效数字
         else printf ( "%3.4f", CHROMOSOME[0][j] );
      }

      if ( M == 1 ) printf ( ")\nf=%3.4f\n", OBJECTIVE[0][1] );//这里的/n表示换行
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
   objective_function();//调用计算函数

   if ( gen == 0 )/*参数为0时，对两个数组执行下面的操作
   				把第二行的值全部复制到第一行，
   				结果就是两个数组前两行是一样的*/
   {
      for ( k = 0; k <= M; k++ ) OBJECTIVE[0][k] = OBJECTIVE[1][k];

      for ( j = 1; j <= N; j++ ) CHROMOSOME[0][j] = CHROMOSOME[1][j];
   }

   for ( i = 1; i < POP_SIZE; i++ )  // i is staring from 1
   {
      label = 0;
      a = OBJECTIVE[i][0]; //a赋值为数组OBJECTIVE每一行的第一个元素

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
		  continue;  //如果myu ( 0, 1 )的结果大于P_CROSSOVER则直接执行下一次循环，而此次循环当中后续的语句不再执行

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
