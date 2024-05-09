// #include <iostream>
// #include <unistd.h>
// #include <poll.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <map>
// #include <sys/epoll.h>
// #include <pthread.h>
// using namespace std;
// map<int, sockaddr_in> _map;
// epoll_event evs[1024];
// pthread_mutex_t mapMutex;
// struct cmb
// {
//     int curfd;
//     int cnt;
// };
// int epfd;
// void *comfun(void *arg);
// void *acceptfun(void *arg);
// int main()
// {
//     int lfd = socket(AF_INET, SOCK_STREAM, 0);
//     sockaddr_in saddr;
//     saddr.sin_family = AF_INET;
//     saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
//     saddr.sin_port = htons(10246);
//     int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
//     if (ret == -1)
//     {
//         cout << "绑定失败！" << endl;
//         return 1;
//     }
//     listen(lfd, 128);
//     epfd = epoll_create(1);
//     epoll_event ev;
//     ev.events = EPOLLIN;
//     ev.data.fd = lfd;
//     ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
//     int size = sizeof(evs) / sizeof(epoll_event);
//     pthread_mutex_init(&mapMutex, NULL);
//     while (true)
//     {
//         int num = epoll_wait(epfd, evs, size, -1);
//         for (int i = 0; i < num; ++i)
//         {
//             // 取出当前的文件描述符
//             int curfd = evs[i].data.fd;
//             // 判断该文件描述符是否用于监听
//             if (curfd == lfd)
//             {
//                 // 生成连接线程
//                 pthread_t tid;
//                 pthread_create(&tid, NULL, acceptfun, &curfd);
//                 pthread_detach(tid);
//             }
//             else
//             {
//                 // 生成通信线程
//                 pthread_t tid;
//                 cmb info;
//                 info.cnt = i;
//                 info.curfd = curfd;
//                 pthread_create(&tid, NULL, comfun, &info);
//                 pthread_detach(tid);
//             }
//         }
//     }
// }
// void *acceptfun(void *arg)
// {
//     // 建立新链接
//     cout << "建立连接" << endl;
//     int *curfd = (int *)arg;
//     sockaddr_in caddr;
//     socklen_t clen = sizeof(caddr);
//     int cfd = accept(*curfd, (sockaddr *)&caddr, &clen);
//     pthread_mutex_lock(&mapMutex);
//     _map[cfd] = caddr;
//     pthread_mutex_unlock(&mapMutex);
//     // 并且将新的通信文件描述符，插入到红黑树
//     epoll_event ev;
//     ev.data.fd = cfd;
//     ev.events = EPOLLIN;
//     epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
//     return NULL;
// }
// void *comfun(void *arg)
// {
//     // 处理通信过程
//     cmb *info = (cmb *)arg;
//     char buff[1024];
//     memset(buff, 0, sizeof(buff));
//     int len = recv(info->curfd, buff, sizeof(buff), 0);
//     if (len == -1)
//     {
//         // 接收失败
//         perror("recive");
//         return NULL;
//     }
//     else if (len == 0)
//     {
//         // 客户端断开连接
       
//         epoll_ctl(epfd, EPOLL_CTL_DEL, info->curfd, evs + info->cnt);
//         close(info->curfd);
//         pthread_mutex_lock(&mapMutex);
//         cout << "IP: " << inet_ntoa(_map[info->curfd].sin_addr) << "  断开连接...." << endl;
//         _map.erase(info->curfd);
//         pthread_mutex_unlock(&mapMutex);
//     }
//     else
//     {
//         pthread_mutex_lock(&mapMutex);
//         cout << "From IP: " << inet_ntoa(_map[info->curfd].sin_addr) << "\tPort: " << ntohs(_map[info->curfd].sin_port)
//              << "\trecive: " << buff << endl;
//         pthread_mutex_unlock(&mapMutex);
//         send(info->curfd, buff, strlen(buff) + 1, 0);
//     }
//     return NULL;
// }