#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include<string.h>
#define MAX 110

typedef struct carta{ //struct com informa��o das cartas
    char cor[30];
    int valor;
}Carta;
typedef struct baralho{//struct do baralho, ou seja, a pilha
    int topo;
    Carta vet[MAX];
}Baralho;
typedef struct listaCarta{//struct "elemento" m�o do jogador (Lista din�mica)
    Carta c;
    struct listaCarta *prox;
}ListaCarta;
typedef struct maoCarta{//struct m�o do jogador(Lista din�mica)
    ListaCarta *inicio;
    int tamanho;
}MaoCarta;
typedef struct jogador{//struct para armazenar os dados dos jogadores
    char nome[30];
    int qt;//quantidade cartas
    int pontos;//pontos
    int v;//partidas vencidas
}Jogador;

Baralho *criaBaralho(); //fun��o de criar o baralho (pilha)
int empilha (Baralho *p, Carta dados);//fun��o para colocar as cartas na pilha
Carta desempilha(Baralho *p, Carta *k);//fun��o que vai desempilhar
void definir(Baralho *p);//fun��o para definir as cartas
void embaralhar(Baralho *p);//embaralhar o baralho (pilha)
MaoCarta *criaMaoCarta(); //Lista din�mica que vai ficar na m�o dos jogadores
void inserir(MaoCarta *m, Carta dados);//inserir na m�o de cartas do jogador
void imprimeMao(MaoCarta *m);//imprimir carta dos jogadores
void imprimeTopo(Baralho *p);//imprime o topo
ListaCarta *busca(MaoCarta *m, int indice);//fun��o para auxiliar a de retirada
Carta retira (MaoCarta *m, int indice);//fun��o para retirar a carta da m�o do jogador
void compra(MaoCarta *m, Baralho *p);//fun��o de compra no monte
void jogadas(MaoCarta *m, MaoCarta *mm, Baralho *descarte, Baralho *p);//fun��o geral das jogadas
void jogarCarta(MaoCarta *m, MaoCarta *mm, int pos, Baralho *descarte, Baralho *p);//fun��o de jogar a carta
void contagem(MaoCarta *m, Jogador j);//fun��o de contagem de pontos

