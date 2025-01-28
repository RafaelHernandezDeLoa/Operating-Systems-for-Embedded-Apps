#include <stdlib.h>
#include <stdio.h>

typedef struct _process { //typdef lets you provide existing data types with a new name
    int pid;            
    int size;
    int tr;
    struct _process * next;
} Process; //typedef is used here to rename the structure _process to "Process"

typedef struct{
    Process* head;
    Process* tail;
} Queue;

Process* create_process(int new_pid, int new_size){
    Process* result = (Process*) malloc(sizeof(Process));
    if(result == NULL) return NULL;
    result->next = NULL;
    result->pid = new_pid;
    result->size = new_size;
    result->tr = 0;
    return result;
}

Queue* create_queue() {
    Queue* result = (Queue*) malloc(sizeof(Queue));
    if(result == NULL) return NULL;
    result->head = NULL;
    result->tail = NULL;
    return result;
}

int add_process_end(Queue* q, Process* p){
    if(q == NULL)
        return -1;

    if(q->head == NULL){
        q->head = p;
        q->tail = p;
        return 0;
    }

    q->tail->next = p;
    q->tail = p;

    return 0;
}


Process* remove_process_start(Queue* q) {
    if(q == NULL || q->head == NULL)
        return NULL;
    
    Process* result = q->head;
    if (q->head == q->tail) {
        q->head = NULL;
        q->tail = NULL;
        return result;
    }

    q->head = q->head->next;
    return result;
}

int rr(Queue* q, int time_unit) {
    if(q == NULL)
        return -1;
    
    Process* p = remove_process_start(q);
    while(p != NULL) {
        // printf about how nice this process runs
        printf("\n Process [%d] running with initial size = %d\n", p->pid, p->size);
        p->size -= time_unit;
        printf("\n Process [%d] MIA with MIA size = %d \n", p->pid, p->size);

            if(p->size >0) add_process_end(q, p);
            else free(p);
        p = remove_process_start(q);
    }

    return 0;
}

int mlfq (Queue* q[],int num_queues, int timeslice){
    if(q[0] == NULL)
        return -1;
    int max_time = timeslice *2;
    Process* p_running;

    for (int i = 0; i < num_queues; i++){
        if (i + 1 == num_queues){
            printf ("\n\n Now running lowest priority queue in Round Robin\n\n");
            rr (q[i], timeslice);
        }

        else{
            printf ("\n\n Now running priority level %d queue in Round Robin\n\n", i + 1);
            p_running = remove_process_start(q[i]);
            while(p_running != NULL){

                printf("\n Process [%d] running with initial size = %d at priority level %d\n", p_running->pid, p_running->size, i + 1); //print

                p_running->size -= timeslice;  //process runs for this amount of time
                p_running->tr += timeslice;

                printf("\n Process [%d] MIA with MIA size = %d at priority level %d\n", p_running->pid, p_running->size, i + 1);

                if (p_running->size <= 0){ 
                    printf("\n Process [%d] has completed\n", p_running->pid); //print//the process is finished running
                    free(p_running);
                }
                else if (p_running->tr >= max_time){
                    printf("\n Process [%d] did not finish in max time and has been moved to priority level %d queue\n", p_running->pid, i + 2); //print
                    p_running->tr = 0;
                    add_process_end(q[i+1], p_running); //we add this process to the next lower priority queue
                }
                else
                    add_process_end(q[i], p_running);

                p_running = remove_process_start(q[i]); //remove the next process to start running
            }

        }
    
    }

    return 0;

}


int main() {
    //example case to run the above code
    Queue* q[3];
    int num_queues = sizeof(q)/sizeof(q[0]);

    for (int i = 0; i < num_queues; i++)
        q[i] = create_queue();
    
    add_process_end(q[0], create_process(1, 40));
    add_process_end(q[0], create_process(2, 20));
    add_process_end(q[0], create_process(3, 5));
    add_process_end(q[0], create_process(4, 10)); 
    add_process_end(q[0], create_process(5, 60));
    add_process_end(q[0], create_process(6, 25));
    add_process_end(q[0], create_process(7, 32));
    add_process_end(q[0], create_process(8, 45)); 

    mlfq(q, num_queues, 10);

    return 0;
}
