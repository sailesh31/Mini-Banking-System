#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdbool.h>

typedef struct jointuser{
  int user_id;
  char account_name[20];
  char user_name_a[20];
  char user_name_b[20];
  char password[20];
  int account_number;
  float balance;
  bool status;
}juser;



int main(void){
  int fd;
  int choice;
  fd=open("jointusers",O_CREAT|O_RDWR,0744);
  juser ju;
  while(1){
  	printf("Please enter the user_id :");
  	scanf("%d",&ju.user_id);
  	printf("Please enter the username 1:");
  	scanf(" %[^\n]",ju.user_name_a);
  	printf("Please enter other username 2:");
  	scanf(" %[^\n]",ju.user_name_b);
  	printf("Please enter the password :");
  	scanf(" %[^\n]",ju.password);
  	printf("Please enter the account number:");
  	scanf("%d",&ju.account_number);
  	printf("Please enter the balance :");
  	scanf("%f",&ju.balance);
  	ju.status=true;
  	write(fd,&ju,sizeof(juser));
    break;
  }
  close(fd);
}
