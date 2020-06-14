#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdbool.h>

typedef struct singleuser{

	int user_id;
	char user_name[20];
	char password[20];
	int account_number;
	float balance;
	bool status;
}suser;

int main(void){
int fd;
int choice;
fd=open("singleusers",O_CREAT|O_RDWR,0744);
suser su;
while(1){
	printf("Please enter the user ID :");
	scanf("%d",&su.user_id);
	printf("Please enter the username:");
	scanf(" %[^\n]",su.user_name);
	printf("Please enter the password :");
	scanf(" %[^\n]",su.password);
	printf("Please enter the account number:");
	scanf("%d",&su.account_number);
	printf("Please enter the balance :");
	scanf("%f",&su.balance);
	su.status=true;
	write(fd,&su,sizeof(suser));
	break;
}
close(fd);
}
