#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct 
{
    char nome[100];
    char sobreNome[100];
    char email[150];
    char cpf[11];
    char telefone[11];
} contato;

typedef struct node
{
    struct node* previous;
    struct node* next;
    contato dado;
} node;

typedef struct
{
    node* first;
    node* last;
    int size;
} headerList;


//Funcao que cria o no: recebe o endereco do contato
struct node* createNode(contato c){
    node *newNode = (node*) malloc(sizeof(node));
    if (newNode != NULL)
    {   
        printf("Node criado com sucesso\n");
        newNode->dado = c;
        return newNode;
    }else return NULL;    
};

//funcao pra transformar uma string em lowercase
void toLowerCase(char str[]) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}


//Funcao que vai dizer se a inicial de uma palavra é maior que a outra: deve receber primeiro a string do nó existente e depois a do nó novo
bool isBefore(char word1[100], char word2[100]){
    char word1Lower[100]; 
    char word2Lower[100];
    strcpy(word1Lower, word1);
    strcpy(word2Lower, word2);
    toLowerCase(word1Lower);
    toLowerCase(word2Lower);
    if (strcmp(word1Lower, word2Lower) < 0){
        return true;
    }else return false;
};

//Retorna a posicao que do contato que deve ficar no next do contato criado. Se for o último, retorna NUll
node* findPosition(node *n, contato c){
    //Condição que vai pegar o último nó e retornar se o novo nó deve ficar antes ou depois dele
    if(n->next == NULL){
        if(isBefore(n->dado.sobreNome, c.sobreNome) == true){
            return NULL;
        }else return n;
    }else{
        if(isBefore(n->dado.sobreNome, c.sobreNome) == false){
            return n;
        }
        else return findPosition(n->next, c);
    }
}



//Funcao que vai adicionar um nó na lista
void addToList(contato c, headerList *header){
    if(header->size == 0){
        //Cria a lista
        printf("\n-----\nLista Criada\n-----\n");
        node *newNode = createNode(c);
        newNode->previous= NULL;
        newNode->next= NULL;
        header->first = header->last = newNode;
        header->size++;
        }
    else{
        node *newNodeNextNode = findPosition(header->first, c);
        if(newNodeNextNode == NULL){
            //Se o novo contato for pra última posição  da lista
            node *newNode = createNode(c);
            newNode->next = NULL;
            header->last->next = newNode;
            newNode->previous = header->last;
            header->last = newNode;
            header->size++;
        }else if (newNodeNextNode == header->first){
            //Se o novo contato for pra primeira posição da lista
            node *newNode = createNode(c);
            newNode->next = header->first;
            newNode->previous = NULL;
            header->first->previous = newNode;
            header->first = newNode;
            header->size++;
        }else{
            //Quando o novo contato fica no meio
            node *pAuxPrevious;
            node *newNode = createNode(c);
            pAuxPrevious = newNodeNextNode -> previous;
            newNode->next= newNodeNextNode;
            pAuxPrevious->next = newNode;
            newNode->previous = pAuxPrevious;
            newNodeNextNode->previous = newNode;
            header->size++;
        }       
    }    
}
void printList(node *no,int cont){
    //Imprime a lista. Deve receber o HeaderFirst e o número 1 que inicia a contagem.
    if (no == NULL){
        return;
    }
    else{
        printf("\n\n\n-------------------------------------------------------\n");
        printf("\nContato %d\n", cont);
        printf("\nNome: %s", no->dado.nome);
        printf("\nSobrenome: %s", no->dado.sobreNome);
        printf("\nEmail: %s", no->dado.email);
        printf("\nCpf: %s", no->dado.cpf);
        printf("\nTelefone: %s", no->dado.telefone);
        printf("\nEndereco de memoria do contato: %p ", no);
        printf("\nEndereco de memoria do anterior: %p ", no->previous);
        printf("\nEndereco de memoria do próximo: %p ", no->next);
        
        if(no->next != NULL){
            printf("\nSobrenome do proximo: %s", no->next->dado.sobreNome);
        }
        printList(no->next, cont+1);
    }
}

