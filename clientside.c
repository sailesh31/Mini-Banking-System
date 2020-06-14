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

int Welcome(int socketfd);
void Select_Action(int socketfd, int type);
void Admin_Action(int socketfd);
void Single_User_Login(int socketfd);
void Joint_User_Login(int socketfd);
void Admin_User_Login(int socketfd);
void Deposit(int socketfd, int type);
void Withdrawl(int socketfd, int type);
void Balance_Enquiry(int socketfd, int type);
void Password_Change(int socketfd, int type);
void View_Details(int socketfd, int type);
void Exit();
void Add_Account(int socketfd);
void Delete_Account(int socketfd);
void Modify_Account(int socketfd);
void Search(int socketfd);

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

int checkbit = 0;

int Welcome(int socketfd)
{
write(1,"Welcome to the online banking system\n",sizeof("Welcome to the online banking system\n"));
write(1,"Press 'S' for singleaccount 'J' for jointaccount 'A' for adminaccount\n",sizeof("Press 'S' for singleaccount 'J' for jointaccount 'A' for adminaccount\n"));
char user_type;
int type;
read(0,&user_type,sizeof(user_type));

  if(user_type=='S')
  {
      type = 1;
      write(socketfd,&type,sizeof(type));
      Single_User_Login(socketfd);
  }

  else if(user_type=='J')
  {
     type = 2;
     write(socketfd,&type,sizeof(type));
     Joint_User_Login(socketfd);
  }

  else if(user_type=='A')
  {
    type = 3;
    write(socketfd,&type,sizeof(type));
    Admin_User_Login(socketfd);
  }

  else
  {
    write(1,"Invalid Account Type\n",sizeof("Invalid Account Type\n"));
  }
return type;
}



void Select_Action(int socketfd, int type)
{
  int action;
  write(1,"How can we help you today!!!\n",sizeof("How can we help you today!!!\n"));
  write(1,"Press 1 for Deposit\n Press 2 for Withdrawl\n Press 3 for Balance Enquiry\n Press 4 for Password Change\n Press 5 for Account Details\n Press 6 to Exit\n ",sizeof("Press 1 for Deposit\n Press 2 for Withdrawl\n Press 3 for Balance Enquiry\n Press 4 for Password Change\n Press 5 for Account Details\n Press 6 to Exit\n "));
  scanf("%d",&action);

  switch(action)
  {
    case 1:
      write(socketfd,&action,sizeof(action));
      Deposit(socketfd,type);
      break;
    case 2:
      write(socketfd,&action,sizeof(action));
      Withdrawl(socketfd,type);
      break;
    case 3:
      write(socketfd,&action,sizeof(action));
      Balance_Enquiry(socketfd,type);
      break;
    case 4:
      write(socketfd,&action,sizeof(action));
      Password_Change(socketfd,type);
      break;
    case 5:
      write(socketfd,&action,sizeof(action));
      View_Details(socketfd,type);
      break;
    case 6:
      write(socketfd,&action,sizeof(action));
      Exit();
      break;
    default:
     write(1,"Invalid Service Exercised\n",sizeof("Invalid Service Exercised\n"));
     break;
  }

  return ;
}

void Admin_Action(int socketfd)
{
  int action;
  write(1,"Welcome Admin\n",sizeof("Welcome Admin\n"));
  write(1,"Press 1 to Add Account\n Press 2 to Delete Account\n Press 3 to Modify Account\n Press 4 to Search\n Press 5 to Change Password\n Press 6 to Exit\n",sizeof("Press 1 to Add Account\n Press 2 to Delete Account\n Press 3 to Modify Account\n Press 4 to Search\n Press 5 to Change Password\n Press 6 to Exit\n"));
  scanf("%d",&action);
  switch(action)
  {
    case 1:
      write(socketfd,&action,sizeof(action));
      Add_Account(socketfd);
      break;
    case 2:
      write(socketfd,&action,sizeof(action));
      Delete_Account(socketfd);
      break;
    case 3:
      write(socketfd,&action,sizeof(action));
      Modify_Account(socketfd);
      break;
    case 4:
      write(socketfd,&action,sizeof(action));
      Search(socketfd);
      break;
    case 5:
      write(socketfd,&action,sizeof(action));
      Password_Change(socketfd,3);
      break;
    case 6:
      write(socketfd,&action,sizeof(action));
      Exit();
      break;
    default:
     write(1,"Invalid Service Exercised\n",sizeof("Invalid Service Exercised\n"));
     break;

  }
  return;
}

