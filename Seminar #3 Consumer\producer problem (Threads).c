#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;


static int stock = 0;
static int secCons, secProd, numConsume, numProduce;

static void* thread_produce(void *args) {
    int producerid = *((int *) args) + 1;
    int ret, restock, prevstock;

    while(true){
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0) return NULL;
        
        if (stock < 100){
            prevstock = stock;
            restock = (rand() % 20) + 1;
            stock = stock + restock;

            if (stock > 100)
                stock = 100;

            printf("\nProducer %d added to stock!\nInventory went from %d to %d\n",producerid,prevstock,stock);
            
            if (stock == 100){
                printf("\nStock is now full!\n");
                while (stock == 100){
                    printf("\nProducer %d is waiting for the stock to be empty.\n", producerid);
                    for (int i = 0; i < numConsume; i++){
                        ret = pthread_cond_signal(&cond_full);
                        if(ret != 0) return NULL;
                    }
                    ret = pthread_cond_wait(&cond_empty, &mtx); 
                    if(ret != 0) return NULL;
                }
            }
        } 
        else{
            for (int i = 0; i < numConsume; i++){
                ret = pthread_cond_signal(&cond_full);
                if(ret != 0) return NULL;
            }
            while(stock != 0){
                printf("\nProducer %d is waiting for the stock to be empty.\n", producerid);
                for (int i = 0; i < numConsume; i++){
                        ret = pthread_cond_signal(&cond_full);
                        if(ret != 0) return NULL;
                    }
                ret = pthread_cond_wait(&cond_empty, &mtx);
                if(ret != 0) return NULL;
            }

        }
        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0) return NULL;
        usleep(1000000*secProd); //sleep for 1000*100 useconds
    }
    return NULL;
}

static void* thread_consume(void* args){
    int consumerid = *((int *) args) + 1;
    int ret, buy, prevstock;
    int laststock = 0;

    while(true){
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0) return NULL;

        while(stock == 0){
            printf("\nConsumer %d is waiting for the stock to be full.\n", consumerid);
            ret = pthread_cond_wait(&cond_full, &mtx);
            if(ret != 0) return NULL;
        }

        if (stock > 0){
            prevstock = stock;
            buy = (rand() % 5) + 1;
            stock = stock - buy;
            if (stock < 0){
                laststock = 1;
                stock = 0;
                ret = pthread_cond_signal(&cond_empty);
                if(ret != 0) return NULL;
            }
            printf("\nConsumer %d has purchased %d units.\nInventory went from %d to %d\n", consumerid, prevstock-stock, prevstock, stock);
            if (laststock == 1){
                printf("\nConsumer %d has bought the last units in stock.\n", consumerid);
                laststock = 0;
            }
            while(stock == 0){
                printf("\nConsumer %d is waiting for the stock to be full.\n", consumerid);
                ret = pthread_cond_signal(&cond_empty);
                if(ret != 0) return NULL;
                ret = pthread_cond_wait(&cond_full, &mtx);
                if(ret != 0) return NULL;
            }
        }
        else{
            while(stock == 0){
                printf("\nConsumer %d is waiting for the stock to be full.\n", consumerid);
                ret = pthread_cond_signal(&cond_empty);
                if(ret != 0) return NULL;
                ret = pthread_cond_wait(&cond_full, &mtx);
                if(ret != 0) return NULL;
            }
        }
        ret = pthread_mutex_unlock(&mtx);
        if(ret != 0) return NULL;
        usleep(1000000*secCons); //sleep for 1000*100 useconds
    }
    return NULL;
}

int main(int argc, char*argv[]){

    
    printf("Enter the number of Consumers: ");
    scanf("%d", &numConsume);
    printf("Enter the number of Producers:");
    scanf("%d", &numProduce);
    printf("Enter when each consumer shows up in seconds: ");
    scanf("%d", &secCons);
    printf("Enter when each producer shows up in seconds: ");
    scanf("%d", &secProd);
    
    pthread_t consumers[numConsume];
    pthread_t producers[numProduce];

    int ret;
    printf("\n");
    for (int i = 0; i < numConsume; i++){
        ret = pthread_create(&consumers[i], NULL, thread_consume, &i);
        usleep(100000);
    }
    for (int j = 0; j < numProduce; j++){
        ret = pthread_create(&producers[j], NULL, thread_produce, &j);
         usleep(100000);
    }


    for (int i = 0; i < numConsume; i++)
        pthread_join(consumers[i],NULL);
        
    for (int i = 0; i < numProduce; i++)
        pthread_join(producers[i],NULL);

    return 0;
    
}
