#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *codice;
    char *partenza;
    char *destinazione;
    int dataAAAAMMGG;
    int oraP;
    int oraA;
    int ritardo;
} voce_t;

static int comp_data_ora(const voce_t *a, const voce_t *b){
    if(a->dataAAAAMMGG!=b->dataAAAAMMGG) return (a->dataAAAAMMGG<b->dataAAAAMMGG)?-1:1;
    if(a->oraP!=b->oraP) return (a->oraP<b->oraP)?-1:1;
    return strcmp(a->codice,b->codice);
}

static void ordina_inserimento_ptr(voce_t **v, int n, int (*cmp)(const voce_t*, const voce_t*)){
    for(int i=1;i<n;i++){
        voce_t *x=v[i];
        int j=i-1;
        while(j>=0 && cmp(x,v[j])<0){ v[j+1]=v[j]; j--; }
        v[j+1]=x;
    }
}

static int cmp_cod(const voce_t *a, const voce_t *b){ return strcmp(a->codice,b->codice); }
static int cmp_part(const voce_t *a, const voce_t *b){ int c=strcmp(a->partenza,b->partenza); return c?c:strcmp(a->codice,b->codice); }
static int cmp_arr(const voce_t *a, const voce_t *b){ int c=strcmp(a->destinazione,b->destinazione); return c?c:strcmp(a->codice,b->codice); }

static int compone_data(int a,int m,int g){ return a*10000+m*100+g; }
static int compone_ora(int h,int mi){ return h*100+mi; }

static void stampa(voce_t *e){
    printf("%s %s %s %08d %04d %04d %d\n", e->codice, e->partenza, e->destinazione, e->dataAAAAMMGG, e->oraP, e->oraA, e->ritardo);
}

static void libera_tabella(voce_t *log, int n){
    for(int i=0;i<n;i++){
        free(log[i].codice);
        free(log[i].partenza);
        free(log[i].destinazione);
    }
    free(log);
}

int main(void){
    int n; 
    if(scanf("%d",&n)!=1) return 0;
    voce_t *log = (voce_t*)malloc(n*sizeof(voce_t));
    for(int i=0;i<n;i++){
        char c[64], p[64], d[64];
        int a,m,g,hp,mp,ha,ma, rit;
        if(scanf("%63s %63s %63s %d-%d-%d %d:%d %d:%d %d", c,p,d,&a,&m,&g,&hp,&mp,&ha,&ma,&rit)!=11) return 0;
        log[i].codice=strdup(c);
        log[i].partenza=strdup(p);
        log[i].destinazione=strdup(d);
        log[i].dataAAAAMMGG=compone_data(a,m,g);
        log[i].oraP=compone_ora(hp,mp);
        log[i].oraA=compone_ora(ha,ma);
        log[i].ritardo=rit;
    }
    voce_t **logC=(voce_t**)malloc(n*sizeof(voce_t*));
    voce_t **logD=(voce_t**)malloc(n*sizeof(voce_t*));
    voce_t **logP=(voce_t**)malloc(n*sizeof(voce_t*));
    voce_t **logA=(voce_t**)malloc(n*sizeof(voce_t*));
    for(int i=0;i<n;i++){ logC[i]=&log[i]; logD[i]=&log[i]; logP[i]=&log[i]; logA[i]=&log[i]; }
    ordina_inserimento_ptr(logC,n,cmp_cod);
    ordina_inserimento_ptr(logD,n,comp_data_ora);
    ordina_inserimento_ptr(logP,n,cmp_part);
    ordina_inserimento_ptr(logA,n,cmp_arr);
    for(;;){
        int cmd;
        if(scanf("%d",&cmd)!=1) break;
        if(cmd==0) break;
        if(cmd==1){ for(int i=0;i<n;i++) stampa(logD[i]); }
        else if(cmd==2){ for(int i=0;i<n;i++) stampa(logC[i]); }
        else if(cmd==3){ for(int i=0;i<n;i++) stampa(logP[i]); }
        else if(cmd==4){ for(int i=0;i<n;i++) stampa(logA[i]); }
        else if(cmd==5){
            char cod[64]; if(scanf("%63s",cod)!=1) continue;
            int l=0,r=n-1,pos=-1;
            while(l<=r){
                int m=(l+r)/2;
                int c=strcmp(logC[m]->codice,cod);
                if(c==0){ pos=m; break; }
                if(c<0) l=m+1; else r=m-1;
            }
            if(pos==-1) printf("NA\n"); else stampa(logC[pos]);
        } else if(cmd==6){
            char pref[64]; if(scanf("%63s",pref)!=1) continue;
            int l=0,r=n-1;
            while(l<=r){
                int m=(l+r)/2;
                int c=strncmp(logP[m]->partenza,pref,strlen(pref));
                if(c>=0) r=m-1; else l=m+1;
            }
            int i=l, any=0;
            while(i<n && strncmp(logP[i]->partenza,pref,strlen(pref))==0){ stampa(logP[i]); any=1; i++; }
            if(!any) printf("NA\n");
        }
    }
    free(logC); free(logD); free(logP); free(logA);
    libera_tabella(log,n);
    return 0;
}