void Single_User_Login(int socketfd)
{
  suser su;
  write(1,"Enter UserId : ",sizeof("Enter UserId : "));
  scanf("%d",&su.user_id);
  write(1,"Enter Username : ",sizeof("Enter Username : "));
  read(0,&su.user_name,sizeof(su.user_name));
  write(1,"Enter Password : ",sizeof("Enter Password : "));
  read(0,&su.password,sizeof(su.password));

  write(socketfd,&su.user_id,sizeof(su.user_id));
  write(socketfd,&su.user_name,sizeof(su.user_name));
  write(socketfd,&su.password,sizeof(su.password));
  read(socketfd,&checkbit,sizeof(checkbit));

  return;

}

void Joint_User_Login(int socketfd)
{
  juser ju;

  write(1,"Enter UserId : ",sizeof("Enter UserId : "));
  scanf("%d",&ju.user_id);
  write(1,"Enter Username : ",sizeof("Enter Username : "));
  read(0,&ju.account_name,sizeof(ju.account_name));
  write(1,"Enter Password : ",sizeof("Enter Password : "));
  read(0,&ju.password,sizeof(ju.password));

  write(socketfd,&ju.user_id,sizeof(ju.user_id));
  write(socketfd,&ju.account_name,sizeof(ju.account_name));
  write(socketfd,&ju.password,sizeof(ju.password));

  read(socketfd,&checkbit,sizeof(checkbit));

  return;

}

void Admin_User_Login(int socketfd)
{
  auser au;

  write(1,"Enter AdminId : ",sizeof("Enter AdminId : "));
  scanf("%d",&au.admin_id);
  write(1,"Enter Admin Username : ",sizeof("Enter Admin Username : "));
  read(0,&au.user_name,sizeof(au.user_name));
  write(1,"Enter Password : ",sizeof("Enter Password : "));
  read(0,&au.password,sizeof(au.password));

  printf("%d\n",au.admin_id);
  printf("%s\n",au.user_name);
  printf("%s\n",au.password);
  write(socketfd,&au.admin_id,sizeof(au.admin_id));
  write(socketfd,&au.user_name,sizeof(au.user_name));
  write(socketfd,&au.password,sizeof(au.password));
  read(socketfd,&checkbit,sizeof(checkbit));

  return;

}


void Deposit(int socketfd, int type)
{
   float amount;
   write(1,"Enter the Amount you want to deposit\n",sizeof("Enter the Amount you want to deposit\n"));
   scanf("%f",&amount);
   write(1,"Depositing Rupees ",sizeof("Depositing Rupees "));
   printf("%f",amount);
   write(1,"\n",sizeof("\n"));
   write(socketfd,&amount,sizeof(amount));
   int transfer = 1;
   write(socketfd,&transfer,sizeof(transfer));
   int check;
   read(socketfd,&check,sizeof(check));
   if(check)
   {
     write(1,"Deposit Successful\n",sizeof("Deposit Successful\n"));
   }
   else
   {
     write(1,"Deposit Unsuccessful. Try again later\n",sizeof("Deposit Unsuccessful. Try again later\n"));
   }

   Select_Action(socketfd, type);
   return;
}

void Withdrawl(int socketfd, int type)
{
  float amount;
  write(1,"Enter Amount to Withdraw\n",sizeof("Enter Amount to Withdraw\n"));
  scanf("%f",&amount);
  write(1,"Withdrawing Rupees ",sizeof("Withdrawing Rupees "));
  printf("%f",amount);
  write(1,"\n",sizeof("\n"));
  write(socketfd,&amount,sizeof(amount));
  int check;
  read(socketfd,&check,sizeof(check));
  if(check)
  {
    write(1,"Successfull Withdraw\n",sizeof("Successfull Withdraw\n") );
  }
  else
  {
    write(1,"Insufficient Balance\n",sizeof("Insufficient Balance\n"));
  }
  Select_Action(socketfd,type);
  return;
}

