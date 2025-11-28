#include<stdio.h>
#include<stdlib.h>
#define MIN(a,b) ((a)<(b)?(a):(b))
//第一次提交，目前思路，先不求空间时间优化，全部存下，以终点为索引，存下所有起点和价格，然后看有哪些终点满足所有人都能到达
//对于存储，以终点为索引有序存储，每个终点对应的数组存起点也按顺序存，起点为索引，价格这样存，若是没有的航班位置置零。
//对于学者城市，存一个数组，索引为城市编号，值为学者数量。
//策略：去一个城市和0号城市谁更便宜就去哪里。
//要注意的一些情况：1.1号城市单独考虑，2.起点终点同城市的情况
//优化：1.先改改short，发现有一定优化，但不多。2.对于m大，边少的情况，要换思路
int sum=-1,dst_city=-1;
void optimize_city1(short **dst,int m)
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
struct Edge
{
    short start;
    short price;
    struct Edge *next;
};
struct Index_Node
{
    struct Edge *head;
    struct Edge *tail;
    short count;
};
struct Edge* Insert_Node(struct Index_Node *target,short start,short price)
{
    struct Edge *temp=(struct Edge *)malloc(sizeof(struct Edge));
    temp->start=start;
    temp->price=price;
    temp->next=NULL;
    if(target->head==NULL)
    {
        target->head=temp;
        target->tail=temp;
    }
    else
    {
        target->tail->next=temp;
        target->tail=temp;
    }
    target->count+=1;
}
short* dij(struct Index_Node *dst,int m,int target)
{
    short *dist=(short *)calloc(m,sizeof(short));//dist[i]为从终点target到i的最短距离
    short *visited=(short *)calloc(m,sizeof(short));//0未访问，1已访问
    for(int i=0;i<m;i++)
    {
        dist[i]=32767;//初始化为无穷大
    }
    dist[target]=0;//到自己的距离为0
    short best_start=-1;
    short best_price=32767;
    for(int cnt=0;cnt<m;cnt++)
    {
        best_start=-1;
        best_price=32767;
        for(int i=0;i<m;i++)
        {
            if(visited[i]==0&&dist[i]<best_price)
            {
                best_price=dist[i];
                best_start=i;
            }//找未访问点中dist最小的点
        }
        if(best_start==-1) break;//所有点都访问完
        visited[best_start]=1;
        //遍历best_start的边
        struct Edge *current=dst[best_start].head;
        while(current!=NULL)
        {
            if(visited[current->start]==0)
            {
                dist[current->start]=MIN(dist[current->start],dist[best_start]+current->price);
            }
            current=current->next;
        }
    }
    free(visited);
    return dist;
}

int main()
{
    int n,m,k;//参会者数量n、城市数量m、航班数量k，在程序中，a城市对应数组下标a-1
    scanf("%d%d%d",&n,&m,&k);
    if(m<=500)//稠密图用矩阵存
    {
        short **dst=(short **)malloc(m*sizeof(short *));//以终点为索引存储航班价格
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
        optimize_city1(dst,m);
        for(int i=0;i<m;i++)//以非0城市为终点，计算总花费
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
    }
    else//稀疏图用链表存
    {
        struct Index_Node *dst=(struct Index_Node *)calloc(m,sizeof(struct Index_Node));//以终点为索引存储航班价格,0城市用数组额外存一遍，别的只用链表
        short *dst0 =(short *)calloc(m,sizeof(short));//存0号城市的航班价格
        for(int i=0;i<k;i++)
        {
            short a,b,c;
            scanf("%hd%hd%hd",&a,&b,&c);
            Insert_Node(&dst[b-1],a-1,c);//起点为a城市，则数组下标为a-1
        }
        int *s_city=(int *)calloc(m,sizeof(int));
        for(int i=0;i<n;i++)
        {
            short a;
            scanf("%hd",&a);
            s_city[a-1]+=1;
        }
        //上面是收数据，下面是处理数据得到结果
        dst0=dij(dst,m,0);//计算0号城市到各城市的最短距离
        for(int i=0;i<m;i++)
        {
            short *dist=dij(dst,m,i+1);//计算i+1号城市到各城市的最短距离
            int temp_sum=0;
            for(int j=0;j<m;j++)
            {
                if(s_city[j]>0)
                {
                    temp_sum+=s_city[j]*MIN(dist[j],dst0[j]);
                }
            }
            if(sum==-1||temp_sum<sum)
            {
                sum=temp_sum;
                dst_city=i;
            }
            free(dist);
        }
        printf("%d\n",dst_city+1);//输出城市编号要加1
        printf("%d",sum);
        for(int i=0;i<m;i++)
        {
            struct Edge *current=dst[i].head;
            while(current!=NULL)
            {
                struct Edge *temp=current;
                current=current->next;
                free(temp);
            }
        }
        free(dst);
        free(dst0);
        free(s_city);
    }
    return 0;
}