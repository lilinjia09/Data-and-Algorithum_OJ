//思路：通过输入更新最短时间的反应状态
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

int calc_total(int current, vector<bool> &visited, vector<vector<int>> &dist, int count, int current_time,int n)
//递归穷举所有路径
{
    if(count == n)
    {
        return current_time + dist[current][n + 1];
    }
    int min_time=1e9;
    for(int i=1;i<=n;i++)
    {
        if(!visited[i])
        {
            visited[i]=true;
            int time=calc_total(i,visited,dist,count+1,current_time+dist[current][i],n);
            min_time=min(min_time,time);
            visited[i]=false;
        }
    }
    return min_time;
}

int main()
{
    int n;
    scanf("%d",&n);
    int total_time=0;
    int total_reac_t=0;
    vector<vector<int>> reac(n+2,vector<int>(5,0));
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
    vector<bool> visited(n+2,false);
    visited[0]=true;
    visited[n+1]=true;
    total_time=calc_total(0,visited,dist,0,0,n);
    printf("%d",total_time+total_reac_t);
    return 0;
}