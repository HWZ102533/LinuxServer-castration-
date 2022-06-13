#include "Base.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "string.h"
#include <sys/wait.h>

using namespace std;

void GetFileInDirectory(const std::string &strDir,std::string &strOutFiles){
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(strDir.c_str())) != NULL){
        while ((dirp = readdir(dp)) != NULL){
            strOutFiles += dirp->d_name;
//            dirp->d_type : DT_DIR DT_FIFO BLK REG LNK SOCK
            strOutFiles += "\n";
        }
        closedir(dp);
    }
}

void TestFileInDirectory(){
    std::string strDir("/home/hwz/CLionProjects/untitled2/");
    std::string strOut;
    GetFileInDirectory(strDir,strOut);
    printf("%s\n", strOut.c_str());
}

void TestFileInDirectoryRecursion(){
    std::string strDir("/home/hwz/CLionProjects/untitled2/");
    std::string strOut;
    GetFileInDirectoryRecursion(strDir,strOut);
    printf("%s\n", strOut.c_str());
}


void GetFileInDirectoryRecursion(const std::string &strDir,std::string &strOutFiles){
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(strDir.c_str())) != NULL){
        while ((dirp = readdir(dp)) != NULL){
            if(dirp->d_type==DT_DIR){
                if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
                    continue;
                string strTemp = strDir + dirp->d_name + "/";
                GetFileInDirectoryRecursion(strTemp,strOutFiles);
            }
//            dirp->d_type : DT_DIR DT_FIFO BLK REG LNK SOCK
                strOutFiles += dirp->d_name;
            strOutFiles += "\n";
        }
        closedir(dp);
    }
}

void TestFileType(){
    std::string strDir("/home/hwz/CLionProjects/untitled2/");
    std::string strOut;
    GetFileInDirectory(strDir,strOut);

    int iIndex = strOut.find('\n');
    while(iIndex!=-1){
        string strTemp = strDir+strOut.substr(0,iIndex);
        string strTemp2;
        GetFileType(strTemp,strTemp2);
        printf("%s\n", strTemp2.c_str());
        strOut.erase(0,iIndex+1);
        iIndex = strOut.find('\n');
    }
}

void GetFileType(const std::string &strDir,std::string &strOutType){
    struct stat buf;
    if (lstat(strDir.c_str(), &buf) < 0)
        return;

    if (S_ISREG(buf.st_mode))
        strOutType = "regular";
    else if (S_ISDIR(buf.st_mode))
        strOutType = "directory";
    else if (S_ISCHR(buf.st_mode))
        strOutType = "character special";
    else if (S_ISBLK(buf.st_mode))
        strOutType = "block special";
    else if (S_ISFIFO(buf.st_mode))
        strOutType = "fifo";
    else if (S_ISLNK(buf.st_mode))
        strOutType = "symbolic link";
    else if (S_ISSOCK(buf.st_mode))
        strOutType = "socket";
    else
        strOutType = "** unknown mode **";
}

void TestTempFile(){
    char name[L_tmpnam];
    //char line[MAXLINE];
    char line[256];
    FILE *fp;
    printf("%s\n", tmpnam(NULL)); /* first temp name */
    tmpnam(name); /* second temp name */
    printf("%s\n", name);
    if ((fp = tmpfile()) == NULL) /* create temp file */
        return;
    fputs("one line of output\n", fp); /* write to temp file */
    rewind(fp); /* then read it back */
    if (fgets(line, sizeof(line), fp) == NULL)
        return;
    fputs(line, stdout); /* print the line we wrote */
}

void TempFile(){
//    if (open("tempfile", O_RDWR) < 0)
//        return;
//    if (unlink("tempfile") < 0)//文件删除了 但是文件空间未释放
//        return;
//    printf("file unlinked\n");
//    sleep(15);
//    printf("done\n");

}

static int globvar = 6;
void TestFork(){
    int var; /* automatic variable on the stack */
    pid_t pid;
    var = 88;
    printf("before vfork\n"); /* we don’t flush stdio */
    if ((pid = vfork()) < 0) {
       return;
    } else if (pid == 0) { /* child */
        globvar++; /* modify parent’s variables */
        var++;
        _exit(0); /* child terminates */
    }
/* parent continues here */
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,
           var);
}

void TestSetPPID_Init(){
    pid_t pid;
    if ((pid = fork()) < 0) {
        return;
    } else if (pid == 0) { /* first child */
        if ((pid = fork()) < 0)
            return;
        else if (pid > 0)
            exit(0); /* parent from second fork == first child */
/*
* We’re the second child; our parent becomes init as soon
* as our real parent calls exit() in the statement above.
* Here’s where we’d continue executing, knowing that when
* we’re done, init will reap our status.
*/
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }
    if (waitpid(pid, NULL, 0) != pid) /* wait for first child */
        return;
/*
* We’re the parent (the original process); we continue executing,
* knowing that we’re not the parent of the second child.
*/
    return;
}

static void
sig_hup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}
static void
pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
           name, (long)getpid(), (long)getppid(), (long)getpgrp(),
           (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

void TestOrphanChild(){
    char c;
    pid_t pid;
    pr_ids("parent");
    if ((pid = fork()) < 0) {
        return;
    } else if (pid > 0) { /* parent */
        sleep(5); /* sleep to let child stop itself */
    } else { /* child */
        pr_ids("child");
        signal(SIGHUP, sig_hup); /* establish signal handler */
        kill(getpid(), SIGTSTP); /* stop ourself */
        pr_ids("child"); /* prints only if we’re continued */
        if (read(STDIN_FILENO, &c, 1) != 1)
            printf("read error %d on controlling TTY\n", errno);
    }
    exit(0);
}
//
#define LOCK_FILE     "./dfc.lock" //file do not in /tmp directory
bool isRunningNow()
{
    int fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0)
        return false;
    //测试互斥锁定区域
    return (lockf(fd, F_TLOCK, 0) == 0);
}

void daemon_exit(int iFlage){
    closelog();
    exit(0);
}

int daemonize(){
    pid_t pid = fork();
    if(pid==0){
        if(!isRunningNow()){
            //lockf(fd, F_UNLCK, 0);
            printf("alread file lock.\n");
            exit(1);
        }
    }else if(pid>0){
        exit(0);
    }else{
        syslog(LOG_ERR,"Fork():%s", strerror(errno));
        //perror("Fork()");
        exit(1);
    }
    int fd = open("/dev/null",O_RDWR);
    if(fd<0){
        //perror("open()");
        syslog(LOG_WARNING,"open():%s", strerror(errno));
        exit(-2);
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);

    if(fd>2)
        close(fd);
    setsid();
    chdir("/");
    umask(0);
    return 0;
}