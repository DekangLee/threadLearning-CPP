#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
using namespace std;
pthread_cond_t cond;
pthread_mutex_t mutex;
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
        Node *newNode = new Node;
        newNode->number = rand() % 100;
        pthread_mutex_lock(&mutex);
        while(size(head) >= MAXSIZE)  // while里的条件即是条件量满足条件时会主动放弃mutex
        //这里不能使用if 原因为同种类型的线程拿到mutex时直接运行，不再判断条件
            pthread_cond_wait(&cond, &mutex); // 可以将条件变量理解为，为完成一个任务在线程之间不断要通信的临界区状态
                                              // 方便线程做出正确动作
        // 线程被其他线程pthread_cond_broadcast唤醒时,会抢互斥锁
        if (head != NULL)
            newNode->next = head;
        head = newNode;
        print(head);
        cout << "producer-id : " << *id << "  number = " << newNode->number << "  chian-szie = " << size(head) << endl;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 3);
    }
    return NULL;
}
void *consumerF(void *arg)
{
    int *id = (int *)arg;
    while (true)
    {
        // 取出头结点的内容
        pthread_mutex_lock(&mutex);
        while (head == NULL)
            pthread_cond_wait(&cond, &mutex); // 注意cond条件的位置
        Node *newNode = head;                 // 读取head时，由于可能发生阻塞，故应该在阻塞解除之后读取临界区内容
        print(head);
        cout << "consumer-id : " << *id << " number = " << newNode->number << "  chian-szie = " << size(head) << endl;
        Node *nextNode = head->next;
        delete head;
        head = nextNode;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}