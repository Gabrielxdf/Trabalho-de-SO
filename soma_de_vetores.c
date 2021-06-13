#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>
#include <time.h>

//declaração das variáveis globais
float *x;
float *y;
float *z;
pthread_mutex_t *mutexes;

typedef struct
{
    float* vetor; //ponteiro para o início do vetor que será preenchido;
    unsigned int posInicial; //posição inicial de preenchimento do vetor;
    unsigned int posFinal; //posição final de preenchimento do vetor;
    unsigned int contMutexThread; //contador que indicará qual dos 𝑁 mutexes deve ser utilizado para controlar a região crítica da thread;
}Thread_preenche;

typedef struct
{
    float* x; //ponteiro para o início do vetor x;
    float* y; //ponteiro para o início do vetor y;
    float* z; //ponteiro para o início do vetor z;
    unsigned int posInicial; //posição inicial de preenchimento do vetor;
    unsigned int posFinal; //posição final de preenchimento do vetor;
    unsigned int contMutexThread; //contador que indicará qual dos 𝑁 mutexes deve ser utilizado para controlar a região crítica da thread;
}Thread_soma;


int main() {
    setlocale(LC_ALL, "Portuguese");

    //obtendo o tamanho do vetor e o número de threads que o usuário deseja
    int tamanho_vetor;
    int n;
    while (1){
    printf("Digite o tamanho do vetor: ");
    scanf("%d", &tamanho_vetor);
    printf("Digite o número de threads: ");
    scanf("%d", &n);
    if (tamanho_vetor % n != 0){
        printf("O número de threads deve ser múltiplo do tamanho do vetor. \n");
    }else{
        break;
    }
    }

    //inicialização dos vetores
    x =(float*) malloc(sizeof(float)*tamanho_vetor);
    y =(float*) malloc(sizeof(float)*tamanho_vetor);
    z =(float*) malloc(sizeof(float)*tamanho_vetor);
    mutexes = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*n);

    //testes - apagar depois
    testes(tamanho_vetor);
    //fim dos testes

    free(x);
    free(y);
    free(z);
    free(mutexes);
    system("pause");
   return 0;
}

void* preencheVetores(void *argPtr){
    Thread_preenche *thread_p = (Thread_preenche*)argPtr;
}
void* somaVetores(void* argPtr);

void testes(int tamanho_vetor){
    Thread_preenche thread_p = {
    .vetor = &x[0],
    .posInicial = 0,
    .posFinal = 10,
    .contMutexThread = 1
    };
    //preencheVetores((void*)&thread_p);
    
    int i;
    // Get the elements of the array
    for (i = 0; i < tamanho_vetor; ++i) {
        x[i] = 1.2 + i;
            
    }
    // Print the elements of the array
    printf("The elements of the array are: ");
    for (i = 0; i < tamanho_vetor; ++i) {
        printf("%f, ", x[i]);
    }
    printf("%d \n", tamanho_vetor);
    // printf() displays the string inside quotation
    printf("Hello, World!\n");
}