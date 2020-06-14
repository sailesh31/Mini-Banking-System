> INTRODUCTION
--------------
The project "Design and Development of online banking management system" has been implemented in 5 parts/files : singleusers.c; jointusers; adminusers.c; clientside.c and serverside.c

The functionalities provided to a client i.e. a single or joint account holder are: Deposit, Withdraw, Balance Enquiry, Password Change, View details and Exit

The functionalities provided to an admin (admin account holder) are : Add Account, Delete Account, Modify Account, Search for a specific account, Password Change and Exit.

> WORKING OF CODE
-----------------
singleuser, jointuser and admin user have been implemented as datastructures as follows:

singleuser
{
  int user_id;
  char user_name[20];
  char password[20];
  int account_number;
  float balance;
  bool status;
}

jointuser
{
  int user_id;
  char account_name[20];
  char user_name_a[20];
  char user_name_b[20];
  char password[20];
  int account_number;
  float balance;
  bool status;
}

adminuser
{
  int admin_id;
  char user_name[20];
  char password[20];
}

1) The files singleusers.c; jointusers; adminusers.c are used to create records/accounts of respective user type and store them in a file.

When one of the above programs is executed, instructions are prompted on the terminal showing various fields related to account details required to create a record/profile. On filling appropriate data the record is created in a file.


2) The file serverside.c has the code and functions required by the server for processing requests from client.

The server has been implemented as a concurrent server using fork() with an ability to listen to upto or queue upto 5 requests at a time.The connection/communication between the client and server is done through socket communication using a socket of the domain AF_INET of the type SOCK_STREAM at both the client and server.

Upon getting a request and details from client for a particular request, the server first checks if all the details/args are correct and enough to process the request. If yes, then that request is processed appropriately with filelockinng if a service like Deposit or Withdraw or anyother requires modifications to be done to account details
i.e. modifications in file to protect the critical data section..


3) The file clientside.c has the code and functions required by the client for login and access.

When clientside.c program is compiled and executed, user is asked to select the type of account the user wants to login. On selecting the appropriate account, user is requested to enter the user/admin id, username(any one of the two in case of joint user account) and password.
These details are then sent to server where the credentials are checked by scanning through the records/accounts file. If correct login details were entered then user is presented with a choice of exercising any service available( listed above in the intro section)
One can exercise as many services as one requires one after the other by entering the appropriate args/values when prompted by the program until one selects EXIT()


4) The functions Deposit, Withdraw, Balance Enquiry, Password Change, View details, Add Account, Delete Account, Modify Account, Search for a specific account and Exit are used to do the work as intuitively suggested by the name.

These functions are implemented in both clientside and serverside to request and process the service. Additional functions Welcome, Single User Login, Joint User Login and Admin Account Login are written in clientside to facilitate login.
serverside also has an additional function named Check to check login credentials received from the user.



>COMPILING THE CODE
-------------------
To compile the C files use the command gcc filename. To execute use ./a.out command


>RUNNING THE PROJECT
--------------------
In order to test the code and see the working demo of the project the following steps are to be followed.
   1. Compile all the 5 files
   2. Create records or user accounts of required types using the programs singleusers or jointusers or adminusers
   3. Start the server by executing the program serverside.c
   4. Then for user access, parallely run the program clientside.c (More than one instance can be run parallely mimicing multi user system as we have a concurrent server)
   5. Once in clientside programs exectution, select the choice of user account selection and avail required services by following the instructions displayed on the screen.