void printContatoByIndex(int index, node *no, int cont){
    //Recebe a posicao, o primeiro nó da lista e o número 1 para iniciar a contagem
    if (cont == index){
        printf("\n\n\n-------------------------------------------------------\n");
        printf("\nContato solicitado: %d\n", cont);
        printf("\nNome: %s", no->dado.nome);
        printf("\nSobrenome: %s", no->dado.sobreNome);
        printf("\nEmail: %s", no->dado.email);
        printf("\nCpf: %s", no->dado.cpf);
        printf("\nTelefone: %s", no->dado.telefone);
        printf("\nEndereco de memoria do contato: %p ", no);
        printf("\nEndereco de memoria do anterior: %p ", no->previous);
        printf("\nEndereco de memoria do próximo: %p ", no->next);
    }else return printContatoByIndex(index, no->next, cont+1);
}

int deleteFromListByIndex(int index, node *no, int cont, headerList *header){
    if(index == header->size && header->size==1){
        //Se a lista só tem um elemento
        header->first = NULL;
        header->last = NULL;
        header->size = 0;
        free(no);
        return 0;
    }else if(index == 1){
        //Se é o primeiro elemento da lista
        node *paux = header->first->next;
        header->first = paux;
        paux->previous = NULL;
        free(no);
        header->size--;
        return 0;
    }else if(index == header->size){
        //se é o último elemtno da lista
        node *paux = header->first->previous;
        header->last = paux;
        paux->next = NULL;
        free(no);
        header->size--;
        return 0;
    }else{
        //Deleta do meio da lista
        if (index == cont){
            node *paux = no->previous;
            paux->next = no->next;
            no->next->previous = paux;
            free(no);
            header->size--;
            return 0;
        }else return deleteFromListByIndex(index, no->next, cont+1, header);
    }
}

void printHeader(headerList *head){
    printf("\n\n----------\nHeader");
    printf("\nPrimeiro contato %s %s", head->first->dado.nome, head->first->dado.sobreNome);
    printf("\nUltimo contato %s %s", head->last->dado.nome, head->last->dado.sobreNome);
    printf("\nTamanho da lista: %d", head->size);

}


int main(int argc, char const *argv[])
{
    headerList *header = (headerList*)malloc(sizeof(headerList));;
    int indexToRemove;
    int indexToFind;
    contato contatoAux;

    header->first = NULL;
    header->last = NULL; 
    header->size = 0;  
    
    bool usingList = true;

    printf("Bem vindo a lista");


    while (usingList){
        int userPick = 0;
        
        printf("\nMenu de opções\n\n\n");
        printf("1- Adicionar contato\n2- Remover contato\n3- Consultar contato\n4- Listar contatos\n\n5-Encerrar lista\n\n");
        scanf("%d", &userPick);

        switch (userPick) {
            case 1:
                // Adicionar contato
                printf("\nDigite o nome: ");
                scanf("%s", contatoAux.nome);
                printf("\nDigite o sobrenome: ");
                scanf("%s", contatoAux.sobreNome);
                printf("\nDigite o email: ");
                scanf("%s", contatoAux.email);
                printf("\nDigite o cpf: ");
                scanf("%s", contatoAux.cpf);
                printf("\nDigite o telefone: ");
                scanf("%s", contatoAux.telefone);

                addToList(contatoAux, header);
                break;

            case 2:
                printf("\nDigite o índice do contato a ser removido: ");
                scanf("%d", &indexToRemove);

                if (indexToRemove < 1 || indexToRemove > header->size) {
                    printf("\nÍndice inválido. Tente novamente.");
                } else {
                    deleteFromListByIndex(indexToRemove, header->first, 1, header);
                    printf("\nContato removido com sucesso.");
                }
                break;

            case 3:
                printf("\nDigite o índice do contato a ser consultado: ");
                scanf("%d", &indexToFind);

                if (indexToFind < 1 || indexToFind > header->size) {
                    printf("\nÍndice inválido. Tente novamente.");
                } else {
                    printContatoByIndex(indexToFind, header->first, 1);
                }
                break;

            case 4:
                if(header->size == 0){
                    printf("\nA lista está vazia. nada a ser consultado");
                }else printList(header->first, 1);
                break;

            case 5:
                usingList = false;
                break;

            default:
                printf("\nOpção inválida. Tente novamente.");
                break;
        }
    }
    free(header);

}