int main(){
    setlocale(LC_ALL, "Portuguese");
    Baralho *p = criaBaralho();//criando baralho
    Baralho *descarte = criaBaralho();//criando o monte de descarte
    MaoCarta *m1 = criaMaoCarta();//criando m�o de cartas jogador 1
    MaoCarta *m2= criaMaoCarta();//criando m�o de cartas jogador 2
    int pos; //vari�vel posi��o carta jogada
    Carta d; //vari�vel do tipo carta passada por par�metro na fun��o de desempilhar


    definir(p);//definindo e empilhando as cartas no monte de baralho
    embaralhar(p);//embaralhando as cartas

    int i;//vari�vel para usar no for
    ListaCarta *b;//ponteiro para receber a carta buscada
    Carta r1, r2;//vari�vel para guardar as cartas jogadas


    for(i=0; i<7; i++){ //distribuindo as cartas
        inserir(m1, desempilha(p, &d));
        inserir(m2, desempilha(p, &d));
    }
    Jogador j[2];
    int sorteio;

    printf("\n***BEM-VINDO AO JOGO UNO***\n"); //pegando o nome dos jogadores
    printf("\nInsira seu nome: ");
    scanf("%s", j[0].nome);
    printf("\nInsira seu nome: ");
    scanf("%s", j[1].nome);

    int tam1 = m1->tamanho;
    int tam2 = m2->tamanho;

    for(i =0; i<1; i++){//sorteio
        sorteio = rand()%2;
    }
    if(sorteio == 1){
        printf("\nJogador(a) 1: %s", j[sorteio].nome);
        j[sorteio].qt = tam1;
        printf("\nJogador(a) 2: %s", j[sorteio-1].nome);
        j[sorteio-1].qt = tam2;
    }else{
        printf("\nJogador(a) 1: %s", j[sorteio].nome);
        j[sorteio].qt = tam1;
        printf("\nJogador(a) 2: %s", j[sorteio+1].nome);
        j[sorteio+1].qt = tam2;
    }

    FILE *arquivo;//abrindo o arquivo
    arquivo = fopen ("arquivo.txt", "w");
    if( arquivo == NULL){
        printf("\nERRO");
    }else{
    }

    printf("\nCartas: ");//iniciando o jogo
    imprimeMao(m1);
    printf("\nPosi��o carta: ");
    scanf("%d", &pos);
    b = busca(m1, pos);
    if (b->c.valor > 9){//primeira jogada
        printf("\nCarta inv�lida");
        do{
            b = busca(m1, pos);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
        }while(b->c.valor < 9);
        r1 = retira(m1, pos);
        empilha(descarte, r1);
        fprintf (arquivo, "Carta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r1.cor, r1.valor);

    }else{
        r1 = retira(m1, pos);
        empilha(descarte, r1);
        fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r1.cor, r1.valor);

    }
    imprimeTopo(descarte);

    do{
        jogadas(m2, m1, descarte, p);
        imprimeTopo(descarte);
        jogadas(m1, m2, descarte,p);
        imprimeTopo(descarte);

    }while(tam1 != 0|| tam2 != 0);
    fclose(arquivo);

    return 0;
}
Baralho *criaBaralho(){//criando o baralho
    Baralho *p = (Baralho *)malloc(sizeof(Baralho));
    p->topo = -1;
    return p;
}
int empilha(Baralho *p, Carta dados){//empilhando as cartas
        if(p->topo == MAX){
            return 0; //pilha cheia
        }else{
            p->topo++;
            p->vet[p->topo] = dados;
            return 1;//empilhada
        }
}
Carta desempilha(Baralho *p, Carta *k){//
    if(p->topo == -1){//monte vazio
    }else{
        *k = p->vet[p->topo];
        p->topo--;
        return *k;
    }
}
void definir(Baralho *p){//definindo as cartas
    Carta dados;
    int i, j;
    for(i=0; i<2; i++){//amarelas
        for (j=0; j<10; j++){
            strcpy(dados.cor, "Amarela");
            dados.valor = j;
            empilha(p,dados);
        }
    }
    for(i=0; i<1; i++){//amarelas inverter
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Amarela inverter");
            dados.valor= 21;
            empilha(p, dados);
        }
    }

    for(i=0; i<1; i++){//amarelas passa a vez
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Amarela passar a vez");
            dados.valor= 23;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//amarelas  +2
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Amarela +2");
            dados.valor= 22;
            empilha(p, dados);
        }
    }
    for(i=0; i<2; i++){//azuis
        for(j=0; j<10; j++){
            strcpy(dados.cor, "Azul");
            dados.valor = j;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//azuis inverter
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Azul inverter");
            dados.valor= 21;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//azuis passa a vez
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Azul passar a vez");
            dados.valor= 23;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//azuis  +2
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Azul +2");
            dados.valor= 22;
            empilha(p, dados);
        }
    }
    for(i=0; i<2; i++){//verdes
        for(j=0; j<10; j++){
            strcpy(dados.cor, "Verde");
            dados.valor = j;
            empilha(p, dados);
        }
    }
       for(i=0; i<1; i++){//verdes inverter
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Verde inverter");
            dados.valor= 21;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//verdes passa a vez
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Verde passar a vez");
            dados.valor= 23;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//verdes  +2
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Verde +2");
            dados.valor= 22;
            empilha(p, dados);
        }
    }
    for(i=0; i<2; i++){
        for(j=0; j<10; j++){//vermelhas
            strcpy(dados.cor, "Vermelha");
            dados.valor = j;
            empilha(p, dados);
        }
    }
       for(i=0; i<1; i++){//vermelhas inverter
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Vermelha inverter");
            dados.valor= 21;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//vermelhas passa a vez
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Vermelha passar a vez");
            dados.valor= 23;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//vermelhas +2
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Vermelha +2");
            dados.valor= 22;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//curinga +4
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Curinga +4");
            dados.valor= 50;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//curinga escolhe a cor
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Curinga escolhe cor");
            dados.valor= 51;
            empilha(p, dados);
        }
    }
    for(i=0; i<1; i++){//curinga trocar m�os
        for(j=0; j<2; j++){
            strcpy(dados.cor, "Curinga trocar m�os");
            dados.valor= 52;
            empilha(p, dados);
        }
    }
}
void embaralhar(Baralho *p){//embaralhando a pilha de cartas
    srand(time(NULL));
    int i, posi1, posi2;
    Carta aux;

	for (i=-1; i <MAX-1; i++){
		do{
            posi1 = rand() %110;//gerando numeros aleat�rios
            posi2 = rand() %110;

		}while(posi1 == posi2);

		aux = p->vet[posi1];//trocando a posi��o das cartas
		p->vet[posi1] = p->vet[posi2];
		p->vet[posi2] = aux;
	}
}
MaoCarta *criaMaoCarta(){ //fun��o da lista de mao do jogador
    MaoCarta *m = (MaoCarta*)malloc(sizeof(MaoCarta));
    m->inicio = NULL;
    m->tamanho = 0;
    return m;
}
void inserir(MaoCarta *m, Carta dados){//fun��o para inserir cartas nas m�os dos jogadores
    ListaCarta *l = (ListaCarta *)malloc(sizeof(ListaCarta));

    l->c= dados;
    l->prox = m->inicio;
    m->inicio = l;
    m->tamanho ++;
}
void imprimeMao(MaoCarta *m){//fun��o para imprimir a m�o de carta dos jogadores
    ListaCarta *l;
    int i;
    i = 0;
    printf("\n");
    for(l = m->inicio; l!=NULL; l = l->prox){
        printf("[%d] Cor:%s \t Valor:%d", i, l->c.cor, l->c.valor);
        i++;
        printf("\n");
    }
}
ListaCarta *busca(MaoCarta *m, int indice){//fun��o para buscar a carta por posi��o "recupera"
    if (indice >=0 && indice < m->tamanho){
        ListaCarta *l = m->inicio;
        for(int i=0; i<indice; i++){
            l = l->prox;
        }
        return l;
    }else{
        return NULL;
    }
}
Carta retira (MaoCarta *m, int indice){//fun��o para retirar a carta da m�o do jogador
    ListaCarta *atual = busca(m, indice);
    Carta *dados;
    dados = atual;
    if(m->tamanho == 0){
      printf("\nLista vazia");
    }
    if(indice == 0){
        if(atual != NULL);
            ListaCarta *aux = m->inicio;
            m->inicio = aux->prox;
            m->tamanho--;
            return *dados;
            free(aux);
    }
    else{
        if(atual != NULL){
            ListaCarta *ant = busca(m, indice -1);
            ant->prox = atual->prox;
            m->tamanho--;
            return *dados;
            free(atual);
        }
    }

}
void imprimeTopo(Baralho *p){//fun��o para imprimir o topo do baralho
    printf("\nCarta topo:");
    printf("\nCor: %s Valor: %d", p->vet[p->topo].cor, p->vet[p->topo].valor);
    printf("\n");
}
void compra(MaoCarta *m, Baralho *p){//fun��o de compra de cartas
    Carta d;
    int aux;
    printf("\n[2]Comprar\n[0]Encerrar compra\n");
    imprimeMao(m);
    while(aux != 0 ){
        scanf("%d", &aux);
        inserir(m, desempilha(p, &d));
        imprimeMao(m);
    }
}
void jogadas(MaoCarta *m, MaoCarta *mm, Baralho *descarte, Baralho *p){//fun��o das jogadas
    int op, pos;
    ListaCarta *b;
    Carta r;
    Jogador j;

    printf("\n[1] Jogar carta");
    printf("\n[2] Comprar carta no monte");
    printf("\n[3] UNO");

    printf("\nCartas: ");
    imprimeMao(m);

    scanf("%d", &op);

    switch (op)
    {
    case 1:
        printf("\nPosi��o carta: ");
        scanf("%d", &pos);
        jogarCarta(m, mm, pos, descarte, p);

        break;
    case 2:
        compra(m, p);
        printf("\nPosi��o carta: ");
        scanf("%d", &pos);
        jogarCarta(m, mm, pos, descarte, p);
        break;
    case 3:
        printf("\nJogador j� pode jogar sua �ltima carta!");
        printf("\nPosi��o carta: ");
        scanf("%d", &pos);
        r = retira(m, pos);
        if(m->tamanho == 0){
            printf("\nRealizar contagem jogador perdedor");
            contagem(mm, j);
        }
    }
}
void jogarCarta(MaoCarta *m, MaoCarta *mm, int pos, Baralho *descarte, Baralho *p){//fun��o que verifica qual carta est� sendo jogada
    Carta r;
    ListaCarta *b;
    FILE *arquivo;
    char cor[10];
    char corA[10] = {"Amarela"};
    char corZ[10] = {"Azul"};
    char corV[10] = {"Verde"};
    char corE[10] = {"Vermelha"};

    char curinga[30] = {"Curinga +4"};
    char eCuringa[30] = {"Curinga escolhe cor"};
    char tCuringa[30] = {"Curinga trocar m�os"};

    b = busca(m, pos);

    int a = strncmp(b->c.cor, descarte->vet[descarte->topo].cor, 4);
    int f = strcmp(b->c.cor, descarte->vet[descarte->topo].cor);
    int c = strcmp(b->c.cor, eCuringa);
    int d = strcmp(b->c.cor, curinga);
    int e = strcmp(b->c.cor, tCuringa);

    if(descarte->vet[descarte->topo].valor <=9){//TOPO CARTA COMUM
        if(b->c.valor == descarte->vet[descarte->topo].valor || f == 0){//carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(a ==0 && b->c.valor == 21){//inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a ==0 && b->c.valor == 23){//passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPassa a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
         else if( c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
            jogadas(mm, m, descarte, p);
        }else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
    fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 21){ // TOPO INVERTER
        if(b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(a== 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
            jogadas(mm, m, descarte, p);
        }else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
        fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 23){ //TOPO PASSA A VEZ
        if( b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(a == 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
         else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
            jogadas(mm, m, descarte, p);
        }else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
        fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 22){ //TOPO +2
        if( b->c.valor == 22){//caso seja carta +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar duas cartas no monte, continua o mesmo jogador");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(a == 0 && b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if (e  == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%d", cor);
            jogadas(mm, m, descarte, p);
        }else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
        fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 50){//TOPO +4
        if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);

        }
        else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%d", cor);
            jogadas(mm, m, descarte, p);
        }
        else if(b->c.valor <=9){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(a == 0 && b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(a == 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d",&pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
        fprintf (arquivo, "Carta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 51){//TOPO ESCOLHE COR
        if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%d", cor);
            jogadas(mm, m, descarte, p);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0 && b->c.valor <=9){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0 && b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0&& b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE) == 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
    fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);

    }else if (descarte->vet[descarte->topo].valor == 52){//TOPO TROCAR M�OS
        if (d == 0){//caso seja um curinga +4
           r = retira(m, pos);
           empilha(descarte, r);
           printf("\nPr�ximo jogador dever� comprar 4 cartas no monte e perde a vez!");
           compra(mm, p);
           imprimeMao(m);
           printf("\nPosi��o carta: ");
           scanf("%d", &pos);
           jogarCarta(m, mm, pos, descarte, p);
        }
        else if(c == 0){//caso seja uma carta escolhe cor
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nEscolha a cor para da carta do pr�ximo jogador!");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%s", cor);
        }
        else if (e == 0){//caso seja de trocar m�os
            r = retira(m, pos);
            empilha(descarte, r);
            MaoCarta *aux = criaMaoCarta();
            aux = m;
            m = mm;
            mm = aux;
            printf("\nCartas trocadas. Escolha a cor para o pr�ximo jogador");
            printf("\n[-]Amarela\n[-]Azul\n[-]Verde\n[-]Vermelha\n");
            scanf("%d", cor);
            jogadas(mm, m, descarte, p);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0&& b->c.valor <=9){//caso seja uma carta comum
            r = retira(m, pos);
            empilha(descarte, r);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0 && b->c.valor == 21){//caso seja carta especial seja de inverter
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nOrdem do jogo invertida, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if(strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0 && b->c.valor == 23){//caso seja carta especial seja de passar a vez
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPerdeu a vez, continua o mesmo jogador");
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else if (strcmp(b->c.cor, corA)== 0 || strcmp(b->c.cor, corZ)== 0 || strcmp(b->c.cor, corV)== 0 || strcmp(b->c.cor, corE)== 0 && b->c.valor == 22){//caso seja carta especial seja de +2
            r = retira(m, pos);
            empilha(descarte, r);
            printf("\nPr�ximo jogador deve comprar 2 cartas no monte e perde a vez");
            compra(mm, p);
            imprimeMao(m);
            printf("\nPosi��o carta: ");
            scanf("%d", &pos);
            jogarCarta(m, mm, pos, descarte, p);
        }
        else{
            printf("\nCarta inv�lida");
            jogadas(m, mm, descarte, p);
        }
        fprintf (arquivo, "\nCarta topo: Cor: %s\tValor:%d\nCarta jogada: Cor: %s\tValor:%d", descarte->vet[descarte->topo].cor,descarte->vet[descarte->topo].valor, r.cor, r.valor);
    }
}
void contagem(MaoCarta *m, Jogador j){//fun��o para contagem dos pontos
    FILE *arquivo;
    ListaCarta *l = m->inicio;
    while(l != NULL){
        if(l->c.valor <= 9){
            j.pontos+=5;
        }else if(l->c.valor == 21 || l->c.valor == 22 || l->c.valor == 23){
            j.pontos+=20;
        }else if (l->c.valor == 50 || l->c.valor == 51 || l->c.valor == 52){
            j.pontos+=50;
        }
    }
    printf("\nPontos: %d", j.pontos);
    fprintf(arquivo, "Vencedor: %s\nPontos: %d", j.nome, j.pontos);
}


