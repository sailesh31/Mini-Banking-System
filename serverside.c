#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<stdbool.h>
#include<string.h>


typedef struct singleuser{
int user_id;
char user_name[20];
char password[20];
int account_number;
float balance;
bool status;
}suser;

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

typedef struct adminuser{
  int admin_id;
  char user_name[20];
  char password[20];
}auser;


int Check(int actcon,int id,char user_name[20],char password[20],int type){
	int check=0;
	if(type==1){
		suser s;
  	int fd = open("singleusers",O_RDONLY);
		lseek(fd,((id))*(sizeof(suser)),SEEK_SET);
		read(fd,&s,sizeof(suser));

    strtok(user_name, "\n");
    strtok(password,"\n");

    if(s.status==true)
    {
  		if((strcmp(s.password,password)==0) && (strcmp(s.user_name,user_name)==0))
      {
  			printf("ValidUser\n");
  			check=1;
      }
      else
      {
        printf("InvalidUserThere\n");
      }
    }
		else
			printf("InvalidUserHere\n");

	  close(fd);
	}

	else if(type==2)
  {
		juser j;
		int fd = open("jointusers",O_RDONLY);
		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j,sizeof(juser));
    strtok(user_name, "\n");
    strtok(password,"\n");
    // printf("%s",user_name);
    if(j.status==true)
    {
  		if((strcmp(j.user_name_a,user_name)==0) || (strcmp(j.user_name_b,user_name)==0))
      {
        if((strcmp(j.password,password)==0))
        {
          printf("ValidUser\n");
          check=1;
        }
        else {printf("Invaliduser\n");}

      }
      else{printf("Invaliduser\n");}
    }
		else
			printf("Invaliduser\n");

	  close(fd);
	}

	else if(type==3){
		auser a;
		int fd = open("adminusers",O_RDONLY);
		lseek(fd,((id))*(sizeof(auser)),SEEK_SET);
		read(fd,&a,sizeof(auser));
  
    strtok(user_name, "\n");
    strtok(password,"\n");


		if((strcmp(a.password,password)==0) && strcmp(a.user_name,user_name)==0)
    {
			printf("ValidUser\n");
			check=1;
    }
		else
			printf("Invaliduser\n");


	  close(fd);
	}
		write(actcon,&check,sizeof(check));
		return check;

}


void Deposit(int actcon,int id,int type){
	float amount;
  int check=0;
  int done;
	read(actcon,&amount,sizeof(amount));
	if(type==1){
		suser s;
		int fd = open("singleusers",O_RDWR);

		//locking
		struct flock lock;
		lock.l_type =F_WRLCK; // write flock
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(suser);
		lock.l_len=sizeof(suser);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		lseek(fd,(id)*(sizeof(suser)),SEEK_SET);
		read(fd,&s,sizeof(suser));
		s.balance=s.balance+amount;

		lseek(fd,-(sizeof(suser)),SEEK_CUR);
		read(actcon,&done,sizeof(done));
		write(fd,&s,sizeof(suser));

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//unlocking

		check=1;
		close(fd);

		write(actcon,&check,sizeof(check));
	}

	else if(type==2){
		juser j;
		int fd = open("jointusers",O_RDWR);

		struct flock lock;
		lock.l_type =F_WRLCK; // write flock
		lock.l_whence=SEEK_SET;
		lock.l_start=(id)*sizeof(juser);
		lock.l_len=sizeof(juser);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j,sizeof(juser));

		j.balance=j.balance+amount;

		lseek(fd,-(sizeof(juser)),SEEK_CUR);
		read(actcon,&done,sizeof(done));
		write(fd,&j,sizeof(juser));

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//unlocking

		check=1;
		close(fd);

		write(actcon,&check,sizeof(check));
	}

		//return check;
}

void Withdrawl(int actcon,int id,int type){
	float amount;
  int check=0;
	read(actcon,&amount,sizeof(amount));
	if(type==1){
		suser s;
		int fd = open("singleusers",O_RDWR);
		lseek(fd,(id)*(sizeof(suser)),SEEK_SET);
		read(fd,&s,sizeof(suser));

		if(s.balance>=amount){
			s.balance-=amount;

			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(suser);
			lock.l_len=sizeof(suser);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(suser)),SEEK_CUR);
			write(fd,&s,sizeof(suser));

			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);

			check=1;
		}

		write(actcon,&check,sizeof(check));
	close(fd);
	}
	else if(type==2){
		juser j;
		int fd = open("jointusers",O_RDWR);
		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j,sizeof(juser));


		if(j.balance>=amount){
			j.balance-=amount;

			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(id)*sizeof(juser);
			lock.l_len=sizeof(juser);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(juser)),SEEK_CUR);
			write(fd,&j,sizeof(juser));

			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);//unlocking
			check=1;
		}


		write(actcon,&check,sizeof(check));
	close(fd);
	}
		return ;
}

