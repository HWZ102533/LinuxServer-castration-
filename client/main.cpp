#include <iostream>
#include "CSocket.h"
//#include <boost/pool/pool.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <thread>
#include "signal.h"
#include "CTimeWheel.h"

#include <sys/time.h>

static CSocket socket1("127.0.0.1","1986");

void alive(){
    boost::json::object request;
    request["ServiceType"] = "alive";
    request["name"] = "user20220416";
    socket1.RequestServer(boost::json::serialize(request),[](char* pStr){
        boost::json::object decode_val = boost::json::parse(pStr).as_object();
        //if(decode_val["Login"].as_int64()==0)
        printf( "alive - %d\n",decode_val["Login"].as_int64());
        if(decode_val["Login"].as_int64()==-3){//other client login
            socket1.Stop();
            printf( "other login - %d\n",decode_val["Login"].as_int64());
        }

    });//

}

void timer(int sig){
    if(SIGALRM == sig){
        //printf("timer\n");
        if(!socket1.IsStop()){
            alive();
            alarm(1);       //重新继续定时1s
        }
    }
}

int main() {
    //SIGALRM SIGPROF
//    CTimeWheel::Instance().Init(SIGALRM,5,[]{
//        printf("thread id:%d\n",std::this_thread::get_id());//std::this_thread::get_id(); gettid()
//        ;});

//    CTimeWheel::Instance().Init(SIGPROF,500,[]{
//        printf("thread id:%d\n",std::this_thread::get_id());//std::this_thread::get_id(); gettid()
//        ;});
//   for(int i=1;i<1000;++i){
//       int j = i*2;
//       CTimeWheel::Instance().AddTask(j,[j]{
//           printf("Index:%d\n",j);//std::this_thread::get_id(); gettid()
//       });
//   }
//
//    while(true){
//        sleep(100);
//    }


    if(!socket1.Connect())
    return -1;
    boost::json::object request;
//    request["ServiceType"] = "register";
//    request["name"] = "黄文钊";
//    request["password"] = "9123456";
//    request["PhoneNo"] = "18218021008";
//    request["Email"] = "20220416@qq.com";
//    request["note"] = "xxxx";
//    printf("%s-%d\n",boost::json::serialize(request).c_str(),boost::json::serialize(request).length());
//    socket1.RequestServer(boost::json::serialize(request),[](char* pStr){
//        boost::json::object decode_val = boost::json::parse(pStr).as_object();
//        if(decode_val["Login"].as_int64()==-7)
//            printf( "%s\n",decode_val["Role"].as_string().c_str());
//        else
//            printf( "%s\n",decode_val["Role"].as_string().c_str());
//
//    });//
//    pause();
//    return 0;

    //pthread_join(socket1.m_tid_reader, nullptr);
    request.clear();
    std::string strAppName = "DFC_ST";
    request["ServiceType"] = "login";
    request["name"] = "黄文钊";
    request["password"] = "9123456";//newpassword u20220416
    request["App"] = "CAD_ST";
    CSocket *pSocket = &socket1;
    socket1.RequestServer(boost::json::serialize(request),[&strAppName](char* pStr){
        boost::json::object decode_val = boost::json::parse(pStr).as_object();
        if(decode_val["Login"].as_int64()!=1)
            printf( "%s\n",decode_val["Role"].as_string().c_str());
        else{
            printf( "login - %s-%s\n",decode_val["Role"].as_string().c_str(),decode_val["Version"].as_string().c_str());
            std::string  strVersion = decode_val["Version"].as_string().c_str();

            if(strVersion.compare("v1.0.01")>0){//update app
                decode_val.clear();
                decode_val["ServiceType"] = "download";
                decode_val["appName"] = strAppName;
                decode_val["version"] = "20220416";
                decode_val["size"] = "list";
                socket1.ReceiveFile(boost::json::serialize(decode_val),strAppName,[](char* pStr){
                    printf( "%s\n",pStr);
                });
            }
        }
    });//
//    request.clear();
//    request["ServiceType"] = "password";
//    request["name"] = "user20220416";
//    request["password"] = "newpassword";//newpassword
//    socket1.RequestServer(boost::json::serialize(request),[](char* pStr){
//        boost::json::object decode_val = boost::json::parse(pStr).as_object();
//        //if(decode_val["Login"].as_int64()==0)
//            printf( "password - %s\n",decode_val["Role"].as_string().c_str());
//    });//
    request.clear();
    alive();
    signal(SIGALRM, timer); //注册安装信号
    alarm(1);       //触发定时器

//    request.clear();
//    std::string strFilename("openssl-1.0.2k.tar.gz");// Debug2.zip openssl-1.0.2k.tar.gz
//    long iSize = 0;
//    struct stat statbuff;
//    if(-1!=stat(strFilename.c_str(), &statbuff))
//        iSize = statbuff.st_size;
//
//    request["ServiceType"] = "upload";
//    request["name"] = strFilename;//filename
//    request["total"] = std::to_string(iSize);//filer size
//    socket1.RequestServer(boost::json::serialize(request),[&socket1,strFilename,iSize](char* pStr){
//        boost::json::object decode_val = boost::json::parse(pStr).as_object();
//        if(decode_val["fileName"].as_string()=="OK"){
//            socket1.SendFile(strFilename,iSize,[](char* pStr){
//                printf( "upload OK!\n");
//            });
//        }//other client login
//    });//
while(true){
    sleep(100);
}
    pause();
    printf( "out-%d\n",std::this_thread::get_id());
    return 0;
}
