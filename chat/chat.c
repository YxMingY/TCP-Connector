//标准io
#include <stdio.h>
#include <stdlib.h>
//线程
#include <pthread.h>
//read,write,close等文件操作在此定义
#include <unistd.h>
//一切皆socket
#include <sys/socket.h>
//错误errno
#include <errno.h>
//IPPROTO_TCP常量在此定义
#include <netinet/in.h>
//inet_addr()在此定义
#include <arpa/inet.h>
//gethostbyname()在此定义
#include <netdb.h>
//memcpy()在此定义
#include <string.h>
void *receive(void *sock)
{
  char output[1024];
  while(1){
    //如果读出东西就输出
    if(read(*(int*)sock,output,1024) > 0){
      puts(output);
      //清理上一次留下的字符串
      memset(output, 0, strlen(output)*sizeof(char));
    }
  }
}
int main(int argc,char *argv[])
{
  if(argc != 3) {
	  printf("use: %d [addr] [port]",argv[0]);
  }
  //套接字
  int sock;
  //连接服务器时用
  struct sockaddr_in addr_in;
  
  //创建套接字
  if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    return 1;
  }
  
  addr_in.sin_family = AF_INET; /* 主机字节序 */
  
  //设置目标服务器端口
  addr_in.sin_port = htons(atoi(argv[2])); /*short, 网络字节序 */
  
  /*
  以下两个函数用于二进制地址格式和点分十进制字符串格式之间相互转换，但是这两个函数仅仅支持IPv4。
       in_addr_t inet_addr(const char *cp);
       char *inet_ntoa(struct in_addr in);
功能相似的两个函数同时支持IPv4和IPv6
       const char *inet_ntop(int domain, const void *addr, char *str, socklen_t size);
       int inet_pton(int domain, const char *str, void *addr);
  */
  
  //设置目标服务器
  struct hostent *hp;
  //支持域名和ip的方法
  hp = gethostbyname(argv[1]);
  /* memcpy()详解:
     函数原型: void *memcpy(void *dest, const void *src, size_t n);
     功能: 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
     返回值:指向dest的指针
  */
  memcpy(&addr_in.sin_addr.s_addr, hp->h_addr_list[0], (size_t)hp->h_length);
  //addr_in.sin_addr.s_addr = inet_addr("139.199.131.89");
  
  //接收准备
  pthread_t receiver;
  
  //连接服务器
  if(connect(sock,(struct sockaddr*) &addr_in,sizeof(addr_in)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    return 1;
  }

  //开始接收
  //创建(启动)线程，最后一个参数为int转换为的void指针，传参，到线程中再将其转化为int类型指针
  pthread_create(&receiver,NULL,&receive,(void*)&sock);
  //输出流
  char input[1024];
  fgets(input,1024,stdin);
  //strcmp()即字符串比较
  while(strcmp(input,"close\n")){
    //写给socket连接
    write(sock,input,strlen(input));
    fgets(input,1024,stdin);
  }
  close(sock);
  //pthread_exit(receiver);
  //pthread_join(receiver,NULL);
  puts("连接结束，OK\n");
  return 0;
}
