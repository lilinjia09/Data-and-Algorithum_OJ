#include<stdio.h>
int main()
{
    int xor=0;
    int num,temp;
    scanf("%d",&num);
    for(int i = 0;i<num;i++)
    {
        scanf("%d",&temp);
        xor = xor ^ temp;
    }
    printf("%d",xor);
    return 0;
}