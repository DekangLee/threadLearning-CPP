// #include<stdio.h>
// #include<math.h>
// #include <iostream>
// using namespace std;
// double dp[22];//一维数组，dp[i]:买i张印章，凑齐n种印章的概率 
// /*
// 为什么用一维数组？
// 因为在此解法所用的递推公式中， 
// 买m张印章，凑齐n种印章的概率只与买m张印章,凑齐n-1种,n-2种...1种 的概率有关 , 
// 与买x(x!=m)张印章凑齐y(y>=1)种印章的概率无关 
// */ 
// long long c_fun(int n,int m)//求组合数C(n,m); 
// {
// 	long i;
// 	long long ans_n=1,ans_m=1,ans_nm=1;
// 	for(i=2;i<=n;i++)
// 		ans_n=ans_n*i;
// 	for(i=2;i<=m;i++)
// 		ans_m=ans_m*i;
// 	for(i=2;i<=n-m;i++)
// 		ans_nm=ans_nm*i;
// 	return ans_n/(ans_m*ans_nm);
	
// } 
// double fun(int n,int m)//求解印章问题 
// {
// 	int i,j;
// 	double t=0;
// 	dp[1]=1.0;//只需凑齐1种，必定可以凑齐 
// 	for(i=2;i<=n;i++)//循环,计算需凑齐2,3,4...n种的情况 
// 	{
// 		t=0;
// 		for(j=1;j<i;j++)// 
// 		{
// 			t=t+c_fun(i,j)*dp[j]*pow(j,m);//递推公式 
					
// 		}
// 		dp[i]=1-t/pow(i,m);
// 		//要用1去减，因为前面递推公式计算出来的是凑不齐的概率,dp数组存的是凑齐的概率 
// 	}
// 	return dp[n];//返回结果 
// } 
// int printf(int a)
// {
// 	return a;
// }
// int main()
// {
// 	// int n,m;
// 	// scanf("%d%d",&n,&m);//共n种印章，买m张 
// 	// fun(n,m);//求解 
// 	// printf("%.4lf\n",dp[n]);//dp[n]即为结果 
//     cout<<sizeof(float)<<endl;
// 	return 0;
// }