void Balance_Enquiry(int actcon,int id,int type){
	if(type==1)
  {
		suser s;
		int fd = open("singleusers",O_RDONLY);
		lseek(fd,(id)*(sizeof(suser)),SEEK_SET);
		read(fd,&s,sizeof(suser));

		write(actcon,&s.balance,sizeof(s.balance));
	  close(fd);
	}

	else if(type==2)
  {
		juser j;
		int fd = open("jointusers",O_RDONLY);
		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j,sizeof(juser));

		write(actcon,&j.balance,sizeof(j.balance));
	  close(fd);
	}

	}

void Password_Change(int actcon,int id,int type){
  	int check=0;
    char existing_password[20];
    char new_password[20];

  	if(type==1)
    {
  		suser s;
  		int fd = open("singleusers",O_RDWR);
  		lseek(fd,(id)*(sizeof(suser)),SEEK_SET);
  		read(fd,&s,sizeof(suser));
  		read(actcon,&existing_password,sizeof(existing_password));

  		if(strcmp(s.password,existing_password)==0){
  			check=1;
  			write(actcon,&check,sizeof(check));
  			read(actcon,&new_password,sizeof(new_password));
  			strcpy(s.password,new_password);


  			struct flock lock;
  			lock.l_type =F_WRLCK; // write flock
  			lock.l_whence=SEEK_SET;
  			lock.l_start=(id)*sizeof(suser);
  			lock.l_len=sizeof(suser);
  			lock.l_pid=getpid();
  			int i=fcntl(fd,F_SETLKW,&lock);
  			printf("Entered the critical section\n");

  			lseek(fd,-(sizeof(suser)),SEEK_CUR);
  			write(fd,&s,sizeof(suser));

  			lock.l_type=F_UNLCK;
  			fcntl(fd,F_SETLK,&lock);//unlocking


  			check=1;
      }
  	 else
     {
       write(actcon,&check,sizeof(check));
     }

  	close(fd);
  	}


  	else if(type==2){
  		juser j;
  		int fd = open("jonitusers",O_RDWR);
  		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
  		read(fd,&j,sizeof(juser));
  		read(actcon,&existing_password,sizeof(existing_password));

  		if(strcmp(j.password,existing_password)==0)
      {
  			check=1;
  			write(actcon,&check,sizeof(check));
  			read(actcon,&new_password,sizeof(new_password));
  			strcpy(j.password,new_password);


  			//locking
  			struct flock lock;
  			lock.l_type =F_WRLCK; // write flock
  			lock.l_whence=SEEK_SET;
  			lock.l_start=(id)*sizeof(juser);
  			lock.l_len=sizeof(juser);
  			lock.l_pid=getpid();
  			int i=fcntl(fd,F_SETLKW,&lock);
  			printf("Entered the critical section\n");

  			lseek(fd,-(sizeof(juser)),SEEK_CUR);
  			write(fd,&j,sizeof(juser));

  			lock.l_type=F_UNLCK;
  			fcntl(fd,F_SETLK,&lock);//unlocking


  		}
  		else{write(actcon,&check,sizeof(check));}

  	close(fd);
  	}
  	else if(type==3){
  		auser a;
  		int fd = open("adminusers",O_WRONLY);
  		lseek(fd,(id)*(sizeof(auser)),SEEK_SET);
  		read(fd,&a,sizeof(auser));

      read(actcon,&existing_password,sizeof(existing_password));

  		if(strcmp(a.password,existing_password)==0){
        check=1;
        write(actcon,&check,sizeof(check));
  			read(actcon,&new_password,sizeof(new_password));
  			strcpy(a.password,new_password);


  			//locking
  			struct flock lock;
  			lock.l_type =F_WRLCK; // write flock
  			lock.l_whence=SEEK_SET;
  			lock.l_start=(id)*sizeof(auser);
  			lock.l_len=sizeof(auser);
  			lock.l_pid=getpid();
  			fcntl(fd,F_SETLKW,&lock);
  			printf("Entered the critical section\n");

  			lseek(fd,-(sizeof(auser)),SEEK_CUR);
  			write(fd,&a,sizeof(auser));

  			lock.l_type=F_UNLCK;
  			fcntl(fd,F_SETLK,&lock);//unlocking
  			}
  		else{write(actcon,&check,sizeof(check));}

  	close(fd);
  	}

  		return;
  }

