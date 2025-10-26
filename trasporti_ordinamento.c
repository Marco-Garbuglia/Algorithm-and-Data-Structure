#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXS 100
#define MAXN 500

typedef struct {
    char codice[MAXS];
    char partenza[MAXS];
    char destinazione[MAXS];
    int dataAAAAMMGG;
    int oraP; 
    int oraA; 
    int ritardo; 
} voce_t;

typedef enum {ORD_NONE, ORD_DATA, ORD_CODICE, ORD_PARTENZA, ORD_ARRIVO} chiave_t;

static int confronta(const voce_t *a, const voce_t *b, chiave_t k){
    if(k==ORD_DATA){
        if(a->dataAAAAMMGG!=b->dataAAAAMMGG) return (a->dataAAAAMMGG<b->dataAAAAMMGG)?-1:1;
        if(a->oraP!=b->oraP) return (a->oraP<b->oraP)?-1:1;
        return strcmp(a->codice,b->codice);
    } else if(k==ORD_CODICE){
        return strcmp(a->codice,b->codice);
    } else if(k==ORD_PARTENZA){
        int c=strcmp(a->partenza,b->partenza);
        if(c!=0) return c;
        return strcmp(a->codice,b->codice);
    } else if(k==ORD_ARRIVO){
        int c=strcmp(a->destinazione,b->destinazione);
        if(c!=0) return c;
        return strcmp(a->codice,b->codice);
    }
    return 0;
}

static void ordina_inserimento(voce_t v[], int n, chiave_t k){
    for(int i=1;i<n;i++){
        voce_t x=v[i];
        int j=i-1;
        while(j>=0 && confronta(&x,&v[j],k)<0){ v[j+1]=v[j]; j--; }
        v[j+1]=x;
    }
}

static int ricerca_dicotomica_codice(voce_t v[], int n, const char *cod){
    int l=0,r=n-1;
    while(l<=r){
        int m=(l+r)/2;
        int c=strcmp(v[m].codice,cod);
        if(c==0) return m;
        if(c<0) l=m+1; else r=m-1;
    }
    return -1;
}

static int ricerca_lineare_codice(voce_t v[], int n, const char *cod){
    for(int i=0;i<n;i++) if(strcmp(v[i].codice,cod)==0) return i;
    return -1;
}

static int ha_prefisso(const char *s, const char *pref){
    size_t ls=strlen(s), lp=strlen(pref);
    if(lp>ls) return 0;
    return strncmp(s,pref,lp)==0;
}

static void stampa_intervallo(voce_t v[], int i0, int i1){
    for(int i=i0;i<=i1;i++){
        printf("%s %s %s %08d %04d %04d %d\n",
            v[i].codice, v[i].partenza, v[i].destinazione,
            v[i].dataAAAAMMGG, v[i].oraP, v[i].oraA, v[i].ritardo);
    }
}

static void stampa_tutto(voce_t v[], int n){
    for(int i=0;i<n;i++){
        printf("%s %s %s %08d %04d %04d %d\n",
            v[i].codice, v[i].partenza, v[i].destinazione,
            v[i].dataAAAAMMGG, v[i].oraP, v[i].oraA, v[i].ritardo);
    }
}

static int compone_data(int a, int m, int g){ return a*10000+m*100+g; }
static int compone_ora(int h, int mi){ return h*100+mi; }

int main(void){
    voce_t v[MAXN];
    int n=0;
    chiave_t ordinamento=ORD_NONE;

    int nr; if(scanf("%d",&nr)!=1) return 0;
    for(int i=0;i<nr;i++){
        int a,m,g,hp,mp,ha,ma;
        if(scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d",
                 v[n].codice, v[n].partenza, v[n].destinazione,
                 &a,&m,&g,&hp,&mp,&ha,&ma,&v[n].ritardo)!=11) return 0;
        v[n].dataAAAAMMGG=compone_data(a,m,g);
        v[n].oraP=compone_ora(hp,mp);
        v[n].oraA=compone_ora(ha,ma);
        n++;
    }

    for(;;){
        int cmd; 
        if(scanf("%d",&cmd)!=1) break;
        if(cmd==0) break;
        if(cmd==1){ ordina_inserimento(v,n,ORD_DATA); ordinamento=ORD_DATA; }
        else if(cmd==2){ ordina_inserimento(v,n,ORD_CODICE); ordinamento=ORD_CODICE; }
        else if(cmd==3){ ordina_inserimento(v,n,ORD_PARTENZA); ordinamento=ORD_PARTENZA; }
        else if(cmd==4){ ordina_inserimento(v,n,ORD_ARRIVO); ordinamento=ORD_ARRIVO; }
        else if(cmd==5){
            char cod[MAXS]; if(scanf("%s",cod)!=1) continue;
            int pos = (ordinamento==ORD_CODICE)?ricerca_dicotomica_codice(v,n,cod):ricerca_lineare_codice(v,n,cod);
            if(pos>=0) stampa_intervallo(v,pos,pos);
            else printf("NA\n");
        } else if(cmd==6){
            char pref[MAXS]; if(scanf("%s",pref)!=1) continue;
            if(ordinamento==ORD_PARTENZA){
                int l=0,r=n-1,found=-1;
                while(l<=r){
                    int m=(l+r)/2;
                    int cmp=strncmp(v[m].partenza,pref,strlen(pref));
                    if(cmp==0){ found=m; break; }
                    if(cmp<0) l=m+1; else r=m-1;
                }
                if(found==-1){ printf("NA\n"); continue; }
                int i0=found, i1=found;
                while(i0-1>=0 && ha_prefisso(v[i0-1].partenza,pref)) i0--;
                while(i1+1<n && ha_prefisso(v[i1+1].partenza,pref)) i1++;
                stampa_intervallo(v,i0,i1);
            } else {
                int any=0;
                for(int i=0;i<n;i++) if(ha_prefisso(v[i].partenza,pref)){ stampa_intervallo(v,i,i); any=1; }
                if(!any) printf("NA\n");
            }
        } else if(cmd==7){
            stampa_tutto(v,n);
        }
    }
    return 0;
}
