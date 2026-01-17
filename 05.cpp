//思路：通过输入更新最短时间的反应状态
//============================ 头文件和宏定义 ============================
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

using namespace std;


int compare_reac(vector<int> front, vector<int> back)
{
    return max(abs(front[1]-back[0]),abs(front[3]-back[2]));
}

void add_reac(vector<vector<int>> &reac, vector<int> &temp, vector<int> &diff_time, int &total_time, int n)
{
    int temp_min=1e9;
    int idx=0;
    for(int i=0;i<reac.size()-1;i++)
    {
        int time1=compare_reac(reac[i],temp);
        int time2=compare_reac(temp,reac[i+1]);
        int extra_time=time1+time2-diff_time[i];
        if(extra_time<temp_min)
        {
            temp_min=extra_time;
            idx=i;
        }
    }
    total_time+=temp_min;
    diff_time[idx]=compare_reac(reac[idx],temp);
    diff_time.insert(diff_time.begin()+idx+1,compare_reac(temp,reac[idx+1]));
    reac.insert(reac.begin()+idx+1,temp);
}

int main()
{
    int n;
    scanf("%d",&n);
    int total_time=0;
    int total_reac_t=0;
    vector<vector<int>> reac(2,vector<int>(5,0));
    reac.reserve(n+2);
    vector<int> diff_time(1,0);
    diff_time.reserve(n+1);
    vector<int> neutral={7,7,7,7,0};
    vector<int> temp(5,0);
    reac[0]=neutral;
    reac[1]=neutral;
    for(int i=0;i<n;i++)
    {
        int a,b,c,d,e;
        scanf("%d %d %d %d %d",&a,&b,&c,&d,&e);
        temp[0]=a;
        temp[1]=b;
        temp[2]=c;
        temp[3]=d;
        temp[4]=e;
        add_reac(reac,temp,diff_time,total_time,n);
        total_reac_t+=e;
    }
    printf("%d",total_time+total_reac_t);
    return 0;
}