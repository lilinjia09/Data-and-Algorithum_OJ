#include<stdio.h>
#include<stdlib.h>
int cal_l(int hor,int ver)
{
    return 2*(hor+ver);
}
int main()
{
    int m,n;
    int top,left;
    int best_hor=0;
    int best_ver=0;
    int l=0;
    float *arr1;
    float p;
    scanf("%d %d",&m,&n);
    scanf("%f",&p);
    arr1 = (float*)malloc(m*n*sizeof(float));
    int *len = (int*)calloc(m*n,sizeof(int));
    int *len1 = (int*)calloc(m , sizeof(int));//优化len空间
    float temp_val;//优化arr1空间
    int *stack=(int*)malloc((m+1)*sizeof(int));
    int order = -1;
    for(int j = 0; )
    //改5:必须尽力优化时间和空间复杂度，把算法放进读取逻辑里，优化时间，把len减成一维数组，这样的话新的len就要根据之前的更新
    
    for(int j = 0; j < n; j++)
    {
        order = -1;
        for(int i = 0; i < m; i++)
        {
            while(order != -1 && len[stack[order]*n + j] >= len[i*n + j])
            {
                int hor = len[stack[order]*n + j];
                order--;
                int ver = (order == -1) ? i : i - stack[order] - 1;
                if(hor * ver != 0 && cal_l(hor, ver) > l)
                {
                    l = cal_l(hor, ver);
                    best_hor = hor;
                    best_ver = ver;
                    top = (order == -1) ? 0 : stack[order] + 1;
                    left = j - hor + 1;
                }
            }
            order++;
            stack[order] = i;
        }
        while(order != -1)
                {
                    int hor = len[stack[order]*n + j];
                    order--;
                    int ver = (order == -1) ? m : m - stack[order] - 1;
                    if(hor * ver != 0 && cal_l(hor, ver) > l)
                    {
                        l = cal_l(hor, ver);
                        best_hor = hor;
                        best_ver = ver;
                        top = (order == -1) ? 0 : stack[order] + 1;
                        left = j - hor + 1;
                    }
                }
    }
    free(stack);
    free(arr1);
    free(len);
    printf("%d %d\n",top,top+best_ver-1);
    printf("%d %d\n",left,left+best_hor-1);
    printf("%d",l);
    return 0;
}