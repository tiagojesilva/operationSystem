#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_ITENS 2 // 0, 1 e 2 é um vetor
#define LOOP 2000

int buffer[N_ITENS];

int IsFull  = 0;                                     //Variable de condicion: 1= buffer lleno, 0= buffer no lleno.
int IsEmpty = 1;                                     //Variable de condicion: 0= buffer no vacio, 1= buffer vacio.

int pos=0;

pthread_mutex_t mut;
pthread_cond_t NotEmpty;
pthread_cond_t NotFull;


void* productor(void *v) 
{
 int i=0;
  for ( i = 0; i < LOOP; i++)
   {
     pthread_mutex_lock (&mut);
     while (IsFull) 
	   { 
		   pthread_cond_wait (&NotFull, &mut);
 	   } 
     printf("\n\r  Produtor Inserindo Informacao no Buffer \n\r,[%d]",pos);
     buffer[pos] = i;
     pos++;
     if (pos==N_ITENS)
     {
       //pos=0;
	     IsFull=1;
     }
     IsEmpty=0;
     pthread_mutex_unlock (&mut);
     pthread_cond_signal (&NotEmpty);
     sleep(random() % 3);                          
  }
  return (NULL);
}

void* consumidor(void *v)
{
   int i=0;
   for ( i=0; i < LOOP; i++) 
   { 
	  pthread_mutex_lock (&mut);
	  while (IsEmpty)
	    {		
		  pthread_cond_wait (&NotEmpty, &mut);
	    }
      printf("\n\r  Consumidor retirando valores do buffer: %d.\n", pos);
      pos--;
      if (pos==0)
          IsEmpty=1;
      else
          IsEmpty=0;
      IsFull=0;
      pthread_mutex_unlock (&mut);
      pthread_cond_signal (&NotFull);
      sleep(random() % 3);                 /* Permite que a outra thread execute */  
  }
  return (NULL);
}

int main(void) 
{
  pthread_t thr_productor, thr_consumidor;
  int i=0;
  for ( i=0; i < N_ITENS; i++)  // Limpa buffer
       buffer[i] = 0;

  pthread_mutex_init (&mut,NULL);

  pthread_cond_init (&NotFull,NULL);
  pthread_cond_init (&NotEmpty,NULL);

  pthread_create(&thr_productor, NULL, productor, NULL);
  printf("\n\r  *** MAIN, T1 criada\n\r");


  pthread_create(&thr_consumidor, NULL, consumidor, NULL);
  printf("\n\r  *** MAIN, T2 criada\n\r");

    pthread_join(thr_productor, NULL); 
  printf("\n\r  *** MAIN, join T1 criada\n\r");


  pthread_join(thr_consumidor, NULL);
  printf("\n\r  *** MAIN, Join T2 criada\n\r");  

  pthread_mutex_destroy (&mut);

  return (0);
}
