#include <stdio.h>
#include <string.h>

// uso lunghezza massima della riga 2^8 come convenzione
#define MAXRIGA 256 

/*  
Programma che cerca una sequenza dentro una stringa secondo semplici regole: 
il punto '.' può sostituire qualsiasi carattere, mentre '*' ripete il carattere precedente più volte  
e '?' lo rende facoltativo. Il codice controlla la corrispondenza dei caratteri e prova  
tutte le posizioni della stringa per trovare quelle che rispettano il modello dato.  
Ogni volta che trova una parte valida, la stampa a video.  
Viene gestito anche il caso in cui non ci siano risultati, stampando un messaggio dedicato.  
*/

static int carattere_corrisponde(char base, char ch){ return base=='.' || base==ch; }

static int corrisponde_prefisso(const char *s, const char *p, int *consumati){
    if(*p=='\0'){ *consumati=0; return 1; }
    if(*s=='\0'){
        const char *q=p;
        while(*q){
            if(*(q+1)=='*' || *(q+1)=='?') q+=2;
            else return 0;
        }
        *consumati=0;
        return 1;
    }
    char base=*p;
    char quant=(p[1]=='*' || p[1]=='?')?p[1]:'\0';
    if(quant=='*'){
        int k=0;
        while(s[k] && carattere_corrisponde(base, s[k])) k++;
        for(int i=0;i<=k;i++){
            int sub=0;
            if(corrisponde_prefisso(s+i, p+2, &sub)){ *consumati=i+sub; return 1; }
        }
        return 0;
    }else if(quant=='?'){
        int sub=0;
        if(corrisponde_prefisso(s, p+2, &sub)){ *consumati=sub; return 1; }
        if(carattere_corrisponde(base, *s)){
            if(corrisponde_prefisso(s+1, p+2, &sub)){ *consumati=1+sub; return 1; }
        }
        return 0;
    }else{
        if(carattere_corrisponde(base, *s)){
            int sub=0;
            if(corrisponde_prefisso(s+1, p+1, &sub)){ *consumati=1+sub; return 1; }
        }
        return 0;
    }
}

int main(void){
    char modello[MAXRIGA], sorgente[MAXRIGA];
    if(!fgets(modello, sizeof(modello), stdin)) return 0;
    if(!fgets(sorgente, sizeof(sorgente), stdin)) return 0;
    size_t lm=strlen(modello); if(lm&&modello[lm-1]=='\n') modello[lm-1]='\0';
    size_t ls=strlen(sorgente); if(ls&&sorgente[ls-1]=='\n') sorgente[ls-1]='\0';
    int trovate=0;
    for(size_t i=0; i<strlen(sorgente); ++i){
        int L=0;
        if(corrisponde_prefisso(sorgente+i, modello, &L) && L>0){
            printf("%.*s\n", L, sorgente+i);
            trovate++;
        }
    }
    if(!trovate) printf("(nessuna corrispondenza)\n");
    return 0;
}
