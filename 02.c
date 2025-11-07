//思路：存树？能存吗，还是要边读边调整；找路。线性结构存储树，则第i个元素的父节点为(i-1)/2，左子节点为2*i+1，右子节点为2*i+2,节点数量为 n = (1 << h) - 1
//找路思路：从根节点开始找左右子树的最大路，然后组合，由于递归，所以出栈时应该是从叶节点出栈，此时开始更新最优值（忽略-99）
//提交1：空间超了，需要优化空间，可能得边扫边处理，思考如何解决
//想到了，存完一层以后就处理一层，然后释放上一层的空间，但会不会因为局部最优解忽略了全局最优解？

#include <stdio.h>
#include <stdlib.h>
void StoreTree(int *tree, int n)
{
    for (int i = 0; i < n; i++)
        scanf("%d", &tree[i]);
}
int FindMaxValue(int *tree,int n,int index,int *value)
{
    int left=0,right=0;
    if(index >= n || tree[index]==-99)
    return 0;
    left=FindMaxValue(tree,n,2*index+1,value);
    right=FindMaxValue(tree,n,2*index+2,value);
    int current=left+right+tree[index];
    if(current>*value)
    *value=current;
    return left>right?left+tree[index]:right+tree[index];
}
int main()
{
    int h,n;
    int *tree,value;
    scanf("%d", &h);
    n=(1<<h)-1;
    tree=(int *)malloc(n*sizeof(int));
    StoreTree(tree,n);
    value=0;
    FindMaxValue(tree,n,0,&value);
    printf("%d",value);
    free(tree);
    return 0;
}