/**
 * @file JobQueue.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A Linked List for storing Jobs in a First in First out (FIFO) data structure.
 */

#ifndef QUEUE_H
#define QUEUE_H
// linked list

#include "Job.h"

/**
 * @brief Node that stores the job and the next node
 */
typedef struct Node_
{
    Job j; /**< The job data to store*/
    struct Node_ *next; /**< The next node*/
} Node;

/**
 * @brief A Linked List for storing Jobs in a First in First out (FIFO) data structure.
 */
typedef struct JobQueue_
{
    unsigned int size; /**< @brief The number of nodes in the list */
    Node *root;

    Job (*pop)(struct JobQueue_ *self); /**< @brief Return the next node in the list/queue */
    void (*push)(struct JobQueue_ *self, Job j); /**< @brief Add a node to the end of the list */
    void (*destroy)(struct JobQueue_ *self); /**< @brief Delete all nodes in the list */
    unsigned int (*length)(struct JobQueue_ *self); /**< @brief Return the number of items in the list */
    int (*isEmpty)(struct JobQueue_ *self); /**< @brief Return 1 if the list is empty, otherwise 0. */
    void (*print)(struct JobQueue_ *self); /**< @brief Print the contents of the list to the screen */
} JobQueue;

JobQueue newJobQueue();

#endif
