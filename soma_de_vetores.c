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
    float* vetor;
    unsigned int posInicial;
    unsigned int posFinal;
    unsigned int contMutexThread;
}Thread_preenche;

typedef struct
{
    float* x;
    float* y;
    float* z;
    unsigned int posInicial;
    unsigned int posFinal;
    unsigned int contMutexThread;
}Thread_soma;

void *preencheVetores(void *argPtr){
    Thread_preenche *thread_p = (Thread_preenche*)argPtr;
    unsigned int inicio = thread_p->posInicial;
    unsigned int final = thread_p->posFinal;

    for(;inicio <= final; inicio++){
        pthread_mutex_lock(&mutexes[thread_p->contMutexThread]);
        thread_p->vetor[inicio] = (float)rand()/(float)(RAND_MAX/1.0);
        printf("x[%u] e o valor é: %f \n", inicio, x[inicio]);
        pthread_mutex_unlock(&mutexes[thread_p->contMutexThread]);
    }
    pthread_exit(0);
}

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
    int divisao_vetor[n+1];
    for (int i = 0; i <= n; i++){
        divisao_vetor[i] = ((tamanho_vetor/n)*(i));
    }
    pthread_t thread[n];
    for (int i = 0; i < n; ++i){
    pthread_mutex_init(&mutexes[i],NULL);
    Thread_preenche thread_preenche = {
        .vetor = &x[0],
        .posInicial = (divisao_vetor[i]),
        .posFinal = divisao_vetor[i+1]-1,
        .contMutexThread = i
    };
       pthread_create(&thread[i], NULL, preencheVetores, &(thread_preenche));
       //jeito errado, o join logo em seguida do create da thread.
       //pthread_join(thread[i], NULL);
    }
    //Assim seria o jeito certo de fazer, o join sendo depois
    //mas não está funcionando.
    for (int i = 0; i < n; i++){
        pthread_join(thread[i], NULL);
    }

    free(x);
    free(y);
    free(z);
    free(mutexes);

    #ifdef _WIN32
        system("pause");
    #else __linux__
        system("read -p 'Press Enter to continue...\n' key");
    #endif
    
   return 0;
}

void *somaVetores(void* argPtr);

