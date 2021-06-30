#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>


int* buffer;
int tamanho_buffer;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int ultimo_slot = 0;

void* produtor(void *arg){
    int valor;
    while(1){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        valor = rand() % 100; //inteiro aleatório entre 0 e 99
        buffer[ultimo_slot] = valor;
        imprime_buffer();
        printf("Produtor produziu o valor: %d na posição %d \n\n", valor, ultimo_slot);
        ultimo_slot++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(rand() %5);
    }
}

void* consumidor(void *arg){
    int valor;
    while(1){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        valor = buffer[ultimo_slot-1];
        buffer[ultimo_slot-1] = -1;
                imprime_buffer();
        printf("Consumidor consumiu o valor: %d na posição %d \n\n", valor, ultimo_slot-1);
        ultimo_slot--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(rand() %2);
    }
}

void imprime_buffer(){
    for(int i = 0; i < tamanho_buffer; i++){
        if (buffer[i] == -1){
            printf("[%d]         .\n", i);
        }
        else{
            printf("[%d]         %d\n", i, buffer[i]);
        }
    }
}
int main() {
    setlocale(LC_ALL, "Portuguese");

    //obtendo os valores do usuário
    int m;
    int n;
    while (1){
    printf("Digite o número de produtores: ");
    scanf("%d", &m);
    printf("Digite o número de consumidores: ");
    scanf("%d", &n);
    printf("Digite o tamanho do buffer: ");
    scanf("%d", &tamanho_buffer);
    if (tamanho_buffer <= 0 || m <= 0 || n <=0){
        printf("O número de produtores, consumidores e o tamanho do buffer devem ser maiores que 0. \n");
    }else{
        break;
    }
    }
    
    //inicializando as variáveis
    pthread_t prod[m], cons[n];
    buffer =(int*) malloc(sizeof(int)*tamanho_buffer);
    for(int i = 0; i < tamanho_buffer; i++){
        buffer[i] = -1;
   }
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,tamanho_buffer);
    sem_init(&full,0,0);

    //inicializando as threads produtoras
    for(int i = 0; i<m; i++){
         pthread_create(&prod[i], NULL, (void *)produtor, NULL);
    }
    //inicializando as threads consumidoras
        for(int i = 0; i<n; i++){
        pthread_create(&cons[i], NULL, (void *)consumidor, NULL);
    }

    for(int i = 0; i<m; i++){
        pthread_join(prod[i], NULL);
    }

    for(int i = 0; i<n; i++){
        pthread_join(cons[i], NULL);
    }
}
