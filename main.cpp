//#include <sched.h>
#include <wait.h>
#include "Base.h"
#include "CSocket.h"
#include "CProtocolHttp.h"
#include "CPoolThread.h"
#include "CCryptoAES.h"
#include "CCacheRedis.h"
#include "CSqlMariadb.h"
#include "CFileAio.h"

int main(int argc, char *argv[]) {
    openlog("netFTP",LOG_PID|LOG_PERROR,LOG_DAEMON);// /var/log/messages
    struct sigaction sa;
    sa.sa_handler = daemon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGTERM);
    sigaddset(&sa.sa_mask,SIGQUIT);
    sigaddset(&sa.sa_mask,SIGINT);

    sigaction(SIGTERM,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);
    //int m_iNum = std::thread::hardware_concurrency()-1;

    if(!CFileAio::GetInstance().Init()){
        printf("CFileAio:%s\n", strerror(errno));
        return -1;
    }

    int iPort = 1986;
    int iIoThread = 8;
    int iWorkThread = 8;
    if(argc==4){
        iPort = atoi(argv[1]);
        iIoThread = atoi(argv[2]);
        iWorkThread = atoi(argv[3]);
    }

     //firewall-cmd --list-ports
    //firewall-cmd --zone=public --add-port=端口号/tcp --permanent
    //systemctl restart firewalld.service
    //systemctl disable firewalld.service
    //systemctl stop firewalld.service  
    //firewall-cmd --state
    //firewall-cmd --reload
    //sysctl -w kernel.core_pattern=core.%p.%e
    //set follow-fork-mode child  和  set detach-on-fork off
    //netstat -lx netstat -ltp  netstat -anp | grep ':1986'
    //ps -aux | grep HWZ
    /* -a (all)显示所有选项，默认不显示LISTEN相关
    -t (tcp)仅显示tcp相关选项
    -u (udp)仅显示udp相关选项
    -n 拒绝显示别名，能显示数字的全部转化成数字。
    -l 仅列出有在 Listen (监听) 的服務状态

    -p 显示建立相关链接的程序名
    -r 显示路由信息，路由表
    -e 显示扩展信息，例如uid等
    -s 按各个协议进行统计
    -c 每隔一个固定时间，执行该netstat命令。*/

    std::string strCachePort("127.0.0.1");
    std::unique_ptr<CCacheRedis>  pCache(new CCacheRedis(strCachePort, 6379));// shared_ptr
    if(!pCache->Init()){
        closelog();
        return 0;
    }

    std::string strIP("127.0.0.1");
    std::string strUser("root");
    std::string strPassword("cjtks");
    std::string DatabaseName("DFC_CAD");
    CSqlMariadb mariadb(strIP,3306,4,strUser,strPassword,DatabaseName);
    mariadb.Init();

    CPoolThread threads(-1);
    threads.Init();

    //boost::pool<> CryptoPool(SOCKET_BUF,20);//less than threads
    std::unique_ptr<CCrypto>  pCrypto(new CCryptoAES());// shared_ptr
    std::unique_ptr<CProtocol>  pProtocol(new CProtocolHttp(pCrypto.get()));
    std::unique_ptr<CServiceManager>  pService(new CServiceManager(pCache.get(),&threads, pProtocol.get(),&mariadb));// shared_ptr

    CSocket listenSocket(iPort,pService.get());
    listenSocket.CreateTCP();

    boost::pool<> m_poolSocket(sizeof(CSocket),20480);//50240

    CLoopHandle loopHandle(&m_poolSocket);
    //daemonize();
    loopHandle.loop(&listenSocket,threads);
    closelog();
}



