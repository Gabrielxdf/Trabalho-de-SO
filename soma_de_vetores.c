#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <locale.h>
#include <time.h>

//declaração das variáveis globais
unsigned int contador_thread = 0;
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

    printf("A thread %u processará os elementos dos vetores nas posições de %u a %u \n", contador_thread,
     thread_p->posInicial, thread_p->posFinal);
    contador_thread++;

    for(;inicio <= final; inicio++){
        pthread_mutex_lock(&mutexes[thread_p->contMutexThread]);
        thread_p->vetor[inicio] = (float)rand()/(float)(RAND_MAX/1.0); 
        //printf("x[%u] e o valor é: %f \n", inicio, x[inicio]);   
        //y[inicio] = (float)rand()/(float)(RAND_MAX/1.0); //resolver depois.
        //z[inicio] = x[inicio] + y[inicio];
        pthread_mutex_unlock(&mutexes[thread_p->contMutexThread]);
    }
    pthread_mutex_destroy(&mutexes[thread_p->contMutexThread]);
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

    clock_t inicio = clock();
    //inicialização das threads e mutexes.
    pthread_t thread[n];
    Thread_preenche thread_preenche[n];
    for (int i = 0; i < n; ++i){
        thread_preenche[i].vetor = &x[0];
        thread_preenche[i].posInicial = (divisao_vetor[i]);
        thread_preenche[i].posFinal = divisao_vetor[i+1]-1;
        thread_preenche[i].contMutexThread = i;

        pthread_mutex_init(&mutexes[i],NULL);
        pthread_create(&thread[i], NULL, preencheVetores, &(thread_preenche[i]));
    }

    for (int i = 0; i < n; i++){
        pthread_join(thread[i], NULL);
    }
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    //impressão dos resultados.
    for (int i = 0; i < tamanho_vetor; i++){
    //printf("[%u]    x = %f    y = %f    z = %f \n", i, x[i], y[i], z[i]);
    }
    printf("Tempo de execução: %lf segundo(s). \n", tempo_gasto);

    free(x);
    free(y);
    free(z);
    free(mutexes);

    #ifdef _WIN32
        //system("pause");
    #else __linux__
        system("read -p 'Press Enter to continue...\n' key");
    #endif
    
   return 0;
}

void *somaVetores(void* argPtr);

