#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
        if (q == NULL)
                return 1;
        return (q->size == 0);
}
void enqueue(struct queue_t *q, struct pcb_t *proc)
{
        if (q == NULL)
        {
                perror("Queue is NULL\n");
                exit(1);
        }
        if (q->size == MAX_QUEUE_SIZE)
        {
                perror("Queue is full\n");
                exit(1);
        }
        q->proc[q->size] = proc;
        q->size++;
}

struct pcb_t *dequeue(struct queue_t *q)
{
        /* TODO: return a pcb whose prioprity is the highest
         * in the queue [q] and remember to remove it from q
         * */
        if (q == NULL || q->size == 0)
                return NULL;
        struct pcb_t *temp = NULL;
#ifdef MLQ_SCHED
        temp = q->proc[0];
        for (int i = 0; i < q->size - 1; i++)
        {
                q->proc[i] = q->proc[i + 1];
        }
        q->proc[q->size - 1] = NULL;
        q->size--;
        return temp;
#else
        temp = q->proc[0];
        index = 0;
        for (int i = 1; i < q->size; i++)
        {
                if (q->proc[i]->priority <= temp->priority)
                {
                        index = i;
                        temp = q->proc[i];
                }
        }
        q->proc[i] = q->proc[q->size - 1];
        q->proc[q->size - 1] = NULL;
        q->size--;
        return temp;

#endif
}
