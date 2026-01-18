//思路：动态规划+剪枝
//============================ 头文件和宏定义 ============================
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

#pragma warning(disable:4996)

using namespace std;


int compare_reac(const vector<int> &front, const vector<int> &back)
{
    return max(abs(front[1]-back[0]),abs(front[3]-back[2]));
}

int main()
{
    int n;
    scanf("%d",&n);
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
    vector<vector<int>> dp(n,vector<int>(1<<n,1e9));//dp[i][s]:到达i点，状态为s的最短时间
    //初始化,只访问一个节点
    for(int i=0;i<n;i++)
    {
        dp[i][1<<i]=dist[0][i+1];//dp的i是到达点，dist的i是起点
    }
    //下界
    vector<int> min_dist(n+2,1e9);
    for(int i=0;i<=n+1;i++)
    {
        for(int j=0;j<=n+1;j++)
        {
            if(i!=j)
            {
                min_dist[i]=min(min_dist[i],dist[i][j]);
            }
        }
    }

    int ans=1e9;

    for(int state=1;state<=full_state;state++)
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

            int lower_bound=dist[i+1][n+1];//回终点的距离
            //计算下界
            for(int j=0;j<n;j++)
            {
                if(!(state & (1<<j)))//j不在state中
                {
                    lower_bound+=min_dist[j+1];
                }
            }
            if(dp[i][state]+lower_bound>=ans)//剪枝
            {
                continue;
            }

            if(state==full_state)//访问完所有节点，更新答案
            {
                ans=min(ans,dp[i][state]+dist[i+1][n+1]);
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
    printf("%d",ans+total_reac_t);
    return 0;
}