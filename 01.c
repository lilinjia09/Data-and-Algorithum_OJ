#include<stdio.h>
#include<stdlib.h>
int cal_l(int hor,int ver)
{
    return 2*(hor+ver);
}
int main()
{
    int m,n;
    int hor_int,ver_int,hor_end,ver_end;
    int hor_f,ver_f;
    int best_hor=0;
    int best_ver=0;
    int l=0;
    float *arr1;
    float p;
    scanf("%d %d",&m,&n);
    scanf("%f",&p);
    arr1 = (float*)malloc(m*n*sizeof(float));
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            scanf("%f",&arr1[i*n+j]);
        }
    }
    //开始判断逻辑，下面是要优化的地方
    //思路3：先处理行，每个位置都存该行该位置前的连续的满足条件的数量，然后对新数组处理
    //存储换时间，每列上下、下上扫两遍，多开两个数组，存第一个比自己短的位置，然后再算
    int *len = (int*)calloc(m*n,sizeof(int));
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            if(arr1[i*n+j]>p)
            {
                if(j==0)len[i*n+j]=1;
                else len[i*n+j]=len[i*n+j-1]+1;
            }
            else len[i*n+j]=0;
        }
    }//算完了每个位置的横向连续长度，存在len里
    int *down=(int*)malloc(m*sizeof(int));
    int *up=(int*)malloc(m*sizeof(int));
    int *follower=(int*)malloc((m+1)*sizeof(int));
    int order=-1;
    for(int j = 0;j<n;j++)
    {
        for(int i = 0;i<m;i++)//从上往下扫，更新上方第一个比自己矮的
        {
            while(order!=-1&&len[i*n+j]<=len[follower[order]*n+j])
            {
                order--;
            }
            if(order==-1)
            {
                down[i]=-1;
            }
            else
            {
                down[i]=follower[order];
            }
            order++;
            follower[order]=i;
        }
        order=-1;
        for(int i=m-1;i>=0;i--)//从下往上扫，更新下方第一个比自己矮的
        {
            while(order!=-1&&len[i*n+j]<=len[follower[order]*n+j])
            {
                order--;
            }
            if(order==-1)
            {
                up[i]=m;
            }
            else
            {
                up[i]=follower[order];
            }
            order++;
            follower[order]=i;
        }
        for(int i=0;i<m;i++)
        {
            int hor=len[i*n+j];
            int ver=up[i]-down[i]-1;
            if(hor*ver!=0&&cal_l(hor,ver)>l)
            {
                l=cal_l(hor,ver);
                hor_f=j-hor+1;
                ver_f=down[i]+1;
                best_hor=hor;
                best_ver=ver;
            }
        }
    }
    free(down);
    free(up);
    free(follower);
    free(arr1);
    free(len);
    printf("%d %d\n",ver_f,ver_f+best_ver-1);
    printf("%d %d\n",hor_f,hor_f+best_hor-1);
    printf("%d",l);
    return 0;
}