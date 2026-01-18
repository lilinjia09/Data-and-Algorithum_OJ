//思路：动态规划
//============================ 头文件和宏定义 ============================
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

#pragma warning(disable:4996)

using namespace std;


int compare_reac(vector<int> front, vector<int> back)
{
    return max(abs(front[1]-back[0]),abs(front[3]-back[2]));
}

int main()
{
    int n;
    scanf("%d",&n);
    int total_time=1e9;
    int total_reac_t=0;
    vector<vector<int>> reac(n+2,vector<int>(5,0));//0号为初始状态，n+1号为终止状态
    vector<int> neutral={7,7,25,25,0};
    reac[0]=neutral;
    reac[n+1]={7,7,25,25,0};
    for(int i=1;i<=n;i++)
    {
        int a,b,c,d,e;
        scanf("%d %d %d %d %d",&a,&b,&c,&d,&e);
        reac[i]={a,b,c,d,e};
        total_reac_t+=e;
    }


    vector<vector<int>> dist(n+2,vector<int>(n+2,0));//dist[i][j]:从i到j的反应时间，i为起点
    for(int i=0;i<=n+1;i++)
    {
        for(int j=0;j<=n+1;j++)
        {
            dist[i][j]=compare_reac(reac[i],reac[j]);
        }
    }
    int full_state= (1<<n)-1;
    vector<vector<int>> dp(n+2,vector<int>(1<<n,1e9));//dp[i][s]:到达i点，状态为s的最短时间，i为从1到n+1,每个点自己和起点一定被访问，除此以外有(n-1)!种访问顺序
    //初始化,只访问一个节点
    for(int i=0;i<n;i++)
    {
        dp[i][1<<i]=dist[0][i+1];//dp的i是到达点，dist的i是起点
    }
    for(int state=0;state<=full_state;state++)
    {
        for(int i=0;i<n;i++)
        {
            if(!(state & (1<<i)))//i不在state中
            {
                continue;
            }
            if(dp[i][state]==1e9)//不可达状态
            {
                continue;
            }
            for(int j=0;j<n;j++)
            {
                if(state & (1<<j))//j已经在state中
                {
                    continue;
                }
                int next_state=state | (1<<j);//将j加入state
                dp[j][next_state]=min(dp[j][next_state],dp[i][state]+dist[i+1][j+1]);
            }

        }
    }
    //计算答案
    for(int i=0;i<n;i++)
    {
        total_time=min(total_time,dp[i][full_state]+dist[i+1][n+1]);
    }
    printf("%d",total_time+total_reac_t);
    return 0;
}