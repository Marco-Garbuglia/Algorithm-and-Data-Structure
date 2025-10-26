#include <stdio.h>
#include <stdlib.h>

static int conta(const int *a, int l, int r, int val){
    int c=0; for(int i=l;i<r;i++) if(a[i]==val) c++; return c;
}

static int majorityR(const int *a, int l, int r){
    int n=r-l;
    if(n<=0) return -1;
    if(n==1) return a[l];
    int m=l+n/2;
    int sx=majorityR(a,l,m);
    int dx=majorityR(a,m,r);
    if(sx==dx) return sx;
    int cs= sx==-1? 0:conta(a,l,r,sx);
    if(cs>n/2) return sx;
    int cd= dx==-1? 0:conta(a,l,r,dx);
    if(cd>n/2) return dx;
    return -1;
}

int majority(int *a, int N){
    return majorityR(a,0,N);
}

int main(void){
    int N; 
    if(scanf("%d",&N)!=1) return 0;
    int *a=(int*)malloc(N*sizeof(int));
    for(int i=0;i<N;i++) scanf("%d",&a[i]);
    int res=majority(a,N);
    printf("%d\n", res);
    free(a);
    return 0;
}
