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
    //思路2：先处理行，每个位置都存该行该位置前的连续的满足条件的数量，然后对新数组处理
    //先看每个位置对应列，选出列中最小的，每选一次计算周长，和best，这样就记录了尾和长度的信息
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
    for(int j=0;j<n;j++)
    {
        for(int i=0;i<m;i++)
        {
            int hor_min=n;
            if(len[i*n+j]>0)
            {
                ver_int=i;
                for(int k=0;k<m-i;k++)
                {
                    if(len[(i+k)*n+j]==0)break;
                    if(len[(i+k)*n+j]<hor_min)
                    {
                        hor_min = len[(i+k)*n+j];
                        ver_end = (i+k);
                        if(cal_l(hor_min,ver_end-ver_int+1)>l)
                        {
                            l = cal_l(hor_min,ver_end-ver_int+1);
                            best_hor = hor_min;
                            best_ver = ver_end-ver_int+1;
                            hor_f = j-hor_min+1;
                            ver_f = ver_int;
                        }
                    }
                }
            }
            
        }
    }
    free(arr1);
    free(len);
    printf("%d %d\n",ver_f,ver_f+best_ver-1);
    printf("%d %d\n",hor_f,hor_f+best_hor-1);
    printf("%d",l);
    return 0;
}