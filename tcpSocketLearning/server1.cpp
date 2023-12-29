#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

/*
 * 1. 创建套接字
 * 2. 绑定地址信息
 * 3. 进行监听--(告诉OS内核可以接收客户端发送的新请求了并且监听新连接的到来)
 * 4. TCP三次握手建立连接----OS会自动进行，不需要我们进行控制
 * 5. 接收新连接
 * 6. 接收数据、发送数据
 * 7. 关闭套接字
 */ 

int main()
{
    //1. 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        cout << "TCP socket failed" << endl;
        return 0;
    }

    //2.绑定地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5656);
    addr.sin_addr.s_addr = inet_addr("172.18.69.25");
    int ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret < 0)
    {
        cout << "bind failed" << endl;
        return 0;
    }

    //3.进行侦听(告诉OS内核可以与客户端建立连接了)
    //listen函数中blocklog参数，设定的是已完成三次握手并已建立连接的队列的容量
    ret = listen(sockfd,1);
    if(ret < 0)
    {
        cout << "listen failed" << endl;
        return 0;
    }

    //4.接收新连接
    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int new_sockfd = accept(sockfd,
    			(struct sockaddr *)&client_addr,&socklen);

    if(new_sockfd < 0)
    {
        cout << "accept failed" << endl;
        return 0;
    }

    printf("accept success: %s:%d, and new_sockfd is %d\n",
    inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),
    			new_sockfd);

    //5.接收和发送数据
    while(1)
    {
        //这里接收数据和发送数据是没有顺序而言的
        //因为已经经过三次握手，客户端和服务端的连接以及建立成功了
        
        char buf[1024] = {0};

        ssize_t recv_size = recv(new_sockfd,buf,sizeof(buf)-1,0);
        if(recv_size < 0)
        {
            cout << "recv failed" << endl;
            continue;
        }
        else if(recv_size == 0)
        {
            cout << "Peer close" << endl;
            close(new_sockfd);
            close(sockfd);
            return 0;
        }
        
        cout << "accrpt data is : " << buf << endl;

        //发送数据
        memset(buf,'\0',sizeof(buf));
        sprintf(buf,"hello lient,i am server,i am %s:%d\n",
        		inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
        		
        ssize_t send_ret = send(new_sockfd,buf,strlen(buf),0); 
        if(send_ret < 0)
        {
            cout << "send failed" << endl;
            continue;
        }

    }

    close(sockfd);
    return 0;
}
