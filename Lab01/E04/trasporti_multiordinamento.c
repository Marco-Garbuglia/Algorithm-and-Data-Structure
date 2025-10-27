#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// uso lunghezza massima della stringa 100 , poco sopra a 64 = 2^5, e della stringa 500,ragionevole, come convenzioni
#define MAXS 100
#define MAXN 500

/* 
Il programma legge tutte le corse dal file e le salva in un vettore di strutture.
Per evitare di riordinare ogni volta, crea quattro vettori di puntatori (uno per data,
uno per codice, uno per partenza e uno per arrivo). In questo modo basta ordinare
solo i puntatori, non i dati veri, e si possono mantenere più ordinamenti insieme.
Uso l’insertion sort per sistemare i puntatori secondo il criterio scelto.
Dal menu posso scegliere come stampare o cercare: per codice usa la ricerca binaria,
per partenza scorre e stampa tutte le corse che iniziano con quel prefisso.
Le date e le ore sono salvate come interi per confrontarle più facilmente.
È la continuazione dell’esercizio precedente e serve per capire come gestire
più ordinamenti contemporaneamente senza duplicare i dati.
*/

typedef struct {
    char codice[MAXS];
    char partenza[MAXS];
    char destinazione[MAXS];
    int dataAAAAMMGG;
    int oraP;
    int oraA;
    int ritardo;
} voce_t;

typedef enum {ORD_DATA, ORD_CODICE, ORD_PARTENZA, ORD_ARRIVO} chiave_t;

static int cmp_ptr(const void *pa, const void *pb, void *vk){
    voce_t *a=*(voce_t* const*)pa;
    voce_t *b=*(voce_t* const*)pb;
    chiave_t k=*(chiave_t*)vk;
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
    } else {
        int c=strcmp(a->destinazione,b->destinazione);
        if(c!=0) return c;
        return strcmp(a->codice,b->codice);
    }
}

static void ordina_puntatori(voce_t *ptr[], int n, chiave_t k){
    for(int i=1;i<n;i++){
        voce_t *x=ptr[i];
        int j=i-1;
        while(j>=0 && cmp_ptr(&x,&ptr[j],&k)<0){ ptr[j+1]=ptr[j]; j--; }
        ptr[j+1]=x;
    }
}

static void stampa_ptr(voce_t *ptr[], int n){
    for(int i=0;i<n;i++){
        voce_t *e=ptr[i];
        printf("%s %s %s %08d %04d %04d %d\n",
            e->codice, e->partenza, e->destinazione,
            e->dataAAAAMMGG, e->oraP, e->oraA, e->ritardo);
    }
}

static int ha_prefisso(const char *s, const char *pref){
    size_t ls=strlen(s), lp=strlen(pref);
    if(lp>ls) return 0;
    return strncmp(s,pref,lp)==0;
}

int main(void){
    voce_t v[MAXN];
    voce_t *logC[MAXN], *logD[MAXN], *logP[MAXN], *logA[MAXN];
    int n=0;

    int nr; if(scanf("%d",&nr)!=1) return 0;
    for(int i=0;i<nr;i++){
        int a,m,g,hp,mp,ha,ma;
        if(scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d",
                 v[n].codice, v[n].partenza, v[n].destinazione,
                 &a,&m,&g,&hp,&mp,&ha,&ma,&v[n].ritardo)!=11) return 0;
        v[n].dataAAAAMMGG=a*10000+m*100+g;
        v[n].oraP=hp*100+mp;
        v[n].oraA=ha*100+ma;
        n++;
    }

    for(int i=0;i<n;i++){
        logC[i]=&v[i];
        logD[i]=&v[i];
        logP[i]=&v[i];
        logA[i]=&v[i];
    }

    ordina_puntatori(logC,n,ORD_CODICE);
    ordina_puntatori(logD,n,ORD_DATA);
    ordina_puntatori(logP,n,ORD_PARTENZA);
    ordina_puntatori(logA,n,ORD_ARRIVO);

    for(;;){
        int cmd;
        if(scanf("%d",&cmd)!=1) break;
        if(cmd==0) break;
        if(cmd==1){
            stampa_ptr(logD,n);
        } else if(cmd==2){
            stampa_ptr(logC,n);
        } else if(cmd==3){
            stampa_ptr(logP,n);
        } else if(cmd==4){
            stampa_ptr(logA,n);
        } else if(cmd==5){
            char cod[MAXS]; if(scanf("%s",cod)!=1) continue;
            int l=0,r=n-1,pos=-1;
            while(l<=r){
                int m=(l+r)/2;
                int c=strcmp(logC[m]->codice,cod);
                if(c==0){ pos=m; break; }
                if(c<0) l=m+1; else r=m-1;
            }
            if(pos==-1) printf("NA\n");
            else {
                voce_t *e=logC[pos];
                printf("%s %s %s %08d %04d %04d %d\n",
                    e->codice, e->partenza, e->destinazione,
                    e->dataAAAAMMGG, e->oraP, e->oraA, e->ritardo);
            }
        } else if(cmd==6){
            char pref[MAXS]; if(scanf("%s",pref)!=1) continue;
            int l=0,r=n-1,first=-1;
            while(l<=r){
                int m=(l+r)/2;
                int c=strncmp(logP[m]->partenza,pref,strlen(pref));
                if(c>=0) r=m-1; else l=m+1;
            }
            first=l;
            int printed=0;
            while(first<n && ha_prefisso(logP[first]->partenza,pref)){
                voce_t *e=logP[first];
                printf("%s %s %s %08d %04d %04d %d\n",
                    e->codice, e->partenza, e->destinazione,
                    e->dataAAAAMMGG, e->oraP, e->oraA, e->ritardo);
                printed=1;
                first++;
            }
            if(!printed) printf("NA\n");
        }
    }
    return 0;
}
