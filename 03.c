#include<stdio.h>
#include<stdlib.h>
//第一次提交，目前思路，先不求空间时间优化，全部存下，以终点为索引，存下所有起点和价格，然后看有哪些终点满足所有人都能到达
//对于存储，以终点为索引有序存储，每个终点对应的数组存起点也按顺序存，起点为索引，价格这样存，若是没有的航班位置置零。
//对于学者城市，存一个数组，索引为城市编号，值为学者数量。
//策略：去一个城市和0号城市谁更便宜就去哪里。
//要注意的一些情况：1.1号城市单独考虑，2.起点终点同城市的情况
//优化：1.先改改short
int sum=-1,dst_city=-1;
void optimize_city(short **dst,int m)
{
    for(int k=0;k<m;k++)
    {
        for(int i=0;i<m;i++)
        {
            if(i==k||dst[k][i]==0) continue;
            for(int j=0;j<m;j++)
            {
                if(k==j||dst[j][k]==0) continue;
                if(dst[k][i]>0&&dst[j][k]>0)
                {
                    if(dst[j][i]==0)
                    {
                        dst[j][i]=dst[k][i]+dst[j][k];
                    }
                    else
                    {
                        dst[j][i]=(dst[j][i]>(dst[j][k]+dst[k][i]))?(dst[j][k]+dst[k][i]):dst[j][i];
                    }
                }
            }
        }
    }
}
int calc_sum(short **dst,int *s_city,int m,int city)
{
    int temp_sum=0;
    for(int i=1;i<m;i++)//从1开始，0号的不需要走
    {
        if(s_city[i]>0)//对于城市i+1的学者
        {
            if(dst[city][i]==0&&city!=i)//没有航班且不是同城市
            {
                temp_sum+=dst[0][i]*s_city[i];//只能回0号城市
            }
            else if(city==i)//同城市
            {
                continue;
            }//不去别的城市最省钱
            else//有航班且不是同城市
            {
                temp_sum+=s_city[i]*(dst[city][i]>dst[0][i]?dst[0][i]:dst[city][i]);//city和0选便宜的
            }
        }
    }
    return temp_sum;
}

int main()
{
    int n,m,k;//参会者数量n、城市数量m、航班数量k，在程序中，a城市对应数组下标a-1
    scanf("%d%d%d",&n,&m,&k);
    short **dst=(short **)malloc(m*sizeof(short *));
    for(int j=0;j<m;j++)
    {
        dst[j]=(short *)calloc(m,sizeof(short));
    }
    for(int i=0;i<k;i++)
    {
        short a,b,c;
        scanf("%hd%hd%hd",&a,&b,&c);
        dst[b-1][a-1]=c;//起点为a城市，则数组下标为a-1
    }
    int *s_city=(int *)calloc(m,sizeof(int));
    for(int i=0;i<n;i++)
    {
        short a;
        scanf("%hd",&a);
        s_city[a-1]+=1;
    }
    //上面是收数据，下面是处理数据得到结果
    optimize_city(dst,m);
    for(int i=1;i<m;i++)//以非0城市为终点，计算总花费
    {
        int temp_sum=calc_sum(dst,s_city,m,i);
        if(sum==-1||temp_sum<sum)
        {
            sum=temp_sum;
            dst_city=i;
        }
    }
    printf("%d\n",dst_city+1);//输出城市编号要加1
    printf("%d",sum);
    for(int j=0;j<m;j++)
    {
        free(dst[j]);
    }
    free(dst);
    free(s_city);
    return 0;
}