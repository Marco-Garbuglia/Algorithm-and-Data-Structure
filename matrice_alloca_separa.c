#include <stdio.h>
#include <stdlib.h>

static int **malloc2dR(int r, int c){
    int **m = (int**)malloc(r*sizeof(int*));
    int *d = (int*)malloc(r*c*sizeof(int));
    for(int i=0;i<r;i++) m[i]=d+i*c;
    return m;
}

static int **malloc2dP(int r, int c){
    int **m = (int**)malloc(r*sizeof(int*));
    for(int i=0;i<r;i++) m[i]=(int*)malloc(c*sizeof(int));
    return m;
}

static void libera2dR(int **m){
    if(!m) return;
    free(m[0]);
    free(m);
}

static void libera2dP(int **m, int r){
    if(!m) return;
    for(int i=0;i<r;i++) free(m[i]);
    free(m);
}

static void separa(int **m, int r, int c, int **bianche, int *nb, int **nere, int *nn){
    int tot=r*c;
    int n_b=tot/2;
    int n_n=tot-n_b;
    *bianche=(int*)malloc(n_b*sizeof(int));
    *nere=(int*)malloc(n_n*sizeof(int));
    int ib=0,in=0;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            if(((i+j)&1)==0) (*nere)[in++]=m[i][j];
            else (*bianche)[ib++]=m[i][j];
        }
    }
    *nb=ib; *nn=in;
}

int main(void){
    int r,c,modo;
    if(scanf("%d %d %d",&r,&c,&modo)!=3) return 0;
    int **m = (modo==0)?malloc2dR(r,c):malloc2dP(r,c);
    for(int i=0;i<r;i++) for(int j=0;j<c;j++) scanf("%d",&m[i][j]);
    int *bianche=0,*nere=0, nb=0, nn=0;
    separa(m,r,c,&bianche,&nb,&nere,&nn);
    for(int i=0;i<nb;i++) printf("%d%c", bianche[i], (i+1==nb)?'\n':' ');
    for(int i=0;i<nn;i++) printf("%d%c", nere[i], (i+1==nn)?'\n':' ');
    free(bianche); free(nere);
    if(modo==0) libera2dR(m); else libera2dP(m,r);
    return 0;
}
