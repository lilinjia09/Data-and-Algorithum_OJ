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
    float p;
    scanf("%d %d",&m,&n);
    scanf("%f",&p);
    int *len1 = (int*)calloc(n , sizeof(int));//优化len空间，现在的len是每一列的高度而不是长度
    float temp_val;//优化arr1空间
    int *stack=(int*)malloc((n+1)*sizeof(int));
    int order = -1;
    for(int i = 0; i < m; i++)
    {
        order = -1;
        for(int j = 0; j < n; j++)
        {
            scanf("%f",&temp_val);
            if(temp_val >= p)
            {
                len1[j]= len1[j] + 1;
            }
            else
            {
                len1[j] = 0;
            }
            while(order != -1 && len1[stack[order]] >= len1[j])
            {
                int ver = len1[stack[order]];
                order--;
                int hor = (order == -1) ? j : j - stack[order] - 1;
                if(hor * ver != 0 && cal_l(hor, ver) > l)
                {
                    l = cal_l(hor, ver);
                    best_hor = hor;
                    best_ver = ver;
                    left = (order == -1) ? 0 : stack[order] + 1;
                    top = i - ver + 1;
                }
            }
            order++;
            stack[order] = j;
        }
        while(order != -1)
        {
            int ver = len1[stack[order]];
            order--;
            int hor = (order == -1) ? n : n - stack[order] - 1;
            if(hor * ver != 0 && cal_l(hor, ver) > l)
            {
                l = cal_l(hor, ver);
                best_hor = hor;
                best_ver = ver;
                left = (order == -1) ? 0 : stack[order] + 1;
                top = i - ver + 1;
            }
        }
    }
    //改5:必须尽力优化时间和空间复杂度，把算法放进读取逻辑里，优化时间，把len减成一维数组，这样的话新的len就要根据之前的更新
    free(stack);
    free(len1);
    printf("%d %d\n",top,top+best_ver-1);
    printf("%d %d\n",left,left+best_hor-1);
    printf("%d",l);
    return 0;
}