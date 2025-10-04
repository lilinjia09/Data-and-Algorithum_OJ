#include<stdio.h>
#include<stdlib.h>
int cal_l(int hor,int ver)
{
    return hor*2+ver*2;
}
int move_down(float*arr,int m,int n,int i,int j,int hor,int ver,float p)//进入move时：ver和hor都是已确定的
{
    if(i+ver>=m)return 0;
    for(int k=0;k<hor;k++)
    {
        if(arr[(i+ver)*n+j+k]<=p)return 0;
    }
    return 1;
}
int main()
{
    int m,n;
    int m_f,n_f;
    int best_hor,best_ver;
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
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            int hor=0;//hor是先判断再更新，就是说在++后的hor才是现在的进度
            int ver;//在++前的ver就是现在的进度
            while((j+hor<n)&&(arr1[i*n+j+hor]>p))
            {
                hor++;
                ver=1;
                while(move_down(arr1,m,n,i,j,hor,ver,p))
                {
                    ver++;
                }
                if(cal_l(hor,ver)>l)
                {
                    m_f = i;
                    n_f = j;
                    best_hor = hor;
                    best_ver = ver;
                    l = cal_l(hor,ver);
                }
            }
        }
    }
    printf("%d %d\n",m_f,m_f-1+best_ver);
    printf("%d %d\n",n_f,n_f-1+best_hor);
    printf("%d",l);
    free(arr1);
    return 0;
}