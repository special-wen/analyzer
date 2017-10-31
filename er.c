/*************************************************************************
	> File Name: er.c
	> Author: zxw
	> Mail: 
	> Created Time: 2017年10月23日 星期一 13时13分42秒
 ************************************************************************/
#include<stdio.h>
void binary(int array[],int n){
    int i = 0;
    int num = n;
    printf("%d %d \n",n,num);
    for(i = 0;i<3;i++){
        array[i] = i;
    }
}
int main(void){
    int array[30];
    int n,i;
    scanf("%d",&n);
    binary(array,n);
    for(i=0;i<3;i++){
        printf("%d ",array[i]);
    }
}