void Balance_Enquiry(int socketfd, int type)
{
  float amount;
  read(socketfd,&amount,sizeof(amount));
  write(1,"Current Balance in this account is: ",sizeof("Current Balance in this account is: "));
  printf("%f",amount);
  write(1,"\n",sizeof("\n"));
  Select_Action(socketfd,type);
  return;
}

void Password_Change(int socketfd, int type)
{
  char existing_password[20];
  char new_password[20];
  write(1,"Enter Old Password : ",sizeof("Enter Old Password : "));
  read(0,&existing_password,sizeof(existing_password));

  write(socketfd,&existing_password,sizeof(existing_password));
  int check;
  read(socketfd,&check,check);

  if(check)
  {
    write(1,"Enter New Password : ",sizeof("Enter New Password : "));
    read(0,&new_password,sizeof(new_password));
    write(socketfd,&new_password,sizeof(new_password));
    write(1,"Password Change Successful\n",sizeof("Password Change Successful\n"));
  }

  else
  {
    write(1,"Wrong Password. Try Again\n",sizeof("Wrong Password. Try Again\n"));
  }

  if(type ==1 || type ==2)
  {
    Select_Action(socketfd,type);
  }
  else
  {
    Admin_Action(socketfd);
  }
  return ;
}

void View_Details(int socketfd, int type)
{
  if(type==1)
  {
    suser su;
    read(socketfd,&su,sizeof(su));
    printf("Username = %s\n",su.user_name);
    printf("Account Number =%d\n",su.account_number);
    printf("Balance = %f\n",su.balance);
  }
  else if(type==2)
  {
    juser ju;
    read(socketfd,&ju,sizeof(ju));
    printf("Username1 = %s\n",ju.user_name_a);
    printf("Username2 = %s\n",ju.user_name_b);
    printf("Account Number =%d\n",ju.account_number);
    printf("Balance = %f\n",ju.balance);
  }
  Select_Action(socketfd,type);
}

void Exit()
{
  exit(0);
}

void Add_Account(int socketfd)
{
  int check;
  char user_type;
  int type;

  write(1,"Press 'S' for singleaccount 'J' for jointaccount\n",sizeof("Press 'N' for singleaccount 'J' for jointaccount\n"));
  read(0,&user_type,sizeof(user_type));

  if(user_type=='S')
  {
     suser su;
     type = 1;
     write(1,"Enter Username\n",sizeof("Enter Username\n"));
     scanf(" %[^\n]",su.user_name);
     write(1,"Enter Password\n",sizeof("Enter Password\n"));
     scanf(" %[^\n]",su.password);
     write(socketfd,&type,sizeof(type));
     write(socketfd,&su,sizeof(suser));
  }
  else if(user_type=='J')
  {
    juser ju;
    type = 2;
    write(1,"Enter Username 1\n",sizeof("Enter Username 1\n"));
    scanf(" %[^\n]",ju.user_name_a);
    write(1,"Enter Username 2\n",sizeof("Enter Username 2\n"));
    scanf(" %[^\n]",ju.user_name_b);
    write(1,"Enter Password\n",sizeof("Enter Password\n"));
    scanf(" %[^\n]",ju.password);
    write(socketfd,&type,sizeof(type));
    write(socketfd,&ju,sizeof(juser));
  }
  read(socketfd,&check,sizeof(check));
  if(checkbit)
    {printf("Account Created Successfully\n");}
  Admin_Action(socketfd);

}

