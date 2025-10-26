#include <stdio.h>
#include <stdlib.h>

static int abs_i(int x){ return x<0?-x:x; }

static int mcd_binario(int a, int b){
    if(a==0) return abs_i(b);
    if(b==0) return abs_i(a);
    unsigned int x=abs_i(a), y=abs_i(b);
    int s=0;
    while(((x|y)&1u)==0u){ x>>=1; y>>=1; s++; }
    while((x&1u)==0u) x>>=1;
    do{
        while((y&1u)==0u) y>>=1;
        if(x>y){ unsigned int t=x; x=y; y=t; }
        y=y-x;
    }while(y);
    return (int)(x<<s);
}

int main(void){
    int a,b;
    while(scanf("%d %d",&a,&b)==2){
        printf("%d\n", mcd_binario(a,b));
    }
    return 0;
}
