//思路：存树？能存吗，还是要边读边调整；找路。线性结构存储树，则第i个元素的父节点为(i-1)/2，左子节点为2*i+1，右子节点为2*i+2,节点数量为 n = (1 << h) - 1
//找路思路：从根节点开始找左右子树的最大路，然后组合，由于递归，所以出栈时应该是从叶节点出栈，此时开始更新最优值（忽略-99）
//提交1：空间超了，需要优化空间，可能得边扫边处理，思考如何解决
//目前的结论：做不到层序的时候边读边处理，必须先存树。

#include <stdio.h>
#include <stdlib.h>
void StoreTree(char *tree, int n)
{
    short temp;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &temp);
        tree[i]=(char)temp;
    }
}
int FindMaxValue(char *tree,int n,int index,int *value)
{
    int left=0,right=0;
    if(index >= n || (int)tree[index]==-99)
    return 0;
    left=FindMaxValue(tree,n,2*index+1,value);
    right=FindMaxValue(tree,n,2*index+2,value);
    int left_contrib =left>0?left:0;
    int right_contrib=right>0?right:0;
    int current=left_contrib+right_contrib+(int)tree[index];
    if(current>*value)
    *value=current;
    if(left_contrib>=right_contrib)
    return left_contrib+(int)tree[index];
    else
    return right_contrib+(int)tree[index];
}
int main()
{
    int h,n;
    int value;
    char *tree;
    scanf("%d", &h);
    n=(1<<h)-1;
    tree=(char *)malloc(n*sizeof(char));
    StoreTree(tree,n);
    value=-20;
    FindMaxValue(tree,n,0,&value);
    printf("%d",value);
    free(tree);
    return 0;
}