#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t mutex;
int i = 0;
float lado[4];

// Macro para limpar o console. CLEAR_SCREEN: Limpa a tela

#ifdef _WIN32
#define CLEAR_SCREEN system("cls || clear");
#else
#define CLEAR_SCREEN puts("\x1b[H\x1b[2J");
#endif

void bhaskara();
void pitagoras();
void *calcula(void *tid);

int main()
{
    int op;

    do{
        printf("\n\n -----------------------------------------");
        printf("\n 1.......Calculadora de Bhaskara com Pipe.\n 2....Calculadora de Pitagoras com Thread.\n 0...................................Sair.\n");
        printf("------------------------------------------\n");
        printf(" Escolha uma opção: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
        CLEAR_SCREEN
            bhaskara();
            break;
        
        case 2:
        CLEAR_SCREEN
            for(i = 0; i < 4; i++)
            {
                lado[i] = 0;
            }
            i=0;
            pitagoras();
            break;
        
        case 0:
        CLEAR_SCREEN
        printf(" Você escolheu sair...\n\n");
            break;

        default:
            break;
        }
    
    }while(op !=0);
    return 0;
}

void bhaskara()
{
    int pipe1[2], pipe2[2];
    int descritor; 
    
    
    if (pipe(pipe1)<0 || pipe(pipe2) <0)
	{ printf("\n Erro na chamada PIPE");
	   exit(0);
	}

    if ( (descritor = fork()) <0)
	{ printf("\n Erro na chamada FORK");
	   exit(0);
	}

    if(descritor > 0) 
    {
        int num[3], delta;
        float raiz[2];

        close(pipe1[0]);
        close(pipe2[1]);

        printf("\n Calculadora de Bhaskara com Pipe:\n\n");
            
            printf(" Insira o coeficiente A: "); 
            scanf("%d", &num[0]);
            printf(" Insira o coeficiente B: "); 
            scanf("%d", &num[1]);
            printf(" Insira o coeficiente C: "); 
            scanf("%d", &num[2]);

            write(pipe1[1], &num, sizeof(num));
    
        
            read(pipe2[0], &delta, sizeof(delta)); 

            if(delta < 0){
            printf(" O Delta é: %d. Com isso não temos raízes!\n", delta);
            }

            if(delta >0){
            read(pipe2[0], &raiz, sizeof(raiz));
            printf(" O Delta é: %d e as raízes são %.1f e %.1f!\n",delta, raiz[0], raiz[1]);
            
            }
            if(delta == 0){
            read(pipe2[0], &raiz, sizeof(raiz));
            printf(" O Delta é: %d e a raíz única é %.1f!\n",delta, raiz[0]);
            
            }
            
            return;

        close(pipe2[0]);
        close(pipe1[1]);

    } else {
        int num[3], delta;
        float raiz[2];

        close(pipe1[1]);
        close(pipe2[0]);

        while(1)
        {
                read(pipe1[0], &num, sizeof(num) );

                delta = (pow(num[1],2)) - 4*num[0]*num[2];
                write(pipe2[1], &delta, sizeof(delta)); 
                
                if(delta >= 0){
                raiz[0] = (-num[1] + sqrt(delta)) / (2*num[0]);
                raiz[1] = (-num[1] - sqrt(delta)) / (2*num[0]);
                write(pipe2[1], &raiz, sizeof(raiz));
                
                }
            }
        }

        close(pipe2[1]);
        close(pipe1[0]);

    return;
}

void pitagoras()
{   
    int create;
    pthread_t t1, t2;
    long num;
    
    printf("\n Calculadora de Pitágoras com Thread:\n");
    pthread_mutex_init(&mutex, NULL);
    num = 1;
    create = pthread_create(&t1, NULL, calcula, (void *)num);
 
    num = 2;
    create = pthread_create(&t2, NULL, calcula, (void *)num);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
 
    pthread_mutex_destroy(&mutex);
    
    return;

}
void *calcula(void *tid){
    long id = (long)tid;
 
    while(i < 4)
    {
        pthread_mutex_lock (&mutex);

        if(i<2){
            printf("\n Thread %ld: Qual valor do lado: ", id);
            scanf("%f", &lado[i]);
        }
        if(i == 2){
            lado[i] = (sqrt((lado[0]*lado[0]) + (lado[1]*lado[1])));
            printf("\n Thread %ld: O valor pra hipotenusa é: %.1f",id, lado[i]);
        }

        if(i == 3){
            if(lado[0]>lado[1]){
                lado[i] = (sqrt((lado[0]*lado[0]) - (lado[1]*lado[1])));
                printf("\n Thread %ld: O valor para cateto é: %.1f",id, lado[i]);
            }

            if(lado[1]>=lado[0]){
                lado[i] = (sqrt((lado[1]*lado[1]) - (lado[0]*lado[0])));
                printf("\n Thread %ld: O valor pra cateto é: %.1f\n\n", id, lado[i]);
            }
        }
        i++;
        pthread_mutex_unlock (&mutex);
        sleep(0.1);

    }
    pthread_exit(NULL);
}