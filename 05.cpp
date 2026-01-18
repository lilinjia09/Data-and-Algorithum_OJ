//思路：DFS+剪枝
//============================ 头文件和宏定义 ============================
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

#pragma warning(disable:4996)

using namespace std;

int ans=1e9;
int n;
vector<int> min_dist; // min_dist[i]: 到节点 i的最小入边
vector<vector<int>> dist;//dist[i][j]:从i到j的反应时间，i为起点
vector<bool> visited;//visited[i]:节点i是否被访问过

int compare_reac(const vector<int> &front, const vector<int> &back)
{
    return max(abs(front[1]-back[0]),abs(front[3]-back[2]));
}

void dfs(int current, int count, int cost, int remain)
{
    // 递归终止：所有反应都访问完了
    if(count == n)
    {
        int total = cost + dist[current][n + 1];
        if(total < ans) 
            ans = total;
        return;
    }

    //当前代价 + 下界 >= 已知最优解，剪掉
    //下界 = 剩余节点的最小入边之和
    if(cost + remain >= ans)
        return;
    
    // 尝试访问每个未访问的反应
    for(int j = 1; j <= n; j++)
    {
        if(visited[j]) continue;
        
        visited[j] = true;
        // remain 减去 min_in[j]，因为 j 即将被访问
        dfs(j, count + 1, cost + dist[current][j], remain - min_dist[j]);
        visited[j] = false;  // 回溯
    }
}

int main()
{
    scanf("%d",&n);
    dist=vector<vector<int>>(n+2,vector<int>(n+2,0));
    min_dist=vector<int>(n+2,1e9);
    visited=vector<bool>(n+2,false);

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


    for(int i=0;i<=n+1;i++)
    {
        for(int j=0;j<=n+1;j++)
        {
            dist[i][j]=compare_reac(reac[i],reac[j]);
        }
    }


    //下界
    int remain=0;
    for(int j=1;j<=n+1;j++)//j是目标
    {
        for(int i=0;i<=n+1;i++)//i是起点
        {
            if(i!=j)
            {
                min_dist[j]=min(min_dist[j],dist[i][j]);
            }
        }
        remain+=min_dist[j];
    }
    dfs(0,0,0,remain);
    
    printf("%d",ans+total_reac_t);
    return 0;
}