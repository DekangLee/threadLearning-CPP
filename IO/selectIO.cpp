#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <map>
#include <pthread.h>
using namespace std;
struct fdInfo
{
    int fd;
    int *maxFd;
    fd_set *readSet;
};
map<int, sockaddr_in> fdSOCK;
pthread_mutex_t fdMutex;
pthread_mutex_t mapMutex;
pthread_mutex_t rdMutex;
void *acceptCon(void *arg)
{
    fdInfo *info = (fdInfo *)arg;
    // 监听文件描述符为就绪状态，说明有新连接
    sockaddr_in caddr;
    socklen_t lenAddr = sizeof(caddr);
    int cfd = accept(info->fd, (sockaddr *)&caddr, &lenAddr);
    pthread_mutex_lock(&rdMutex);
    FD_SET(cfd, info->readSet);
    pthread_mutex_unlock(&rdMutex);

    pthread_mutex_lock(&mapMutex);
    fdSOCK.insert(make_pair(cfd, caddr));
    pthread_mutex_unlock(&mapMutex);

    pthread_mutex_lock(&fdMutex);
    *(info->maxFd) = *(info->maxFd) > cfd ? *(info->maxFd) : cfd;
    pthread_mutex_unlock(&fdMutex);

    delete info;
    return NULL;
}
void *comfun(void *arg)
{
    fdInfo *info = (fdInfo *)arg;

    char buff[1024];
    int len = recv(info->fd, buff, sizeof(buff), 0);
    if (len == 0)
    {
        // 客户端断开连接
        cout << "客户端 IP:" << inet_ntoa(fdSOCK[info->fd].sin_addr) << " port:" << fdSOCK[info->fd].sin_port << " 已断开连接....." << endl;
        FD_CLR(info->fd, info->readSet); // 将之前产生的文件描述符从readSet中移除
        close(info->fd);
    }
    else if (len == -1)
        perror("recive");
    else
    {
        // 连接成功功能区
        cout << "From IP:" << inet_ntoa(fdSOCK[info->fd].sin_addr) << " revive: " << buff << endl;
        for (int i = 0; i < len; ++i)
        {
            buff[i] = toupper(buff[i]);
        }
        int ret = send(info->fd, buff, strlen(buff) + 1, 0);
        if (ret == -1)
        {
            perror("send");
            exit(1);
        }
    }
    delete info;
    return NULL;
}
int main()
{
    // 1.创建用于监听的文文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10246);
    saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
    int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return 1;
    }
    listen(lfd, 128);
    fd_set readSet; // 创建用于读的文件描述符集合
    FD_ZERO(&readSet);
    FD_SET(lfd, &readSet);
    int *maxFD = new int(lfd);
    pthread_mutex_init(&fdMutex, NULL);
    pthread_mutex_init(&mapMutex, NULL);
    pthread_mutex_init(&rdMutex, NULL);
    while (true)
    {
        pthread_mutex_lock(&rdMutex);
        fd_set temp = readSet;
        pthread_mutex_unlock(&rdMutex);
        int ret = select(*maxFD + 1, &temp, NULL, NULL, NULL);
        // 判断是否为监听的文件描述符
        if (FD_ISSET(lfd, &temp))
        {
            // 创建accept子线程
            pthread_t tid;
            fdInfo *info = new fdInfo;
            info->fd = lfd;
            info->maxFd = maxFD;
            info->readSet = &readSet;
            pthread_create(&tid, NULL, acceptCon, info);
            pthread_detach(tid);
        }
        for (int i = 0; i <= *maxFD; ++i)
        {
            if (i != lfd && FD_ISSET(i, &temp))
            {
                // 该文件描述符并非监听文件描述符，而是通信文件描述符
                // 接收数据，创建通信子线程
                pthread_t tid;
                fdInfo *info = new fdInfo;
                info->fd = i;
                info->maxFd = maxFD;
                info->readSet = &readSet;
                pthread_create(&tid, NULL, comfun, info);
                pthread_detach(tid);
            }
        }
    }
    delete maxFD;
    close(lfd);
    pthread_mutex_destroy(&mapMutex);
    pthread_mutex_destroy(&fdMutex);
    return 0;
}