void Delete_Account(int socketfd)
{
  int check;
  char user_type;
  int type;

  write(1,"Press 'S' for singleaccount 'J' for jointaccount\n",sizeof("Press 'N' for singleaccount 'J' for jointaccount\n"));
  read(0,&user_type,sizeof(user_type));
  int userid;
  if(user_type =='S')
  {
    type = 1;
    write(socketfd,&type,sizeof(type));
    write(1,"Enter User Id\n",sizeof("Enter User Id\n"));
    scanf("%d",&userid);
    write(socketfd,&userid,sizeof(userid));
  }

  else if(user_type=='J')
  {
    type = 2;
    write(socketfd,&type,sizeof(type));
    write(1,"Enter User Id\n",sizeof("Enter User Id\n"));
    scanf("%d",&userid);
    write(socketfd,&userid,sizeof(userid));
  }

  if(check)
  {
    write(1,"Successfully Deleted The Account\n",sizeof("Successfully Deleted The Account\n"));
  }
  else
  {
    write(1,"Error Deleting The Account\n",sizeof("Error Deleting The Account\n"));
  }
  Admin_Action(socketfd);


}

void Modify_Account(int socketfd)
{
  int type;
  int check;
  char user_type;

  write(1,"Press 'S' for singleaccount 'J' for jointaccount\n",sizeof("Press 'N' for singleaccount 'J' for jointaccount\n"));
  read(0,&user_type,sizeof(user_type));

  if(user_type=='S')
  {
    suser su;
    printf("Enter the User ID\n");
    scanf("%d",&su.user_id);
    printf("Enter Modifications Required\n");
		printf("Enter the username\n");
		scanf(" %[^\n]",su.user_name);
		printf("Enter the password\n");
		scanf(" %[^\n]",su.password);
		printf("Enter the balance\n");
		scanf("%f",&su.balance);
		write(socketfd,&su,sizeof(suser));

  }

  else if(user_type=='J')
  {
    juser ju;
    printf("Enter the User ID\n");
    scanf("%d",&ju.user_id);
    printf("Enter Modifications Required\n");
    printf("Enter the username1\n");
    scanf(" %[^\n]",ju.user_name_a);
    printf("Enter the other username2\n");
    scanf(" %[^\n]",ju.user_name_b);
    printf("Enter the password\n");
    scanf(" %[^\n]",ju.password);
    printf("Enter the balance\n");
    scanf("%f",&ju.balance);
    write(socketfd,&ju,sizeof(juser));
  }

  read(socketfd,&check,sizeof(check));
	if(check)
		{printf("Successfully Modified the user\n");}
  else
    {printf("Unable to make modifications to the Account\n");}
	Admin_Action(socketfd);

}

void Search(int socketfd)
{
  int type;
  int check;
  char user_type;
  int userid;
  write(1,"Press 'S' for singleaccount 'J' for jointaccount\n",sizeof("Press 'N' for singleaccount 'J' for jointaccount\n"));
  read(0,&user_type,sizeof(user_type));

  if(user_type=='S'){
    write(socketfd,&type,sizeof(type));
    suser su;
    printf("Enter the User ID\n");
    scanf("%d",&userid);
    write(socketfd,&userid,sizeof(userid));
    read(socketfd,&su,sizeof(su));
    printf("username = %s\n",su.user_name);
    printf("account number =%d\n",su.account_number);
    printf("Balance = %f\n",su.balance);
    }
  else if(user_type=='J'){
    write(socketfd,&type,sizeof(type));
    juser ju;
    printf("Enter the User ID\n");
    scanf("%d",&userid);
    write(socketfd,&userid,sizeof(userid));
    read(socketfd,&ju,sizeof(ju));
    printf("Username 1  = %s\n",ju.user_name_a);
    printf("Username 2 = %s\n",ju.user_name_b);
    printf("Account Number =%d\n",ju.account_number);
    printf("Balance = %f\n",ju.balance);

    }


  Admin_Action(socketfd);
}



int main()
{
  int socketfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in server1;

  server1.sin_family = AF_INET;
	server1.sin_addr.s_addr=inet_addr("127.0.0.1");
	server1.sin_port = htons(5555);

  connect(socketfd,(struct sockaddr *)&server1,sizeof(server1));

  int type = Welcome(socketfd);
  if(type==1 || type==2){
    if(checkbit)
       {Select_Action(socketfd,type);}
    else
       {write(1,"Incorrect Credentials\n",sizeof("Incorrect Credentials\n"));}
  }
  else if(type==3){
    if(checkbit)
      {Admin_Action(socketfd);}
    else
      {write(1,"Incorrect Credentials\n",sizeof("Incorrect Credentials\n"));}
    }

}
