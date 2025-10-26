#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char codice[8];
    char nome[64];
    char cognome[64];
    int dataN; 
    char via[64];
    char citta[64];
    int cap;
} Item;

typedef struct nodo {
    Item x;
    struct nodo *next;
} Nodo;

static int compData(int a, int b){ 
    if(a<b) return -1; 
    if(a>b) return 1; 
    return 0; 
}

static int data_to_int(const char *s){
    int g,m,a; 
    if(sscanf(s,"%d/%d/%d",&g,&m,&a)!=3) return 0;
    return a*10000+m*100+g;
}

static Nodo* inserisci_ordinato(Nodo *h, Item it){
    Nodo *n=(Nodo*)malloc(sizeof(Nodo)); n->x=it; n->next=NULL;
    if(!h || compData(it.dataN, h->x.dataN)<0){ n->next=h; return n; }
    Nodo *p=h;
    while(p->next && compData(it.dataN, p->next->x.dataN)>=0) p=p->next;
    n->next=p->next; p->next=n; 
    return h;
}

static Nodo* carica_file(Nodo *h, const char *nome){
    FILE *fp=fopen(nome,"r"); if(!fp) return h;
    Item it; char data[32];
    while(fscanf(fp,"%7s %63s %63s %31s %63s %63s %d",
                 it.codice,it.nome,it.cognome,data,it.via,it.citta,&it.cap)==7){
        it.dataN=data_to_int(data);
        h=inserisci_ordinato(h,it);
    }
    fclose(fp);
    return h;
}

static Nodo* inserisci_da_stdin(Nodo *h){
    Item it; char data[32];
    if(scanf("%7s %63s %63s %31s %63s %63s %d",
             it.codice,it.nome,it.cognome,data,it.via,it.citta,&it.cap)!=7) return h;
    it.dataN=data_to_int(data);
    return inserisci_ordinato(h,it);
}

static Nodo* cerca_codice(Nodo *h, const char *cod){
    for(Nodo *p=h;p;p=p->next) if(strcmp(p->x.codice,cod)==0) return p;
    return NULL;
}

static Nodo* cancella_codice(Nodo *h, const char *cod, Item *out, int *ok){
    *ok=0;
    if(!h) return h;
    if(strcmp(h->x.codice,cod)==0){
        Nodo *t=h->next; *out=h->x; free(h); *ok=1; return t;
    }
    Nodo *p=h;
    while(p->next && strcmp(p->next->x.codice,cod)!=0) p=p->next;
    if(p->next){
        Nodo *q=p->next; p->next=q->next; *out=q->x; free(q); *ok=1;
    }
    return h;
}

static Nodo* estrai_intervallo(Nodo *h, int d1, int d2, Item *out, int *ok){
    *ok=0;
    if(!h) return h;
    if(h->x.dataN>=d1 && h->x.dataN<=d2){
        Nodo *t=h->next; *out=h->x; free(h); *ok=1; return t;
    }
    Nodo *p=h;
    while(p->next && !(p->next->x.dataN>=d1 && p->next->x.dataN<=d2)) p=p->next;
    if(p->next){
        Nodo *q=p->next; p->next=q->next; *out=q->x; free(q); *ok=1;
    }
    return h;
}

static void stampa_item(FILE *fp, const Item *it){
    fprintf(fp,"%s %s %s %08d %s %s %d\n",
        it->codice,it->nome,it->cognome,it->dataN,it->via,it->citta,it->cap);
}

static void stampa_su_file(Nodo *h, const char *nome){
    FILE *fp=fopen(nome,"w"); if(!fp) return;
    for(Nodo *p=h;p;p=p->next) stampa_item(fp,&p->x);
    fclose(fp);
}

static void libera(Nodo *h){
    while(h){ Nodo *t=h->next; free(h); h=t; }
}

int main(void){
    Nodo *h=NULL;
    for(;;){
        int cmd;
        if(scanf("%d",&cmd)!=1) break;
        if(cmd==0) break;
        if(cmd==1){
            h=inserisci_da_stdin(h);
        } else if(cmd==2){
            char nome[128]; if(scanf("%127s",nome)!=1) continue;
            h=carica_file(h,nome);
        } else if(cmd==3){
            char cod[8]; if(scanf("%7s",cod)!=1) continue;
            Nodo *p=cerca_codice(h,cod);
            if(p) stampa_item(stdout,&p->x); else printf("NA\n");
        } else if(cmd==4){
            char cod[8]; if(scanf("%7s",cod)!=1) continue;
            Item out; int ok;
            h=cancella_codice(h,cod,&out,&ok);
            if(ok) stampa_item(stdout,&out); else printf("NA\n");
        } else if(cmd==5){
            char s1[32], s2[32];
            if(scanf("%31s %31s",s1,s2)!=2) continue;
            int d1=data_to_int(s1), d2=data_to_int(s2);
            if(d1>d2){ int t=d1; d1=d2; d2=t; }
            int any=0; 
            for(;;){
                Item out; int ok;
                Nodo *hn=h;
                h=estrai_intervallo(h,d1,d2,&out,&ok);
                if(!ok){ h=hn; break; }
                stampa_item(stdout,&out); any=1;
            }
            if(!any) printf("NA\n");
        } else if(cmd==6){
            char nome[128]; if(scanf("%127s",nome)!=1) continue;
            stampa_su_file(h,nome);
        }
    }
    libera(h);
    return 0;
}