void View_Details(int actcon,int id,int type){
	int check=0;

	if(type==1){
		suser s;
		int fd = open("singleusers",O_RDONLY);
		lseek(fd,(id)*(sizeof(suser)),SEEK_SET);
		write(actcon,&s,sizeof(suser));
		check=1;
		close(fd);
	}
	else if(type==2){
		juser j;
		int fd = open("jointusers",O_RDONLY);
		lseek(fd,(id)*(sizeof(juser)),SEEK_SET);
		write(actcon,&j,sizeof(juser));
		check=1;
		close(fd);
	}
	// write(actcon,&check,sizeof(check));
	return ;
	}

void Exit(int actcon)
{
  		close(actcon);
  		exit(0);
}

void Add_Account(int actcon){
	int type;
  int check=0;
	read(actcon,&type,sizeof(type));

	if(type==1){
		suser s1,s2;
		read(actcon,&s2,sizeof(suser));
		int fd = open("singleusers",O_RDWR);
		lseek(fd,-(sizeof(suser)),SEEK_END);
		read(fd,&s1,sizeof(suser));
		s2.user_id=s1.user_id+1;
		s2.account_number=s1.account_number+1;

		//locking
		struct flock lock;
		lock.l_type =F_WRLCK; // write flock
		lock.l_whence=SEEK_END;
		lock.l_start=-sizeof(suser);
		lock.l_len=sizeof(suser);
		lock.l_pid=getpid();
		fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		write(fd,&s2,sizeof(suser));

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//unlocking

		check=1;
		close(fd);
	}

	else if(type==2){
		juser j1,j2;
    read(actcon,&j2,sizeof(juser));
		int fd = open("jointusers",O_RDWR);
		lseek(fd,-(sizeof(juser)),SEEK_END);
		read(fd,&j1,sizeof(juser));
		j2.user_id=j1.user_id+1;
		j2.account_number=j1.account_number+1;


		struct flock lock;
		lock.l_type =F_WRLCK; // write flock
		lock.l_whence=SEEK_END;
		lock.l_start=-sizeof(juser);
		lock.l_len=sizeof(juser);
		lock.l_pid=getpid();
		fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		write(fd,&j2,sizeof(juser));

		lock.l_type=F_UNLCK;
	  fcntl(fd,F_SETLK,&lock);//unlocking
		check=1;
		close(fd);
	}
	write(actcon,&check,sizeof(check));
	return ;
}

void Delete_Account(int actcon){
	int type;
  int check=0;
  int user_id;
	read(actcon,&type,sizeof(type));

	if(type==1){
		suser s;
		read(actcon,&user_id,sizeof(user_id));
		int fd = open("singleusers",O_RDWR);
		lseek(fd,(user_id)*(sizeof(suser)),SEEK_SET);
		read(fd,&s,sizeof(suser));
		if(s.status==true){
			s.status=false;

			//locking
			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(user_id)*sizeof(suser);
			lock.l_len=sizeof(suser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(suser)),SEEK_CUR);
			write(fd,&s,sizeof(suser));

			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);//unlocking

			check=1;
    }

		close(fd);
	}
	else if(type==2){
		juser j;
		read(actcon,&user_id,sizeof(user_id));
		int fd = open("jointusers",O_RDWR);
		lseek(fd,(user_id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j,sizeof(juser));
		if(j.status==true){
			j.status=false;

			//locking
			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(user_id)*sizeof(juser);
			lock.l_len=sizeof(juser);
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(juser)),SEEK_CUR);
			write(fd,&j,sizeof(juser));

			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);//unlocking


			check=1;}

		close(fd);
	}
	write(actcon,&check,sizeof(check));
	return;
}

