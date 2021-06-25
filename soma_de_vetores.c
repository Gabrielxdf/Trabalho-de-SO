#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <locale.h>
#include <time.h>

//declaração das variáveis globais
float *x;
float *y;
float *z;
pthread_mutex_t *mutexes;
pthread_cond_t pode_somar = PTHREAD_COND_INITIALIZER; // sinalizado quando z puder somar o x e o y

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
    Thread_preenche *thread_t = (Thread_preenche*)argPtr;
    unsigned int inicio = thread_t->posInicial;
    unsigned int final = thread_t->posFinal;
    for(;inicio <= final; inicio++){
        thread_t->vetor[inicio] = (float)rand()/(float)(RAND_MAX/1.0);
    }

    /*os vetores são inicializados com números negativos, o código abaixo
    é preciso para apenas liberar o cálculo do vetor z quando
    os vetores x e y estiverem de fato populados */
    if (x[final] >=0.0 && y[final] >=0.0) {
        pthread_cond_signal(&pode_somar);
        pthread_mutex_unlock(&mutexes[thread_t->contMutexThread]);
    }
    pthread_exit(0);
}

void *somaVetores(void* argPtr){
    Thread_soma *thread_z = (Thread_soma*)argPtr;
    unsigned int inicio = thread_z->posInicial;
    unsigned int final = thread_z->posFinal;
    pthread_mutex_lock(&mutexes[thread_z->contMutexThread]);
    pthread_cond_wait(&pode_somar, &mutexes[thread_z->contMutexThread]);
    for(;inicio <= final; inicio++){
        thread_z->z[inicio] = thread_z->x[inicio] + thread_z->y[inicio];
    }
    pthread_exit(0);
}

void imprime_resultados(int tamanho_vetor){
    for (int i = 0; i < tamanho_vetor; i++){
        printf("[%u]    x = %f    y = %f    z = %f \n", i, x[i], y[i], z[i]);
    }
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

    //inicialização dos vetores e variáveis
    x =(float*) malloc(sizeof(float)*tamanho_vetor);
    y =(float*) malloc(sizeof(float)*tamanho_vetor);
    z =(float*) malloc(sizeof(float)*tamanho_vetor);
    mutexes = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*n);
    
    int divisao = tamanho_vetor / n;
    clock_t inicio = clock();
    pthread_t thread_x[n];
    pthread_t thread_y[n];
    pthread_t thread_z[n];
    Thread_preenche thread_preenche_x[n];
    Thread_preenche thread_preenche_y[n];
    Thread_soma thread_soma_z[n];

    //inicialização das threads e mutexes.
    for (int i = 0; i < n; ++i){
        int posInicial = i*divisao;
        int posFinal = i*divisao+divisao-1;
        //para o x
        thread_preenche_x[i].vetor = &x[0];
        thread_preenche_x[i].posInicial = posInicial;
        thread_preenche_x[i].posFinal = posFinal;
        thread_preenche_x[i].contMutexThread = i;

        //para o y
        thread_preenche_y[i].vetor = &y[0];
        thread_preenche_y[i].posInicial = posInicial;
        thread_preenche_y[i].posFinal = posFinal;
        thread_preenche_y[i].contMutexThread = i;

        //para o z
        thread_soma_z[i].x = &x[0];
        thread_soma_z[i].y = &y[0];
        thread_soma_z[i].z = &z[0];
        thread_soma_z[i].posInicial = posInicial;
        thread_soma_z[i].posFinal = posFinal;
        thread_soma_z[i].contMutexThread = i;

        printf("A thread %u processará os elementos dos vetores nas posições de %u a %u \n", i,
        posInicial, posFinal);

        pthread_mutex_init(&mutexes[i], NULL);
        pthread_create(&thread_x[i], NULL, preencheVetores, &(thread_preenche_x[i]));
        pthread_create(&thread_y[i], NULL, preencheVetores, &(thread_preenche_y[i]));
        pthread_create(&thread_z[i], NULL, somaVetores, &(thread_soma_z[i]));
    }

    for (int i = 0; i < n; i++){
        pthread_join(thread_x[i], NULL);
        pthread_join(thread_y[i], NULL);
        pthread_join(thread_z[i], NULL);
    }
    clock_t fim = clock();
    float tempo_gasto = (float)(fim - inicio) / CLOCKS_PER_SEC;

    //impressão dos resultados.
    char escolha;
    while (1){
    printf("Deseja imprimir os resultados ? S/N ");
    scanf(" %c", &escolha);
    if (escolha == 'S' || escolha == 's'){
        imprime_resultados(tamanho_vetor);
        break;
    }else if (escolha == 'N' || escolha == 'n'){    
        break;
    }
    }

    printf("Tempo de execução: %f segundo(s). \n", tempo_gasto);

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