#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
#include <semaphore.h>
using namespace std;
pthread_mutex_t mutex;
// 生产者信号量
sem_t pSem;
// 消费者信号量
sem_t cSem;
const int MAXSIZE = 50;
struct Node
{
    int number;
    Node *next;
    Node()
    {
        this->next = NULL;
    }
    Node(int num)
    {
        this->number = num;
        this->next = NULL;
    }
    Node(const Node &rNode)
    {
        this->number = rNode.number;
        this->next = NULL;
    }
};
void geneChain(Node **head, int n)
{
    while (n != 0)
    {
        int num = rand() % 10;
        cout << num << " ";
        Node *newNode = new Node(num);
        if (*head == NULL)
        {
            *head = newNode;
        }
        else
        {
            newNode->next = *head;
            *head = newNode;
        }
        n--;
    }
    cout << endl;
}
void print(Node *head)
{
    Node *thisNode = head;
    while (thisNode != NULL)
    {
        cout << thisNode->number << " ";
        thisNode = thisNode->next;
    }
    cout << endl;
}
int size(Node *counthead)
{
    if (counthead == NULL)
        return 0;
    Node *countNode = counthead;
    int count = 0;
    while (countNode != NULL)
    {
        count++;
        countNode = countNode->next;
    }
    return count;
}
Node *head = NULL;
void *producerF(void *arg)
{
    int *id = (int *)arg;
    while (true)
    {
        sem_wait(&pSem);
        pthread_mutex_lock(&mutex);
        Node *newNode = new Node;
        newNode->number = rand() % 100;
        if (head != NULL)
            newNode->next = head;
        head = newNode;
        print(head);
        cout << "producer-id : " << *id << "  number = " << newNode->number << "  chian-szie = " << size(head) << endl;
        sem_post(&cSem);
        usleep(rand() % 3);
    }
    return NULL;
}
void *consumerF(void *arg)
{
    int *id = (int *)arg;
    while (true)
    {
        sem_wait(&cSem);
        Node *newNode = head;  // 读取head时，由于可能发生阻塞，故应该在阻塞解除之后读取临界区内容
        print(head);
        cout << "consumer-id : " << *id << "  number = " << newNode->number << "  chian-szie = " << size(head) << endl;
        Node *nextNode = head->next;
        delete head;
        head = nextNode;
        sem_post(&pSem);
        usleep(rand()%4);
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&pSem,0,1); // 生产者信号量资源初始化为1  此时是不需要进行线程同步的
    sem_init(&cSem,0,0); // 消费者信号量资源初始化为0
    pthread_t producer[5], consumer[5];
    int pTid[5] = {1, 2, 3, 4, 5};
    int cTid[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        pthread_create(producer + i, NULL, producerF, pTid + i);
        pthread_create(consumer + i, NULL, consumerF, cTid + i);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
    }
    sem_destroy(&pSem);
    sem_destroy(&cSem);
    return 0;
}