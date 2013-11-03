#include <stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>

int main(int argc,char*argv[]){
  MYSQL *conn_ptr;
  conn_ptr = mysql_init(NULL);
  if(!conn_ptr){
    fprintf(stderr,"mysql_init failed\n");
    return -1;
  }
  conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","a","test",0,NULL,0);
  if(conn_ptr){
    printf("Connect sucess\n");
    
  }else{
    fprintf(stderr,"Connect failed\n");
      if (mysql_errno(&conn_ptr)) {
          fprintf(stderr,"connection error %d：%s\n",mysql_errno(&conn_ptr),mysql_error(&conn_ptr));
      }
  }
  mysql_close(conn_ptr);
  return 0;
}
