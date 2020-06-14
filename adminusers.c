#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

typedef struct admin{

	int admin_id;
	char user_name[20];
	char password[20];
}auser;

int main(void){
int fd;
fd=open("adminusers",O_CREAT|O_RDWR,0744);
auser au;
while(1){
	printf("Please enter the admin ID :");
	scanf("%d",&au.admin_id);
	printf("Please enter the username :");
	scanf(" %[^\n]",au.user_name);
	printf("Please enter the password :");
	scanf(" %[^\n]",au.password);
	write(fd,&au,sizeof(auser));
  break;
}
close(fd);
}