void Modify_Account(int actcon){
	int type;
  int check=0;
	read(actcon,&type,sizeof(type));

	if(type==1){
		suser s1,s2;
		read(actcon,&s2,sizeof(suser));
		int fd = open("singleusers",O_RDWR);
		lseek(fd,(s2.user_id)*(sizeof(suser)),SEEK_SET);
		read(fd,&s1,sizeof(suser));
		strcpy(s1.user_name,s2.user_name);
		strcpy(s1.password,s2.password);
		s1.account_number=s2.account_number;
		s1.balance=s2.balance;

		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(s2.user_id)*sizeof(suser);
		lock.l_len=sizeof(suser);
		lock.l_pid=getpid();
		fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		lseek(fd,-(sizeof(suser)),SEEK_CUR);
		write(fd,&s1,sizeof(suser));

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//unlocking


		check=1;
		close(fd);
	}

	else if(type==2){
		juser j1,j2;
		int fd = open("jusers",O_RDWR);
		read(actcon,&j2,sizeof(j2));
		lseek(fd,(j2.user_id)*(sizeof(juser)),SEEK_SET);
		read(fd,&j1,sizeof(juser));
		strcpy(j1.user_name_a,j2.user_name_a);
		strcpy(j1.user_name_b,j2.user_name_b);
		strcpy(j1.password,j2.password);
		j1.account_number=j2.account_number;
		j1.balance=j2.balance;


		struct flock lock;
		lock.l_type =F_WRLCK; // write flock
		lock.l_whence=SEEK_SET;
		lock.l_start=(j2.user_id)*sizeof(juser);
		lock.l_len=sizeof(juser);
		lock.l_pid=getpid();
		fcntl(fd,F_SETLKW,&lock);
		printf("Entered the critical section\n");

		lseek(fd,-(sizeof(juser)),SEEK_CUR);
		write(fd,&j1,sizeof(juser));

		lock.l_type=F_UNLCK;
		int j=fcntl(fd,F_SETLK,&lock);//unlocking
		check=1;
		close(fd);
	}
	write(actcon,&check,sizeof(check));
	return ;
	}

void Search(int actcon){
	int type;
  int check=0;
  int user_id;
	read(actcon,&type,sizeof(type));
	if(type==1){
		suser s;
		read(actcon,&user_id,sizeof(user_id));
		int fd = open("singleusers",O_RDONLY);
		lseek(fd,(user_id)*(sizeof(suser)),SEEK_SET);
		write(actcon,&s,sizeof(suser));
		check=1;
		close(fd);
	}
	else if(type==2){
		juser j;
		read(actcon,&user_id,sizeof(user_id));
		int fd = open("jointusers",O_RDONLY);
		lseek(fd,(user_id)*(sizeof(juser)),SEEK_SET);
		write(actcon,&j,sizeof(juser));
		check=1;
		close(fd);
	}
	write(actcon,&check,sizeof(check));
	return ;
	}


int main()
{
int sd = socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in server1,client;

server1.sin_family = AF_INET;
server1.sin_addr.s_addr = INADDR_ANY;
server1.sin_port = htons(5555);
bind(sd,(struct sockaddr*)&server1,sizeof(server1));

listen(sd,5);
write(1,"Server up and running\n",sizeof("Server up and running\n"));



while(1)
{
    int client_len = sizeof(client);
    int actcon=accept(sd,(struct sockaddr *)&client,&(client_len));
    if(!fork()){
    int type;
    int id;
    char user_name[20];
    char password[20];
    int log;
    write(1,"Login Request Reached Server\n",sizeof("Login Request Reached Server\n"));
    read(actcon,&type,sizeof(type));

    while(1)
    {
         if(type==1)
         {
            suser su;
            read(actcon,&id,sizeof(su.user_id));
            read(actcon,&user_name,sizeof(su.user_name));
       		 	read(actcon,&password,sizeof(su.password));
       			log=Check(actcon,id,user_name,password,type);
       			break;
         }

         else if(type==2)
         {
           juser ju;
           read(actcon,&id,sizeof(ju.user_id));
           read(actcon,&user_name,sizeof(ju.account_name));
           read(actcon,&password,sizeof(ju.password));
           log=Check(actcon,id,user_name,password,type);
           break;
         }

         else if(type==3)
         {
           auser au;
           read(actcon,&id,sizeof(au.admin_id));
           read(actcon,&user_name,sizeof(au.user_name));
           read(actcon,&password,sizeof(au.password));
           log=Check(actcon,id,user_name,password,type);
           break;
         }
    }

    while(1)
    {
        if(type==1 || type==2)
        {
              int option;
              read(actcon,&option,sizeof(option));
              if(option==1)
                {Deposit(actcon,id,type);}
              else if(option==2)
                {Withdrawl(actcon,id,type);}
              else if(option==3)
                {Balance_Enquiry(actcon,id,type);}
              else if(option==4)
                {Password_Change(actcon,id,type);}
              else if(option==5)
                {View_Details(actcon,id,type);}
              else if(option==6)
                {Exit(actcon);}
       }
       else if(type==3)
       {
          int choice;
          read(actcon,&choice,sizeof(choice));
          if(choice==1)
            {Add_Account(actcon);}
          else if(choice==2)
            {Delete_Account(actcon);}
          else if(choice==3)
            {Modify_Account(actcon);}
          else if(choice==4)
            {Search(actcon);}
          else if(choice==5)
            {Password_Change(actcon,id,type);}
          else if(choice==6)
            {Exit(actcon);}
      }
    }
}

	else close(actcon);
}

}
