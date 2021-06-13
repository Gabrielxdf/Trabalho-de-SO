#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>
#include <time.h>

//declaração das variáveis globais
float *x;
float *y;
float *z;
float *mutexes;

int main() {
    setlocale(LC_ALL, "Portuguese");

    //obtendo o N e o tamanho do vetor
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
    mutexes = (int*) malloc(sizeof(int)*n);
    
    
    
    
    
    //testes
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
    printf("%d \n", n);
    // printf() displays the string inside quotation
    printf("Hello, World!\n");


    free(x);
    free(y);
    free(z);
    free(mutexes);
    system("pause");
   return 0;
}
