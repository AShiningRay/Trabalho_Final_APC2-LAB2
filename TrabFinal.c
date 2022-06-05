/**                                   **********Variables Table************
        bool decs : boolean variable used by the user to decide if he wants to look and show a product by its name or register(position)
        char product_name[25] : The name of the product (assigned by the user on the register_product() function)
        char searchname[25] : A string with the same size of product_name. Receives a name from the user's keyboard and checks if there is a product with that same name on the memory.
        char buffer[15] : Small char buffer used to load the data from the Product Number.txt file.
        float providerprice : The price of each of that product bought from the provider (assigned on the register_product() function)
        float sellingprice : The price of each of that product sold to the consumers (assigned on the register_product() function)
        float sell1 : The value deducted from the amount of products sold and their respective unitary selling prices to the consumer
        float buy1 : The value deducted from the amount of products bought and their respective unitary buying prices stipulated by the provider
        float profit : Represents the result of the calculations done on the report_productstats() function [profit = sell1 - buy1]
        unsigned int quantity : The quantity of the product that the establishment bought (assigned by the user on the register_product() function)
        unsigned int product_code : The product's code (assigned automatically on the register_product() function)
        unsigned int regcheck : Used to check all the positions of the products struct and their respective data.
        unsigned int ps : Indicates the starting position to search for a certain product, moving up until the last position
        unsigned int pos : Consists of showing the products names for all positions, and then is used to show a more in-depth info about the product in the position that the user specified.
        unsigned int buy : Keeps track of the number of products that have been bought from the provider
        unsigned int sell : Keeps track of the number of products that have been sold to the customers
        unsigned int cont : The product number counter, which is saved into its own file to be load on posterior runs in order to load the products correctly between program executions.
        unsigned int i : A standard variable regularly used on "for" loops.
        unsigned int quant : Holds the quantity of products the user wants to register
        unsigned short int dec : Variable used by the user to decide which function to call in the main menu.
*/

#include <stdio.h> // For pretty much everything related to input and output defined by C ANSI
#include <string.h> // Allows for string manipulating functions like strcmp(), which is heavily used in the code
#include <stdlib.h> // Allows the use of system("cls") to clear the screen output and the rand() function, used to assign a random code to the products
#include <unistd.h> // Allows the use of usleep() variable, to delay certain parts of the code
#include <stdbool.h> // Used by the boolean variables, like the "decs" variable
#include <windows.h> // Used to make the executable more stylish

typedef struct prod // Defines the prod struct as a new type
{ // Variables explained on the variables table.
char product_name[25];
unsigned int quantity, product_code;
float providerprice, sellingprice;
} proddata;
 proddata products; // Defines the products variable as a proddata struct type

void main_menu (struct prod *p); // Brings up the main menu, which is responsible for calling forward the functions below
int check_fopen(FILE *frp, FILE *fp, FILE *fs); // This function's sole purpose is check if all the files have been opened successfully.
void register_product (struct prod *p, unsigned int quant, unsigned int *cont,  unsigned int *buy, float *buy1, float *sell1, float *profit,  unsigned int *sell); // Registers a product with the information provided by the user.
void search_product (struct prod *p, unsigned int cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs); // Searches for a product based on a position or if it fits the description given by the user
void show_products (struct prod *p, unsigned int cont, char searchname[], bool decs); // Shows all the products currently in the storage, or a product specified by the user
void edit_products (struct prod *p, unsigned int cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs); // Edits a specific part of the product specified by the user
void sell_products (struct prod *p, unsigned int cont, unsigned int *sell, float *sell1); // Sells a product specified by the user
void inventory_control (struct prod *p, int cont); // Shows the products that are currently in shortage and need restocking
void report_productstats (struct prod *p, int cont, int buy, int sell, float *profit, float *sell1, float *buy1); // Reports the financial stats about the products(amount bought, sold, money spent, made and Net Profit)
void delete_product (struct prod *p, unsigned int *cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs, FILE *frp); // Deletes a product specified by the user
void trashbin (); // Deletes all the data currently used by the program, including its files
void SetColor(int ForgC); //Used for stylistic purposes, we should not need ti code that part of the library at all, but it seems that the windows.h header does not support that function at all.
void ClearConsoleToColors(int ForgC, int BackC); //Used for stylistic purposes, we should not need ti code that part of the library at all, but it seems that the windows.h header does not support that function at all.
void animation(unsigned int num); //Used for stylistic purposes(Yes, we wanted to do a very stylish code for the last exercise)

int main ()
{
    proddata *p; // Defines a proddata type pointer.
    p = &products; // Points it to the address of the products struct.
  main_menu (p); // Honestly, using the main function just to call the main menu is quite unnecessary.
  return 0;
}

void main_menu(struct prod *p)
{ // Every single one of these variables are explained on the variables table.
  unsigned int regcheck=0, ps=0, pos=0, buy=0, sell=0, cont,i=0,quant=0;
  float sell1, buy1, profit;
  unsigned short int dec;
  char searchname[25], buffer[15];
  bool decs=0;

      FILE *fp; // File pointer responsible for opening the Products.txt file and retrieving its data
      FILE *frp; // General purpose File pointer used to open various different files for reading, writing a appending.
      FILE *contador; // File pointer tasked with opening the Product Number.txt file and retrieving its data.
      fp = fopen("Produtos.txt","r");
    contador = fopen("Product Number.txt", "r");
    if (check_fopen(contador,fp, NULL) == -3) {fclose(fp); fclose (contador); usleep(999999); trashbin(); free(p);} // Checks one files above has been opened and the other has not, and erases all data on the files and memory, to prevent the code from saving buggy files afterwards.
    while(i<5)  //Starts the reading of the contents saved into Product Number.txt
    {
      fgets(buffer, 15, contador); // For each iteration, it gets a line from the file and puts it into the buffer variable
       if (i==0) cont = atoi(buffer); // This one converts the number of products from string to char
        if (i==1) buy = atoi(buffer); // This one converts the number of products bought from string to char
         if (i==2) sell = atoi(buffer); // This one converts the number of products sold from string to char
          if (i==3) buy1 = atoi(buffer); // This one converts the sum of money spent by the number of products bought from string to char
           if (i==4) sell1 = atoi(buffer); // This one converts the sum of money made by the number of products sold from string to char
            if (i==5) profit = atoi(buffer); // This one converts the Net profit from string to char
        i++;
    }
    profit = sell1 - buy1; // Calculates the Net Profit
fclose(contador); // Closes this pointer, since it will no be used anymore throughout the code
    p = malloc (cont * sizeof(products)); // Allocates the number of products taken from cont, which was retrieved from Product Number.txt
   for(i=0; i< cont; i++)
   {// Reads the products struct data that was previously saved into the file using fwrite()
     fread((p+i), sizeof(products),1,fp);
   }
   fclose (fp);
  while (dec != 10) // Keeps the program running while the exit option is not selected
  { // Displays all of the functions supported by the code.
      SetColor(9);
   printf("\n\t----------------------------------------------- \n");
   usleep(70000);
   printf("\t|                  MAIN MENU                  |\n");
   usleep(70000);
   printf("\t----------------------------------------------- \n\n");
   usleep(70000);
   SetColor(2);
   usleep(70000);
   printf("\t1 - Register a product.\n");
   usleep(70000);
   printf("\t2 - Search a product.\n");
   usleep(70000);
   printf("\t3 - Show products.\n");
   usleep(70000);
   printf("\t4 - Edit a product's data.\n");
   usleep(70000);
   printf("\t5 - Sell products.\n");
   usleep(70000);
   printf("\t6 - Inventory control.\n");
   usleep(70000);
   printf("\t7 - Report financial stats.\n");
   usleep(70000);
   printf("\t8 - Delete a product.\n");
   usleep(70000);
   printf("\t9 - Delete all the products.\n");
   usleep(70000);
   printf("\t10 - Exit.\n\n");
   usleep(70000);
   SetColor(14);
   usleep(70000);
   printf("\tInsert the function that you want to access : ");
   scanf ("%u", &dec);
     switch (dec) // Uses the switch to decide which function to call
      {
        case 1 : {
            printf("\n\nHow many products do you want to register?\n");
            scanf("%u", &quant);
            while (quant < 0) // Checks if the user has inserted a valid value for the quantity(since you cannot ) asking the user to digit the quantity again
            {
               printf("Invalid quantity, Digit again: ");
                scanf("%u", &quant);
                 setbuf(stdin,NULL); //Clears the buffer to prevent the values from staying in the buffered memory
            }
            quant += cont;
            p = realloc(p, quant*sizeof(products)); // Reallocates the number of positions the struct is allowed to have, to better accommodate the new products
              register_product (p, quant, &cont, &buy, &buy1,&sell1,&profit,&sell); // Calls the function that registers the products
                 }; break;
        case 2 : search_product (p, cont, regcheck, ps ,pos, searchname, decs); break; // Calls the function that searches for a specific product
        case 3 : show_products (p, cont, searchname, decs); break; // Calls the function that displays all the products, or a specific one
        case 4 : edit_products (p, cont, regcheck, ps, pos, searchname, decs); break; // Calls the function that edits a specific part of a product
        case 5 : sell_products (p,cont, &sell, &sell1); break; // Calls the function that sells a product specified by the user
        case 6 : inventory_control (p,cont); break; // Calls the function that displays the products that are in shortage and need restocking or replacement
        case 7 : report_productstats (p,cont,buy,sell, &profit, &sell1,&buy1); break; // Calls the function that displays the financial stats of the establishment
        case 8 : delete_product (p, &cont, regcheck, ps ,pos, searchname, decs, frp); break; // Calls the function that deletes a product specified by the user
        case 9 : fclose (fp); trashbin (); cont =0; sell =0; sell1 = 0; buy = 0; buy1 = 0; profit = 0; break; // Calls the function that deletes all the data used by the program, zeroing all the counters along the way.
      }
      if (dec < 1 || dec > 10) // Checks if the user has typed a option that is present in the menu, or if it is out of bounds
        {
            system ("cls"); // Clears the screen, and loads the menu again, cleaning garbage text that can worsen visibility by quite a bit.
            printf("\n\n\t Invalid Option, reloading main Menu...\n\n");
            setbuf(stdin,NULL); //Clears the buffer to prevent the user from causing a glitch in the code by inserting a char when the program is expecting a unsigned integer.
        }
  }
  if (dec == 10)
    {
      printf("\n\n\tClosing the program . . . \n\n"); // Tells the user that the program is going to close, and proceeds to save the counter and other data into the Product Number.txt file.
      usleep(999999); // Gives the user some time to read the text above.
    }
    contador = fopen("Product Number.txt", "w"); // Begins the process of saving the product's data into Product Number.txt
    fprintf(contador, "%d", cont); // saves the number of products
     fprintf(contador,"\n");
      fprintf(contador, "%d", buy); // saves the number of products bought from the provider
       fprintf(contador,"\n");
        fprintf(contador, "%d", sell); // saves the number of products sold to the consumers
         fprintf(contador,"\n");
          fprintf(contador, "%f", buy1); // saves the amount of money spent in the bought products
           fprintf(contador,"\n");
            fprintf(contador, "%f", sell1); // saves the amount of money made by the sold products
             fprintf(contador,"\n");
              fprintf(contador, "%f", profit); // saves the Net profit, calculated beforehand
               fclose(contador); // Closes the Product Number.txt file
}

int check_fopen(FILE *frp, FILE *fp, FILE *fs) /** Checks if every file opened by the program has been opened successfully.
 It was implemented as a separate function to make the file opening checking operations easier, stripping the burden of writing the same lines in every function that opens a file */
{
    SetColor(9);
    if (fp == NULL && frp == NULL && fs  == NULL) // Checks if none of the files have been opened successfully focusing on the last parameter, since it is the one passed by the other functions.
    {
        perror("\n\tThe file could not be opened or does not exist.\n"); // Prints that the file could not be opened and explains what error has been encountered.
        usleep(500000);
        printf("\n\n"); // Skips two lines to prevent the menu from being printed incorrectly
    }
     if (fp == NULL && frp == NULL && fs  == NULL) // Checks if none of the files have been opened successfully focusing on the first and second parameters, used on the program's boot.
        {
         printf("\n\tNo files have been found, so they will be created posteriorly.\n");
         animation(0);
         return -1;
        }
     else if (fp == NULL && fs == NULL) // Checks if the Product Number.txt is present but the products.txt is not.
        {
        printf("\n\tThe counter file has been initialized but the products file has not \n\nDeleting the counter file and starting the program.\n");
        return -3;
        }
     else if (frp == NULL && fs == NULL) // Checks if the Products.txt is present but the Product Number.txt is not.
        {
         printf("\n\tThe products file has been initialized but the counter file has not \n\nDeleting the products file and starting the program.\n");
         return -3;
        }
     else if (fp != NULL && frp != NULL && fs  == NULL)
      {
          printf("\n\tPrior execution files found! Loading them into memory.\n"); // Checks both Product Number.txt and products.txt are present.
          animation (1);
          return -2;
      }

    if (fs != NULL) printf("\n\tThe file has been opened successfully.\n\n"); // Checks if the pointer passed by the other functions has been opened successfully
    usleep (400000); // Delays the next command for a while to better read what this functions has printed
    return 0; // Returns 0 if no problems have occurred.
}

void register_product (struct prod *p, unsigned int quant, unsigned int *cont, unsigned int *buy, float *buy1, float *sell1, float *profit, unsigned int *sell) /** This function is used to register one or more products,
depending on the user's preference. That function works by asking the user to input one kind of data at a time and saving it to its respective variable inside the products struct,
using the "p" pointer to access its positions*/
{
  FILE *fps; // Defines a pointer to open the Produtos.txt file only for this function
  fps = fopen("Produtos.txt", "a+"); // Opens, or the file to append new products into it
    if (check_fopen(NULL,NULL, fps) == 0); // Checks if the file above could be opened or created successfully.
    else return;
  int i, j; // Used for the "for" loops below
  for (i=0; i < quant; i++) // Repeats the loop while the i variable is smaller than or equal to the quantity defined by the user on the main_menu() function
    {
    SetColor(10);
     printf ("Type the name of the product: \n"); // Asks the user what is the name of the product
      scanf("%s", &(p + (*cont))->product_name); // Receives the name of the product typed by the user
      for(j = 0; j < *cont; j++)
        {
         while(stricmp((p + (*cont))->product_name, (p + j)->product_name) == 0) // Checks if there is another product with the same name as the one typed above
         {
            printf("There seems to be another product with that same name.\n Type another name:");
            scanf("%s", &(p + (*cont))->product_name);
             setbuf(stdin,NULL); //When typing strings, it is better to be safe than sorry
         }
        }
       printf("\nDigit the quantity of products you bought: \n"); // Asks the user how many of that product he's bought from the provider
        scanf("%u", &(p + (*cont))->quantity); // Receives the number of products the user has bought
        while ((p + (*cont))->quantity <= 0) // Checks if the user has given a valid quantity of products bought
        {
            printf("\nYou cannot insert a negative number of merchandise,\nDigit the number again:"); // Repeats these two lines  while the user has not inserted a valid value
            scanf("%u", &(p + (*cont))->quantity); // Repeats these two lines while the user has not inserted a valid value
        }
         printf("\nDigit the price you paid to the provider for each of said product: \n"); // Asks for the price that the user has paid for a unit of that product
          scanf("%f", &(p + (*cont))->providerprice);// Receives the price that the user has paid for a unit of that product
        while ((p + (*cont))->providerprice <= 0)// Checks if the user has given a valid price for the product he bought
        {
            printf("\nYou cannot possibly have bought the merchandise for less than $0,\nDigit the price again:"); // Repeats these two lines while the user has not inserted a valid value
            scanf("%f", &(p + (*cont))->providerprice); // Repeats these two lines while the user has not inserted a valid value
        }
            *buy = *buy + (p + (*cont))->quantity; // Calculates the total sum of products that the establishment has bought
            *buy1 = *buy1 + ((p + (*cont))->quantity * (p + (*cont))->providerprice); // Calculates the total sum of products that the establishment has sold
        printf("\nDigit the price by unit that you will sell that product for: \n");
         scanf("%f", &(p + (*cont))->sellingprice);
             while ((p + (*cont))->sellingprice <= 0) // Checks if the user has given a valid price for the product he sold
        {
            printf("\nYou cannot sell the merchandise for less than $0,\nDigit the price again:"); // Repeats these two lines while the user has not inserted a valid value
            scanf("%f", &(p + (*cont))->sellingprice); // Repeats these two lines while the user has not inserted a valid value
        }
            (p + (*cont))->product_code = rand(); // Assigns a random code to the product
            for (i = 0; i < (*cont); i++)
            {
                if ((p + i)->product_code == (p + (*cont))->product_code) // Checks if there is already a product with that same code in the storage
                {
                    while ((p + i)->product_code == (p + (*cont))->product_code) // Repeats the line below while the new product has the same code as another one in the storage
                    {
                        (p + (*cont))->product_code = rand(); // Assigns a new random code to the product.
                    }
                }
            }
            fwrite((p + (*cont)), sizeof(products),1,fps); // Writes that product's data to the Produtos.txt file, allowing it to be read on a later program execution
            (*cont)++; // Increases the product number counter by 1.
    }
    FILE *contador;
   contador = fopen("Product Number.txt", "w"); // Begins the process of saving the product's data into Product Number.txt
    fprintf(contador, "%d", *cont); // saves the number of products
     fprintf(contador,"\n");
      fprintf(contador, "%d", *buy); // saves the number of products bought from the provider
       fprintf(contador,"\n");
        fprintf(contador, "%d", *sell); // saves the number of products sold to the consumers
         fprintf(contador,"\n");
          fprintf(contador, "%f", *buy1); // saves the amount of money spent in the bought products
           fprintf(contador,"\n");
            fprintf(contador, "%f", *sell1); // saves the amount of money made by the sold products
             fprintf(contador,"\n");
              fprintf(contador, "%f", *profit); // saves the Net profit, calculated beforehand
               fclose(contador); // Closes the Product Number.txt file
    fclose(fps); // Closes the Produtos.txt file
     animation(2);
}

void search_product (struct prod *p, unsigned int cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs) /** Searches for a product in the storage that
fits in the user's description. It uses a boolean variable to let the user decide between which kind of search he wants, since boolean type variables are executed faster and only use 1 bit of memory.
After the user has inserted the product he wants to search for, this functions goes through the products struct positions to see if this product exists, and if it exists, it retrieves all of its data*/
{
    SetColor(1);
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
         system("cls");
         printf("You do not have any product registered, please register a product before utilizing this function.");
         return;
        }
else
  {
    bool equal=0; // Used to check if the product the user has searched for has been found (0 = not found, 1 = found)
  printf("\nChoose an option:\n"); // Shows to the user the options he can use to search for the product
   printf("\n0 - Search by name.\n");
    printf("\n1 - Search by register.\n");
     printf("\n Your Choice: ");
      scanf("%d", &decs);
      while (decs < 0 || decs > 1) // Checks if the user has selected a valid option
      {
          printf("Invalid Option!\n Choose 0 or 1:");
          scanf("%d", &decs);
          setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
      }
  if (decs == 0) // Initializes the search of the product by name
    {
        printf("\nInsert the name of the product: ");
        scanf("%s", searchname); // Asks the user for the name of the product
        animation(5);
        for(ps = 0; ps < cont; ps++)
        {
         if (stricmp(searchname, (p + ps)->product_name) == 0) // Checks if the product has been found
            {
                printf("\n Product found!");
                 printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + ps)->product_name, (p + ps)->quantity, (p + ps)->providerprice, (p + ps)->sellingprice);
                 equal = 1; // Signals to the code that the product has been found
                 return;
            }
        }
        if (equal == 0) // checks if the product has not been found
            {
            system("cls");
            printf("\n Product not found.\n\n");
            return;
            }
    }
  if (decs == 1) // Initializes the search of the product by register(position)
   {
       printf("\n\tPrinting all positions and names:\n");
     for(regcheck = 0; regcheck < cont; regcheck ++) // prints all the positions used by the products, followed by their names only
        printf("\n%Position %d: %s", regcheck, (p + regcheck)->product_name);

      printf("\n\nWhich position do you want to request?: ");
      scanf("%u", &pos); // Asks the user about which product position he wants to get a more in-depth information about
      while (pos < 0 || pos > cont) // Checks if the user chose a valid position
        {
         printf("position out of bounds.\n Select another one: ");
         scanf ("%u", &pos);
         setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
        }
        animation(5);
      printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + pos)->product_name, (p + pos)->quantity, (p + pos)->providerprice, (p + pos)->sellingprice); // Prints that position's info
   }
  }
}

void show_products (struct prod *p, unsigned int cont, char searchname[], bool decs) /** This function prints all the products currently in the storage, or only a product expressly specified by the user. At a quick glance,
it is implemented in a similar fashion to the search_product() function, and it works almost the same way too. The reason for that is that since the operations performed by these functions is similar,
utilizing a similar base for both would save development time and also, make the code cleaner, since they would use the same variables too.*/
{
    SetColor(3);
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
         system("cls");
         printf("You do not have any product registered, please register a product before utilizing this function.");
         return;
        }
else
  {
  bool equal; // Used to check if the product the user has searched for has been found (0 = not found, 1 = found)
  unsigned int i;
    printf("\n\n\tWhat do you want to do?\n\n");  // Shows to the user the options he can use to search for the product
      printf("\t0 - Show a specific product.\n\n");
       printf("\t1 - Show all products.\n\n");
        printf("\tChoose an option:");
         scanf("%d", &decs);
    while (decs < 0 || decs > 1) // Checks if the user has selected a valid option
    {
     printf("Invalid Option!\n Choose 0 or 1:");
     scanf("%d", &decs);
     setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
    }
    if (decs == 0) // Initializes the search of the product by name, to show its information later.
    {
       printf("\nInsert the name of the product: ");
        scanf("%s", searchname); // Asks the user for the name of the product
        SetColor(5);
        animation(5);
        for(i = 0; i < cont; i++)
        {
         if (stricmp(searchname, (p + i)->product_name) == 0) // checks if the product has been found
            {
                system("cls");
                printf("\nProduct found!");
                printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + i)->product_name, (p + i)->quantity, (p + i)->providerprice, (p + i)->sellingprice);
                equal = 1; // Signals to the code that the product has been found
            }
        }
    }
    if (decs == 1) // Initializes the Displaying of all the products
    {
        animation(5);
        system("cls");
        printf("Printing all products.\n");
      for (i=0; i < cont; i++) // Displays the product's information, one position at a time.
      {
       SetColor(3);
       printf("\n\nProduct Name: ");
       SetColor(14);
       printf("%s\n", (p + i)->product_name);
       SetColor(3);
       printf("The quantity of products you bought: ");
       SetColor(14);
       printf("%d\n", (p + i)->quantity);
       SetColor(3);
       printf("The price you paid to the provider for each of said product: ");
       SetColor(14);
       printf("%.2f\n", (p + i)->providerprice);
       SetColor(3);
       printf("The price by unit that you will sell that product: ");
       SetColor(14);
       printf("%.2f\n", (p + i)->sellingprice);
       SetColor(3);
       printf("Product's Code: ");
       SetColor(14);
       printf("%d\n\n", (p + i)->product_code);
       equal = 1; // Signals the code that the products have been printed successfully
      }
    }
    if (equal == 0) // checks if the product has not been found, or if there was a error displaying all the products
    {
     system("cls");
      printf("\nProduct not found\n");
    }
   usleep(1000000);
  }
}

void edit_products (struct prod *p, unsigned int cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs) /** This function edits the information about a product specified by the user.
 At a quick glance, it is implemented in a similar fashion to the search_product() and show_products() functions, and it works almost the same way too.*/
{
    SetColor(4);
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
         system("cls");
         printf("You do not have any product registered, please register a product before utilizing this function.");
         return;
        }
else
  {
    bool edit; // Uses as a signal to the code if the product has been edited or not
    unsigned short int editnum; // Used to select which part of the product information the user wants to edit.
    printf("\n\nChoose an option:\n\n");
     printf("\n0 - Search by name.\n");
      printf("\n1 - Search by register.\n");
       printf("\n Your Choice: ");
        scanf("%d", &decs);
  while (decs < 0 || decs > 1) // Checks if the user has selected a valid option
    {
     printf("Invalid Option!\n Choose 1 or 2:");
     scanf("%d", &decs);
     setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
    }
  if (decs == 0) // Initializes the search of the product by name, to edit its information.
    {
        printf("\nInsert the name of the product: ");
        scanf("%s", searchname);
        for(ps = 0; ps < cont; ps++)
        {
         if (stricmp(searchname, (p + ps)->product_name) == 0) // checks if the product has been found
            {
                printf("\n Product found!");
                 printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + ps)->product_name, (p + ps)->quantity, (p + ps)->providerprice, (p + ps)->sellingprice);

                 printf("\nWhat do you want to edit?\n");
                  printf("\n1 - Name");
                   printf("\n2 - Quantity");
                    printf("\n3 - Provider price");
                     printf("\n4 - Selling price");
                      printf("\n Your Choice: ");
                       scanf("%d", &editnum);
                while (editnum < 1 || editnum > 4) // Checks if the user has selected a valid option
                {
                 printf("Invalid Option!\n Choose from 1 to 4:");
                  scanf("%d", &editnum);
                  setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
                }
                    SetColor(2);
                  switch (editnum) // Proceeds to edit the information about the part the user specified about the product
                  {
                  case 1 : printf("\nType the new name for %s: ", (p + ps)->product_name); scanf("%s", &(p + ps)->product_name); printf("Edit saved successfully"); break;
                  case 2 : printf("\nType the new quantity of %s: ", (p + ps)->product_name); scanf("%u", &(p + ps)->quantity); printf("Edit saved successfully"); break;
                  case 3 : printf("\nType the new provider price for %s: ", (p + ps)->product_name); scanf("%f", &(p + ps)->providerprice); printf("Edit saved successfully"); break;
                  case 4 : printf("\nType the new selling price for %s: ", (p + ps)->product_name); scanf("%f", &(p + ps)->sellingprice); printf("Edit saved successfully"); break;
                  default : printf("Invalid option, returning to the main menu."); break;
                  }
                edit = 1;
            }
        }
         if (edit == 0) printf("\n Product not found.");
    }
  if (decs == 1) // Initializes the display of all the products positions, to edit the information on the position selected by the user
   {
       printf("\n\tPrinting all positions and names:\n");
       for(regcheck = 0; regcheck < cont; regcheck ++) // Prints the positions occupied by the products struct.
       {
          printf("\n%Position %d: %s", regcheck, (p + regcheck)->product_name);
       }
        printf("\n\nWhich position do you want to request?: ");
         scanf("%u", &pos); // Asks the user about which product position he wants to get a more in-depth information about
         while (pos < 0 || pos > cont) // Checks if the user has selected a valid option
        {
         printf("position out of bounds.\n Select another one: ");
         scanf ("%u", &pos);
         setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
        }
        printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + pos)->product_name, (p + pos)->quantity, (p + pos)->providerprice, (p + pos)->sellingprice);

         printf("\nWhat do you want to edit?\n"); // Asks the user about the part of the product's information he wants to edit.
                 printf("\n1 - Name");
                  printf("\n2 - Quantity");
                   printf("\n3 - Provider price");
                    printf("\n4 - Selling price");
                     printf("\n Your Choice: ");
                      scanf("%d", &editnum);
                while (editnum < 1 || editnum > 4) // Checks if the user has selected a valid option
                {
                 printf("Invalid Option!\n Choose from 1 to 4:");
                  scanf("%d", &editnum);
                  setbuf(stdin,NULL); // Clears the in buffer to prevent the user from typing a char and causing a glitch in the code
                }
                SetColor(2);
                  switch (editnum) // Proceeds to edit the information about the part the user specified about the product
                  {
                   case 1 : printf("\nType the new name for %s: ", (p + pos)->product_name); scanf("%s", &(p + pos)->product_name); printf("\nEdit saved successfully.\n\n"); break;
                   case 2 : printf("\nType the new quantity of %s: ", (p + pos)->product_name); scanf("%u", &(p + pos)->quantity); printf("\nEdit saved successfully.\n\n"); break;
                   case 3 : printf("\nType the new provider price for %s: ", (p + pos)->product_name); scanf("%f", &(p + pos)->providerprice); printf("\nEdit saved successfully.\n\n"); break;
                   case 4 : printf("\nType the new selling price for %s: ", (p + pos)->product_name); scanf("%f", &(p + pos)->sellingprice); printf("\nEdit saved successfully.\n\n"); break;
                   default : printf("Invalid option, returning to the main menu."); break;
                  }
    edit = 1; // Signals the code that the product has been edited successfully
    usleep (500000);
   }
   if (edit == 1) // Checks if the product has been edited successfully
   {
   FILE *fre;
   fre = fopen ("Produtos.txt", "w"); // Opens the Produtos.txt file to overwrite the data inside it.
   check_fopen(NULL,NULL, fre); // Checks if the file has been opened successfully

      for (ps = 0; ps < cont; ps++) fwrite((p + ps), sizeof(products),1,fre); // Starts overwriting the products information.
     fclose (fre); // Closes the Produtos.txt file.
     usleep (500000);
   }
  }
}

void sell_products (struct prod *p, unsigned int cont, unsigned int *sell, float *sell1) /** This function edits the information about a product specified by the user.
 At a quick glance, it is implemented in a similar fashion to the search_product() and show_products() functions, and it works almost the same way too.*/
{
    SetColor(4);
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
         system("cls");
         printf("You do not have any product registered, please register a product before utilizing this function.");
         return;
        }
else
  {
  unsigned int y=0,i, x=0, quant;
  char name[25];
    printf("\n\nType the name of the product that you want do sell: \n");
     scanf("%s", name);
    for (i=0; i<cont;i++)
    {
        if (stricmp(name, (p + i)->product_name) == 0) // checks if the product has been found
            {
            y = 1; // Signals to the code that the product has been found
            printf("\nProduct found!\n\n");
             printf("\nType the quantity of the product that you want to sell: \n");
              scanf("%u", &quant);
            while (quant < 0)
            {
             printf("\nYou can't sell a negative number of merchandise.\nType the number of sold items again: ");
              scanf("%u", &quant); //Prevents you from typing a negative number.
            }
            if (quant > (p + i)->quantity)
            {
                while (quant > (p + i)->quantity && quant != 0)//Prevents you from selling a nonexistent quantity of the product.
                    {
                     printf("\nYou do not have the amount of this product in the storage, type another quantity or type 0 to exit.\n");
                      scanf("%u", &quant);
                    }
            }
            if (quant != 0) x=1;
            if (quant == 0) system("cls");
            if (x==1)
                {
                FILE *fp;
                fp = fopen("Produtos.txt","w"); //Open a file to overwrite its contents.
                check_fopen(NULL,NULL, fp); //Checks if the file has been opened successfully.


                (p + i)->quantity = (p + i)->quantity - quant; //Decreases the storage quantity.
                *sell = *sell + quant; // Saves the amount of products that have been sold.
                *sell1 = *sell1 + (quant * (p + i)->sellingprice); //Calculates the sum of the total of products sold.
                system("cls");
                SetColor(14);
                printf("You've sold this product with success!\n");
                    for (i = 0; i < cont; i++) fwrite((p + i), sizeof(products),1,fp);
                    fclose(fp); // Closes the file opened previously
                    animation(3);
                }
            }
    }
    if (y==0) // Checks if the product has not been found.
        {
         system("cls");
         printf("\nProduct not found, please try again.\n\n");
        }
  }
usleep(50000);
}

void inventory_control (struct prod *p, int cont)/**This function make a inventory control of all storage and offers some options to choose,
Its implementation was based on the concept of simplicity, allowing the user to select which kind of product he wants to see, and the code does the rest*/
{
    SetColor(4);
    int t;
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
         system("cls");
         printf("You do not have any product registered, please register a product before utilizing this function.");
         return;
        }
else
  {
        int x=0;
    char name[25];
   unsigned int opc, i, quant, y=0;

    while (opc != 4)
    {
     printf("\n\nChoose one of the following options: \n\n"); // Displays the menu containing all the options that the user can access.
      printf("\n1 - Products that does not have in the storage.\n");
       printf("\n2 - Storage's products.\n");
        printf("\n3 - Storage Restitution\n");
         printf("\n4 - Exit.\n");
          printf("Option: ");
           scanf("%u", &opc);
                if (opc != 1 && opc != 2 && opc != 3 && opc != 4) //Prevents you from typing an unlisted option.
                    {
                      while (opc != 1 && opc != 2 && opc != 3 && opc != 4)
                      {
                       printf("\nThis option does not exist, please try again.\nOption: ");
                        scanf("%u", &opc);
                        setbuf(stdin,NULL); // Clears the input buffer to prevent the user from typing a char and causing a glitch in the code
                      }
                    }
     if (opc == 1)
        {
         SetColor(4);
        printf("\n\nPrinting products that does not have any stock in the storage and need restocking: \n");
        for (i=0; i<cont; i++)
          {
            if ((p+i)->quantity == 0)
                printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + i)->product_name, (p + i)->quantity, (p + i)->providerprice, (p + i)->sellingprice);
          }
        }
        if (opc == 2)
        {
            SetColor(2);
                for (i=0;i<cont;i++) //Displays the products that have been sold.
                    {
                     if ((p+i)->quantity != 0)
                      printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + i)->product_name, (p + i)->quantity, (p + i)->providerprice, (p + i)->sellingprice);
                    }
        }
        if (opc == 3)
            {
                SetColor(14);
            printf("Type the name of the product that you want to restock: \n\n");
            scanf("%s", name);
            for (i=0; i<cont;i++)
            {
              if (stricmp(name, (p + i)->product_name) == 0) // Compare the names of the products to find what you want.
              {
               y = 1;
               x = i;
               printf("\nProduct found!\n\n");
               printf("\nType the quantity that you want to restock: \n");
               scanf("%u", &quant);
               (p + i)->quantity = (p + i)->quantity + quant; //The quantity of the product is increased.
              }

            }
        if (y==0)
            printf("\nProduct not found, please try again.\n\n");
        else
        printf("\nThe stock of %s has been restocked successfully!\n\n", (p + x)->product_name); //Shows a success message if the product was restocked in the storage.
        animation(2);
            }
    }
  }
FILE *fpm;
fpm = fopen("Produtos.txt","w"); // Open a writing file.

for (t=0; t<cont; t++)
    {
     fwrite((p + t), sizeof(products),1,fpm); //Write the new amount of the product in the file Produtos.txt.
    }
fclose(fpm);
system("cls");
}

void report_productstats (struct prod *p, int cont, int buy, int sell,float *profit, float *sell1, float *buy1) /**This function shows a report of all products that you sold, bought and the Net profit of your sales.
 Just like the function above, its implementation was based on the concept of simplicity, allowing the user to select which kind of product he wants to see, and the code does the rest*/
{
    SetColor(4);
    if (cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu.
        {
        system("cls");
        printf("You do not have any product registered, please register a product before utilizing this function.");
        return;
        }
else
  {
   int i,opc;
   *profit = *sell1 - *buy1; //Calculates the profit based on the sells and the buys.
    while (opc != 4)
    {
    printf("\n\nChoose one of the following options: \n\n");
     printf("1 - Show a report.\n");
      printf("\n2 - Products that does not have in the storage.\n");
       printf("\n3 - Storage's products.\n");
        printf("\n4 - Exit.\n");
         scanf("%d", &opc);
        if (opc != 1 && opc != 2 && opc != 3 && opc != 4) //Prevents you from typing an unlisted option.
        {
            while (opc != 1 && opc != 2 && opc != 3 && opc != 4) //Prevents you from typing an unlisted option.
                {
                 printf("\nThis option does not exist, please try again.\n");
                  scanf("%d", &opc);
                  setbuf(stdin,NULL); // Clears the input buffer to prevent the user from typing a char and causing a glitch in the code
                }
        }
        if (opc == 1) //Shows a general report of your the products you bought, sold, and the resulting Net Profit
            {
                SetColor(2);
            system("cls");
            ClearConsoleToColors(0, 15);
            animation(6);
        printf("    ______________________________________________________________________________\n");
        printf("    |                                                      REPORT                |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER: %d                      |\n", buy);
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS: %d            |\n", sell);
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD: %.2f                                                       |\n", *sell1);
        printf("    |                                                                            |\n");
        printf("    |    TOTAL PURCHASED: %.2f                                                 |\n", *buy1);
        printf("    |                                                                            |\n");
        printf("    |    PROFIT: %.2f                                                          |\n", *profit);
        printf("    |                                                                            |\n");
        printf("    |_____________________________REPORT FINISHED________________________________|\n");
            }
            if (opc == 2)
            {
            printf("\n\nPrinting products that are unavailable or in shortage and need replacement or restocking: \n");
            for (i=0; i<cont; i++)
             {
                 SetColor(4);
            if ((p+i)->quantity == 0) //Shows the products that you don't have in the storage and need replacement.
                printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + i)->product_name, (p + i)->quantity, (p + i)->providerprice, (p + i)->sellingprice);
             }
           }
        if (opc == 3) //Shows the products that you have in the storage, and are ready to be sold.
          {
            system("cls");
            SetColor(2);
            for (i=0;i<cont;i++)//Shows the products that you have in the storage, and are ready to be sold, by positions.
            {
             if ((p+i)->quantity != 0) printf("\n\nProd name: %s \n\nQuantity of products: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + i)->product_name, (p + i)->quantity, (p + i)->providerprice, (p + i)->sellingprice);
            }
          }
    }
  }
  ClearConsoleToColors(9, 0);
}

void delete_product (struct prod *p, unsigned int *cont, unsigned int regcheck, unsigned int ps, unsigned int pos, char searchname[], bool decs, FILE *frp) /** This function allows you delete a product by the name or or the position of the product.*/
{
    SetColor(1);
    if (*cont == 0) // Checks if the user has registered any products, if not, that function tells him to register a product and returns to the main menu
        {
        system("cls");
        printf("You do not have any product registered, please register a product before utilizing this function.");
        return;
        }
else
 {
  printf("\n\nChoose an option:\n\n"); //Function's main menu.
   printf("\n0 - Delete by name.\n");
    printf("\n1 - Delete by register.\n");
     printf("\n Your Choice: ");
      scanf("%d", &decs);
      while (decs < 0 || decs > 1) //Prevents you from typing an unlisted option.
    {
     printf("Invalid Option!\n Choose 1 or 2:");
     scanf("%d", &decs);
     setbuf(stdin,NULL); // Clears the input buffer to prevent the user from typing a char and causing a glitch in the code
    }
    if (decs == 0)
    {
        printf("\nInsert the name of the product: ");
        scanf("%s", searchname);
        for(ps = 0; ps < *cont; ps++) //Searches in all positions the name of the product that you want to delete.
        {
         if (stricmp(searchname, (p + ps)->product_name) == 0)
            {
                printf("\n Product found!");
                 printf("\n\n Deleting: \nProd name: %s \n\nQuantity of products you bought: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + ps)->product_name, (p + ps)->quantity, (p + ps)->providerprice, (p + ps)->sellingprice);
             for (pos = ps; ps < *cont; ps++) //Delete the product that you want and make a reallocation of all positions.
             {
              strcpy((p + ps)->product_name, (p + ps + 1)->product_name); // Reallocates the product's name
              (p + ps)->quantity = (p  + ps + 1)->quantity; // Reallocates the product's quantity
              (p + ps)->providerprice = (p + ps + 1)->providerprice; // Reallocates the unitary price of the product that has been bought
              (p + ps)->sellingprice = (p + ps + 1)->sellingprice; // Reallocates the unitary price of the product to be sold
             }
             (*cont)--; //Decreases the number of products by 1.
              frp = fopen("Produtos.txt", "w");
               check_fopen(NULL,NULL, frp); // Checks if the file has been opened successfully

                  for (ps = 0; ps < *cont; ps++) // Writes the new data into the file.
                    fwrite((p + ps), sizeof(products),1,frp);
            }
        else printf("\n Product not found.\n\n");
        }
    }
    if (decs == 1) //Displays the product that will be deleted, located in the position the user selected
     {
       printf("\n\tPrinting all positions and names:\n");
     for(regcheck = 0; regcheck < *cont; regcheck ++)
        printf("\n%Position %d: %s", regcheck, (p + regcheck)->product_name);

      printf("\n\nWhich position do you want to delete?: ");
       scanf("%u", &pos);
      printf("\n\n Deleting: \nProd name: %s \n\nQuantity of products you bought: %d \n\nPrice you paid to the provider for each of said product: %.2f \n\nPrice by unit that you will sell that product for: %.2f\n\n", (p + pos)->product_name, (p + pos)->quantity, (p + pos)->providerprice, (p + pos)->sellingprice);
       for (pos = pos; pos < *cont; pos++) //Deletes the product in the selected position reallocates all positions.
        {
         strcpy((p + pos)->product_name, (p + pos + 1)->product_name); // Reallocates the product's name
         (p + pos)->quantity = (p  + pos + 1)->quantity; // Reallocates the product's quantity
         (p + pos)->providerprice = (p + pos + 1)->providerprice; // Reallocates the unitary price of the product that has been bought
         (p + pos)->sellingprice = (p + pos + 1)->sellingprice; // Reallocates the unitary price of the product to be sold
        }
         (*cont)--; // Decreases the number of products by 1.
     }
   frp = fopen("Produtos.txt", "w"); // Writes the new data into the file.
    for (ps = 0; ps < *cont; ps++)
        fwrite((p + ps), sizeof(products),1,frp);
 }
}

void trashbin () /** Erases all the files and data currently used by the program, That means it deletes the Produtos.txt and Product Number.txt file, as well as the positions
allocated for the products struct by the malloc() function, it uses a rather simple implementation, with the free(p) function being actually used outside this function
since it caused some random force closes and crashes, depending on where in the code this functions was called, now this doesn't happen anymore*/
{
    SetColor(12);
    int i; // Despite having the same name as some variables above, it is used solely for the loop below
    system("cls"); // Cleans the screen, shifting the focus to the text below
  printf("Erasing Everything");
  for (i=0; i<5;i++) // This loop only exists for stylistic purposes.
  {
    printf(".");
    usleep(400000); //Prints a dot every 0.4 seconds, to simulate an animation.
  }
  animation(4);
   remove("Produtos.txt"); // Deletes the Produtos.txt file, present in the same folder as this code's executable
   remove("Product Number.txt"); // Deletes the Product Number.txt file, present in the same folder as this code's executable
   printf("\nDone!\n\n\n"); // Tells the user that the operation has been completed successfully
}

void SetColor(int ForgC) //Used for stylistic purposes, we should not need ti code that part of the library at all, but it seems that the windows.h header does not support that function at all.
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                 //Mask out all but the background attribute, and add in the foreground   color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }

 void ClearConsoleToColors(int ForgC, int BackC) //Used for stylistic purposes, we should not need ti code that part of the library at all, but it seems that the windows.h header does not support that function at all.
 {
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
               //Get the handle to the current output buffer...
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
                     //This is used to reset the carat/cursor to the top left.
 COORD coord = {0, 0};
                  //A return value... indicating how many chars were written
                    //   not used but we need to capture this since it will be
                      //   written anyway (passing NULL causes an access violation).
  DWORD count;

                               //This is a structure containing all of the console info
                      // it is used here to find the size of the console.
 CONSOLE_SCREEN_BUFFER_INFO csbi;
                 //Here we will set the current color
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                          //This fills the buffer with a given character (in this case 32=space).
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
                          //This will set our cursor position for the next print statement.
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}

void animation(unsigned int num) //Used for stylistic purposes(Yes, we wanted to do a very stylish code for the last exercise)
{
  int rep;
  if (num == 0)
  {
      SetColor(12);
      system("CLS");
 printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("             ________       \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("              ________      \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("               ________     \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("                ________    \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("                 ________   \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("                  ________  \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("                   ________ \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(50000);
 system("CLS");

  printf("              THE STORAGE COULD NOT BE LOADED            \n");
printf("                    ________\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |          EMPTY.\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |______|\n");

 usleep(700000);
  }

  if (num == 1)
 {
     SetColor(9);
     system("CLS");
 printf("              STORAGE LOADED SUCCESSFULLY                 \n");
 usleep(50000);
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("            ________       \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("             ________       \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("              ________      \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("               ________     \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("                ________    \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

 printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("                 ________   \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

 printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("                 ________   \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("                   ________ \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

  SetColor(10);
printf("              STORAGE LOADED SUCCESSFULLY                 \n");
printf("                    ________\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|       LOADING COMPLETE!    \n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
 }

  if(num == 2)
    {
        SetColor(2);
        system("CLS");
printf("\t\t\t\t\t                     _________________________________________________\n ");
printf("\t\t\t\t\t             /|     |                                                 |\n");
printf("\t\t\t\t\t             ||     |                                                 |\n ");
printf("\t\t\t\t\t        .----|-----,|                                                 | \n");
printf("\t\t\t\t\t        ||  ||   ==||                                                 | \n");
printf("\t\t\t\t\t   .-----'--'|   ==||                                                 | \n");
printf("\t\t\t\t\t   |)-      ~|     ||_________________________________________________|\n ");
printf("\t\t\t\t\t   | ___     |     |____...==..._  >|______________________________| \n");
printf("\t\t\t\t\t  [_/.-.|'--'--------   /.-.  .-.|/   |/             | .-.  .-. /    \n ");
printf("\t\t\t\t\t    ( o )`===========`  ( o )( o )    o              `( o )( o )`  \n ");
printf("\t\t\t\t\t     '-'                 '-'  '-'                      '-'  '-'    \n ");

usleep(50000);
system("cls");

printf("\t\t\t\t                     _________________________________________________ \n");
printf("\t\t\t\t             /|     |                                                 |\n");
printf("\t\t\t\t             ||     |                                                 |\n ");
printf("\t\t\t\t        .----|-----,|                                                 |\n ");
printf("\t\t\t\t        ||  ||   ==||                                                 | \n");
printf("\t\t\t\t   .-----'--'|   ==||                                                 | \n");
printf("\t\t\t\t   |)-      ~|     ||_________________________________________________| \n");
printf("\t\t\t\t   | ___     |     |____...==..._  >|______________________________| \n");
printf("\t\t\t\t  [_/.-.|'--'--------  /.-.  .-.|/   |/              | .-.  .-. /     \n");
printf("\t\t\t\t    ( o )`===========` ( o )( o )    o              `( o )( o )`   \n");
printf("\t\t\t\t     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("\t\t\t                     _________________________________________________ \n");
printf("\t\t\t             /|     |                                                 |\n");
printf("\t\t\t             ||     |                                                 | \n");
printf("\t\t\t        .----|-----,|                                                 | \n");
printf("\t\t\t        ||  ||   ==||                                                 | \n");
printf("\t\t\t   .-----'--'|   ==||                                                 | \n");
printf("\t\t\t   |)-      ~|     ||_________________________________________________| \n");
printf("\t\t\t   | ___     |     |____...==..._  >|______________________________| \n");
printf("\t\t\t  [_/.-.|'--'--------  /.-.  .-.|/   |/              | .-.  .-. /     \n");
printf("\t\t\t    ( o )`===========` ( o )( o )    o              `( o )( o )`  \n");
printf("\t\t\t     '-'                '-'  '-'                       '-'  '-'    \n");

usleep(50000);
system("cls");

printf("\t\t                     _________________________________________________ \n");
printf("\t\t             /|     |                                                 |\n");
printf("\t\t             ||     |                                                 | \n");
printf("\t\t        .----|-----,|                                                 | \n");
printf("\t\t        ||  ||   ==||                                                 | \n");
printf("\t\t   .-----'--'|   ==||                                                 | \n");
printf("\t\t   |)-      ~|     ||_________________________________________________| \n");
printf("\t\t   | ___     |     |____...==..._  >|______________________________| \n");
printf("\t\t  [_/.-.|'--'--------  /.-.  .-.|/   |/             | .-.  .-. /     \n");
printf("\t\t    ( o )`===========` ( o )( o )    o              `( o )( o )`   \n");
printf("\t\t    '-'                 '-'  '-'                      '-'  '-'     \n");

usleep(50000);
system("cls");

printf("\t                    _________________________________________________ \n");
printf("\t             /|     |                                                 |\n");
printf("\t             ||     |                                                 | \n");
printf("\t        .----|-----,|                                                 | \n");
printf("\t        ||  ||   ==||                                                 | \n");
printf("\t   .-----'--'|   ==||                                                 | \n");
printf("\t   |)-      ~|     ||_________________________________________________| \n");
printf("\t   | ___     |     |____...==..._  >|______________________________| \n");
printf("\t  [_/.-.|'--'--------  /.-.  .-./     |/             | .-.  .-. /     \n");
printf("\t    ( o )`===========' ( o )( o )     o              `( o )( o )`   \n");
printf("\t     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                 |\n");
printf("             ||     |                                                 | \n");
printf("        .----|-----,|                                                 | \n");
printf("        ||  ||   ==||                                                 | \n");
printf("   .-----'--'|   ==||                                                 | \n");
printf("   |)-      ~|     ||_________________________________________________| \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(700000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                      |\n");
printf("             ||     |                                                     | \n");
printf("        .----|-----,|                                                    | \n");
printf("        ||  ||   ==||                                                   | \n");
printf("   .-----'--'|   ==||                                                  | \n");
printf("   |)-      ~|     ||_________________________________________________| \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                      /\n");
printf("             ||     |                                                     / \n");
printf("        .----|-----,|                                                    / \n");
printf("        ||  ||   ==||                                                   / \n");
printf("   .-----'--'|   ==||                                                  / \n");
printf("   |)-      ~|     ||_________________________________________________/ \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                           /\n");
printf("             ||     |                                                         / \n");
printf("        .----|-----,|                                                       / \n");
printf("        ||  ||   ==||                                                     / \n");
printf("   .-----'--'|   ==||                                                   / \n");
printf("   |)-      ~|     ||_________________________________________________/ \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                           -\n");
printf("             ||     |                                                         - \n");
printf("        .----|-----,|                                                       - \n");
printf("        ||  ||   ==||                                                     - \n");
printf("   .-----'--'|   ==||                                                   - \n");
printf("   |)-      ~|     ||_________________________________________________/ \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                           \n");
printf("             ||     |                                                          _ \n");
printf("        .----|-----,|                                                        _ \n");
printf("        ||  ||   ==||                                                      _ \n");
printf("   .-----'--'|   ==||                                                    _ \n");
printf("   |)-      ~|     ||__________________________________________________ \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(50000);
system("cls");

printf("                     _________________________________________________ \n");
printf("             /|     |                                                  \n");
printf("             ||     |                                                  \n");
printf("        .----|-----,|                                                 \n");
printf("        ||  ||   ==||                                                  \n");
printf("   .-----'--'|   ==||                                                  _|_PP_|_P_|_PP_|_           \n");
printf("   |)-      ~|     ||___________________________________________________|_PP_|_P_|_PP_|____ \n");
printf("   | ___     |     |____...==..._  >|______________________________| \n");
printf("  [_/.-.|'--'--------  /.-.  .-.|/    |/             | .-.  .-. /     \n");
printf("    ( o )`===========` ( o )( o )     o              `( o )( o )`   \n");
printf("     '-'                '-'  '-'                       '-'  '-'     \n");

usleep(999999);
system("cls");
    }

 if (num == 3)
 {
     SetColor(14);
     system("CLS");
  printf("           ,                            \n");
  printf(" (_)______/________  \n");
  printf("  |-|-|/|-|-|-|-|/| \n");
  printf("   |==/-|-|-|-|-/ |  \n");
  printf("    |/|-|-|-|,-'  /  \n");
  printf("     |--|-''' ;  / \n");
  printf("      |_j_______/_\n");
  printf("       (_)     (_)\n");

  usleep(50000);
  system("cls");

  printf("               ,                           \n ");
  printf("     (_)______/________  \n");
  printf("  -   |-|-|/|-|-|-|-|/| \n");
  printf("   -   |==/-|-|-|-|-/ |  \n");
  printf("     -  |/|-|-|-|,-'  /  \n");
  printf("   -     |--|-''' ;  /  \n");
  printf("     -    |_j_______/_\n");
  printf("    --     (_)     (_)\n");

  usleep (50000);
  system("cls");

  printf("                   ,                        \n    ");
  printf("         (_)______/________  \n");
  printf("        - |-|-|/|-|-|-|-|/| \n");
  printf("       -   |==/-|-|-|-|-/ |  \n");
  printf("          - |/|-|-|-|,-'  /  \n");
  printf("         -   |--|-''' ;  /  \n");
  printf("     -        |_j_______/_\n");
  printf("    --   --  - (_)     (_)\n");

  usleep(50000);
  system("cls");

  printf("                       ,                         \n  ");
  printf("             (_)______/________  \n");
  printf("       -      |-|-|/|-|-|-|-|/| \n");
  printf("            -  |==/-|-|-|-|-/ |  \n");
  printf("         -    - |/|-|-|-|,-'  /  \n");
  printf("               - |--|-''' ;  /  \n");
  printf("          -    -  |_j_______/_\n");
  printf("    --   --   --   (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                           ,                       \n");
  printf("                 (_)______/________  \n");
  printf("             -    |-|-|/|-|-|-|-|/| \n");
  printf("                -  |==/-|-|-|-|-/ |  \n");
  printf("           -    -   |/|-|-|-|,-'  /  \n");
  printf("             -    -  |--|-''' ;  /  \n");
  printf("              -    -  |_j_______/_\n");
  printf("    --   --   --  --   (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                               ,                       \n");
  printf("                     (_)______/________  \n");
  printf("                   -  |-|-|/|-|-|-|-|/| \n");
  printf("                  -  - |==/-|-|-|-|-/ |  \n");
  printf("               -     -  |/|-|-|-|,-'  /  \n");
  printf("                   -  -  |--|-''' ;  /  \n");
  printf("                        - |_j_______/_\n");
  printf("    --   --   --  --  --   (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                                    ,                       \n");
  printf("                          (_)______/________  \n");
  printf("                   -     - |-|-|/|-|-|-|-|/| \n");
  printf("                -      -  - |==/-|-|-|-|-/ |  \n");
  printf("                    -   -  - |/|-|-|-|,-'  /  \n");
  printf("                     -   -  - |--|-''' ;  /  \n");
  printf("                      -      - |_j_______/_\n");
  printf("    --   --   --  --  --  --    (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                                        ,                       \n");
  printf("                              (_)______/________  \n");
  printf("                       -       |-|-|/|-|-|-|-|/| \n");
  printf("                    -      -    |==/-|-|-|-|-/ |  \n");
  printf("                        -     -  |/|-|-|-|,-'  /  \n");
  printf("                         -   -  - |--|-''' ;  /  \n");
  printf("                          -        |_j_______/_\n");
  printf("    --   --   --  --  --  --  --    (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                                            ,                       \n");
  printf("                                  (_)______/________  \n");
  printf("                                   |-|-|/|-|-|-|-|/| \n");
  printf("                        -         - |==/-|-|-|-|-/ |  \n");
  printf("                            -        |/|-|-|-|,-'  /  \n");
  printf("                                    - |--|-''' ;  /  \n");
  printf("                                 -   - |_j_______/_\n");
  printf("    --   --   --  --  --  --  --  --    (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                                                ,                       \n");
  printf("                                      (_)______/________  \n");
  printf("                                 -     |-|-|/|-|-|-|-|/| \n");
  printf("                        -      -     -  |==/-|-|-|-|-/ |  \n");
  printf("                            -    -     - |/|-|-|-|,-'  /  \n");
  printf("                                     -  - |--|-''' ;  /  \n");
  printf("                                -          |_j_______/_\n");
  printf("    --   --   --  --  --  --  --  --  --    (_)     (_)\n");

  usleep(50000);
  system ("cls");

  printf("                                                    ,                       \n");
  printf("                                          (_)______/________  \n");
  printf("                                        -  |-|-|/|-|-|-|-|/| \n");
  printf("                                     -      |==/-|-|-|-|-/ |  \n");
  printf("                                         -   |/|-|-|-|,-'  /  \n");
  printf("                                              |--|-''' ;  /  \n");
  printf("                                            -  |_j_______/_\n");
  printf("    --   --   --  --  --  --  --  --  --  --    (_)     (_)\n");

  usleep(900000);
  system ("cls");
 }

 if (num == 4)
  {
      SetColor(10);
      system("CLS");
printf("              DELETING DATA                \n");
printf("                    ________\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

   usleep(50000);
   system("CLS");
 SetColor(4);
printf("              DELETING DATA                \n");
printf("                  ________  \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                \n");
printf("                  ________  \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("             DELETING DATA                \n");
printf("                ________    \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                 \n");
printf("                ________    \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                 \n");
printf("               ________     \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("             DELETING DATA                 \n");
printf("              ________      \n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                \n");
printf("             ________       \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                 \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                 \n");
printf("             ________       \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                 \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |oooooo|\n");

 usleep(50000);
 system("CLS");

printf("              DELETING DATA                \n");
printf("            ________        \n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |       DATA DELETED SUCCESSFULLY\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");
printf("                    |      |\n");

 usleep(200000);
 system("CLS");


printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|  ~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~ ~~ ~  |\n");
printf("|               ~~~~~~~~          ~~~~~~~~~~~~      ~~~~    |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~    ~~~~~~~~~~ |\n");
printf("| ~~~~          ~~~~~~~~      ~~~~   ~~~~~~~~~~~~~~ ~~ ~~~  |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~    ~~~~~~     ~~  |\n");
printf("| ~~    ~~~~~~~~       ~~~~   ~~~~~~~~     ~~~~~   ~  ~ ~   |\n");
printf("| ~~                ~~~~~~   ~  ~~~~   ~~~~   ~ ~~~ ~  ~    |\n");
printf("|        ~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~    ~~~  ~~~    ~~~~~ |\n");
printf("| ~~            ~~~~~~~~~~     ~~~~~~~~~~~~~~~~~    ~~~~~~  |\n");
printf("|  ~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~  ~~~~   ~~~~ ~~~ |\n");
printf("|  ~~~~         ~~~~~~~~~~~~~~~~~~   ~~~~~ ~~~~~~~   ~~~~   |\n");
printf("| ~~               ~~~~~~~~~~~~~~~~   ~~~~~~   ~~~~   ~~    |\n");
printf("|  ~~     ~~~~~~~~~~ ~~~~      ~~~~~~~~~~      ~~~~  ~      |\n");
printf("|         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~     |\n");
printf("|          ~~~~~~~~~~~~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~  ~ |\n");
printf("|      ~~      ~~~               ~~~~~~~~~~~~~~ ~~~~~~~~~~  |\n");
printf("|      ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~ ~  |\n");
printf("|    ~~ ~~   ~~~~~~~~~~~~~~     ~~~~~~~~~~          ~~~~    |\n");
printf("|    ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~    |\n");
printf("|   ~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~  |\n");
printf("-------------------------------------------------------------\n");

     usleep(50000);
     system ("cls");

printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|  ~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~          |\n");
printf("|               ~~~~~~~~          ~~~~~~~~~~~~              |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~    ~~~        |\n");
printf("| ~~~~          ~~~~~~~~      ~~~~   ~~~~~~~~~~~~~~         |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~    ~~~~           |\n");
printf("| ~~    ~~~~~~~~       ~~~~   ~~~~~~~~     ~~~~~            |\n");
printf("| ~~                ~~~~~~   ~  ~~~~   ~~~~   ~ ~~~         |\n");
printf("|        ~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~    ~~~  ~~~          |\n");
printf("| ~~            ~~~~~~~~~~     ~~~~~~~~~~~~~~~~~            |\n");
printf("|  ~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~  ~~~~            |\n");
printf("|  ~~~~         ~~~~~~~~~~~~~~~~~~   ~~~~~ ~~~~~~~          |\n");
printf("| ~~               ~~~~~~~~~~~~~~~~   ~~~~~~   ~~~~         |\n");
printf("|  ~~     ~~~~~~~~~~ ~~~~      ~~~~~~~~~~      ~~~~         |\n");
printf("|         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~           |\n");
printf("|          ~~~~~~~~~~~~~~~~~~~~~~~~~~     ~~~~~~~~~         |\n");
printf("|      ~~      ~~~               ~~~~~~~~~~~~~~ ~~~         |\n");
printf("|      ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~       |\n");
printf("|    ~~ ~~   ~~~~~~~~~~~~~~     ~~~~~~~~~~             ~    |\n");
printf("|    ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~       |\n");
printf("|   ~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~       |\n");
printf("-------------------------------------------------------------\n");

     usleep(50000);
     system ("cls");

printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|  ~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                 |\n");
printf("|               ~~~~~~~~          ~~~~~~~~                  |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~               |\n");
printf("| ~~~~          ~~~~~~~~      ~~~~   ~~~~~~~~~              |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~                   |\n");
printf("| ~~    ~~~~~~~~       ~~~~   ~~~~~~~~                      |\n");
printf("| ~~                ~~~~~~   ~  ~~~~   ~~~                  |\n");
printf("|        ~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~    ~                 |\n");
printf("| ~~            ~~~~~~~~~~     ~~~~~~~~~~~~                 |\n");
printf("|  ~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~                    |\n");
printf("|  ~~~~         ~~~~~~~~~~~~~~~~~~   ~~~~~                  |\n");
printf("| ~~               ~~~~~~~~~~~~~~~~   ~~~~~~                |\n");
printf("|  ~~     ~~~~~~~~~~ ~~~~      ~~~~~~~~~~                   |\n");
printf("|         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                    |\n");
printf("|          ~~~~~~~~~~~~~~~~~~~~~~~~~~     ~~                |\n");
printf("|      ~~      ~~~               ~~~~~~~~~                  |\n");
printf("|      ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                 |\n");
printf("|    ~~ ~~   ~~~~~~~~~~~~~~     ~~~~~~~~~~                  |\n");
printf("|    ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                |\n");
printf("|   ~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                |\n");
printf("-------------------------------------------------------------\n");

     usleep(50000);
     system ("cls");

printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|  ~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~                           |\n");
printf("|               ~~~~~~~~                                    |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~~~                             |\n");
printf("| ~~~~          ~~~~~~~~      ~~                            |\n");
printf("| ~~   ~~~~~~~~~~~~~~~~~~~~~~                               |\n");
printf("| ~~    ~~~~~~~~       ~~~~                                 |\n");
printf("| ~~                ~~~~~~   ~                              |\n");
printf("|        ~~~~~~~~~~~~~~~~~~~~~                              |\n");
printf("| ~~            ~~~~~~~~~~                                  |\n");
printf("|  ~~~   ~~~~~~~~~~~~~~~~~~~~~~                             |\n");
printf("|  ~~~~         ~~~~~~~~~~~~~                               |\n");
printf("| ~~               ~~~~~~~~~~                               |\n");
printf("|  ~~     ~~~~~~~~~~ ~~~~                                   |\n");
printf("|         ~~~~~~~~~~~~~~                                    |\n");
printf("|          ~~~~~~~~~~~~~                                    |\n");
printf("|      ~~      ~~~                                          |\n");
printf("|      ~~~~  ~~~~~~~~~~~~~~                                 |\n");
printf("|    ~~ ~~   ~~~~~~~~~~~~~~                                 |\n");
printf("|    ~~   ~~~~~~~~~~~~~~~~                                  |\n");
printf("|   ~~    ~~~~~~~~~~~~~~                                    |\n");
printf("-------------------------------------------------------------\n");

    usleep(50000);
    system ("cls");

printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|  ~~~~~~~~  ~~~~~~~                                        |\n");
printf("|               ~                                           |\n");
printf("| ~~   ~~~~~~~                                              |\n");
printf("| ~~~~                                                      |\n");
printf("| ~~   ~~~~~~~~                                             |\n");
printf("| ~~    ~~~~~~~~                                            |\n");
printf("| ~~                                                        |\n");
printf("|        ~~~~~~                                             |\n");
printf("| ~~                                                        |\n");
printf("|  ~~~   ~~~~~~                                             |\n");
printf("|  ~~~~                                                     |\n");
printf("| ~~                                                        |\n");
printf("|  ~~     ~~~                                               |\n");
printf("|         ~~                                                |\n");
printf("|          ~~                                               |\n");
printf("|      ~~      ~~~                                          |\n");
printf("|      ~~~~  ~~~                                            |\n");
printf("|    ~~ ~~   ~~                                             |\n");
printf("|    ~~   ~~~                                               |\n");
printf("|   ~~    ~~~                                               |\n");
printf("-------------------------------------------------------------\n");
    usleep(50000);
    system ("cls");

printf(" __________________________________________________________\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("|                                                           |\n");
printf("-------------------------------------------------------------\n");

    usleep(50000);
    system ("cls");
     printf("                       /        \n");
     printf("                          /    \n");
     printf("                     /      \n");
     printf("                         /   \n");
     printf("                ´|´´`        \n");
     printf("               (<*|`*)      \n");
     printf("               (```|`        \n");
     printf("                          \n");
     printf("                          \n");
     printf("   _____________________  \n");
     printf("  |                     |  \n");
     printf("  |                     |  \n");
     printf("  |                    / \n");
     printf("   |                  / \n");
     printf("    |                / \n");
     printf("     |______________/ \n");

     usleep(50000);
     system ("cls");

     printf("                              \n");
     printf("                             \n");
     printf("                            \n");
     printf("                           \n");
     printf("                           \n");
     printf("                  /        \n");
     printf("                    /      \n");
     printf("                /          \n");
     printf("              /     /  /   \n");
     printf("   __________´|´´`______  \n");
     printf("  |         (<*|`*)     | \n");
     printf("  |         (```|`      | \n");
     printf("  |                    / \n");
     printf("   |                  / \n");
     printf("    |                / \n");
     printf("     |______________/\n");

     usleep(50000);
     system ("cls");

     printf("                               \n");
     printf("                              \n");
     printf("                            \n");
     printf("                            \n");
     printf("                           \n");
     printf("                          \n");
     printf("                            \n");
     printf("                           \n");
     printf("              /  /  /        \n");
     printf("   _____________/_______  \n");
     printf("  |               /     | \n");
     printf("  |         ´|´´`  /    | \n");
     printf("  |        (<*|`*)     / \n");
     printf("   |       (```|`     / \n");
     printf("    |                / \n");
     printf("     |______________/ \n");

     usleep(50000);
     system ("cls");

     printf("                               \n");
     printf("                             \n");
     printf("                          \n");
     printf("                          \n");
     printf("                         \n");
     printf("                           \n");
     printf("                           \n");
     printf("                           \n");
     printf("                              \n");
     printf("   _____________________ \n");
     printf("  |      /   /          |  \n");
     printf("  |      /    /         | \n");
     printf("  |     ´|´´`          / \n");
     printf("   |   (<*|`*)        / \n");
     printf("    |  (```|`        / \n");
     printf("     |______________/ \n");

     usleep(50000);
     system ("cls");

     printf("                               \n");
     printf("                             \n");
     printf("                          \n");
     printf("                          \n");
     printf("                         \n");
     printf("                           \n");
     printf("                           \n");
     printf("                           \n");
     printf("                              \n");
     printf("   _____________________ \n");
     printf("  |                     |  \n");
     printf("  |     /    /          | \n");
     printf("  |      /             / \n");
     printf("   |   ´|´´`  /       / \n");
     printf("    | (<*|`*)        / \n");
     printf("     |(```|`________/ \n");

     usleep(50000);
     system ("cls");

     printf("                              \n");
     printf("                               \n");
     printf("                             \n");
     printf("                           \n");
     printf("                           \n");
     printf("                            \n");
     printf("                            \n");
     printf("                            \n");
     printf("                             \n");
     printf("   _____________________   \n");
     printf("  |                     |   \n");
     printf("  |                     | \n");
     printf("  |    /  /            / \n");
     printf("   |     >  ´|´´`     / \n");
     printf("    | / > (<*|`*)    / \n");
     printf("     |____(```|`____/ \n");

     usleep(700000);
     system ("cls");
  }

 for (rep = 0; rep <= 1; rep ++)
  {
  if (num == 5)
   {
 SetColor(11);
 system("CLS");
printf(" _____________________________________________________\n");
printf("|                                                      |\n");
printf("|  ~~~~~~~~  ______  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("|         .-'      `-..         ~~~~~~~~               |\n");
printf("| ~~    .'            .`.  ~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("| ~~~~ / struct prod   ``´           ~~~~~~~~          |\n");
printf("| ~~  ;  oduct_name[25]; ;`   ~~~~~~~~~~~~~~~~~~~~~~   |\n");
printf("| ~~  | gned int quantit |;     ~~~~~~~~       ~~~~    |\n");
printf("| ~~  ; providerprice, se;|               ~~~~~~       |\n");
printf("|     '`   ddata;       / ;     ~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("| ~~  ` `. ddata prod.' /             ~~~~~~~~~~       |\n");
printf("|  ~~~  `.`-._____.-' .'  ~~~~~~~~~~~~~~~~~~~~~~~~~~   |\n");
printf("|  ~~~~   / /`_____.-'          ~~~~~~~~~~~~~~~~~~     |\n");
printf("| ~~     / / /                 ~~~~~~~~~~~~~~~~        |\n");
printf("|  ~~   / / /                  ~~~~~~~~~~              |\n");
printf("|      / / /     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~        |\n");
printf("|     / / /        ~~~~~~~~~~~~~~~~~~~~~~~~~~          |\n");
printf("|    / / /   ~~      ~~~               ~~~~~~~~~~~~~~  |\n");
printf("|   / / /   ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     |\n");
printf("|  / / /   ~~ ~~   ~~~~~~~~~~~~~~                      |\n");
printf("| / / /   ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf(" / / /   ~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("-`/_/---------------------------------------------------\n");

usleep(150000);
system ("cls");

printf(" _____________________________________________________\n");
printf("|                                                      |\n");
printf("|            ~~~~~~~~  ______  ~~~~~~~~~~~~~~~~~~~     |\n");
printf("|      ~~~~~~       .-'      `-..         ~~~~~~~~     |\n");
printf("| ~~      ~~      .'            .`.  ~~~~~~~~~~~~~~~~  |\n");
printf("|           ~~~ ; /  prod        ``´           ~~~~~~  |\n");
printf("| ~~  ~~~~      ;  name[25];       ;`   ~~~~~~~~~~~~   |\n");
printf("| ~~ ~~~~~~     |  quantity, produc|;     ~~~~~~~~     |\n");
printf("| ~~    ~~~~    ; price, sellingp  ;|                  |\n");
printf("|        ~~     '`                / ;     ~~~~~~~~~~~~ |\n");
printf("| ~~            ` `.products;  .' /             ~~~~~  |\n");
printf("|  ~~~   ~~~~     `.`-._____.-' .'  ~~~~~~~~~~~~~~~~~~ |\n");
printf("|  ~~~~   ~~~~  ~~  / /`_____.-'          ~~~~~~~~~~~~ |\n");
printf("| ~~      ~~   ~~  / / /          ~~     ~~~~~~~~~~~~~ |\n");
printf("|  ~~    ~~~~     / / /      ~~~~~~      ~~~~~~~~~~    |\n");
printf("|   ~~~~~~    ~~ / / /     ~~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|         ~~    / / /        ~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|    ~~~~      / / /   ~~      ~~~               ~~~~~ |\n");
printf("|    ~~   ~~  / / /   ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|  ~~   ~~   / / /   ~~ ~~   ~~~~~~~~~~~~~~            |\n");
printf("|     ~~    / / /   ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|   ~~~~   / / /      ~~~~~~~~~~~~~~~~~~~~~~    ~~     |\n");
printf("-----------`/_/-----------------------------------------\n");

usleep(150000);
system ("cls");

printf(" _____________________________________________________\n");
printf("|                                                      |\n");
printf("|            ~~~~~~~~   ~~~~      ______  ~~~~~~~~~~~~~|\n");
printf("|      ~~~~~~    ~~~~~~        .-'      `-..         ~ |\n");
printf("| ~~      ~~        ~~~~     .'            .`.  ~~~~~~ |\n");
printf("|           ~~~            ; /              ``´        |\n");
printf("| ~~  ~~~~~~~~  ~~  ~~~~   ;  25];            ;`   ~~~ |\n");
printf("| ~~ ~~~~~~     ~~~~~~     |  ty, product_code|;       |\n");
printf("| ~~    ~~~~    ~~~~~~~~   ; sellingprice     ;|       |\n");
printf("|        ~~ ~~~~~~~~~~~    '`                / ;       |\n");
printf("| ~~     ~~~~~~~~~~~~      ` `.cts;       .' /         |\n");
printf("|  ~~~   ~~~~                `.`-._____.-' .'  ~~~~~~  |\n");
printf("|  ~~~~   ~~~~  ~~    ~~~~~    / /`_____.-'          ~ |\n");
printf("| ~~      ~~   ~~    ~~~~     / / /          ~~     ~~~|\n");
printf("|  ~~    ~~~~       ~~~~     / / /      ~~~~~~      ~  |\n");
printf("|   ~~~~~~    ~~  ~~~~~~    / / /     ~~~~~~~~~~~~~~~~~|\n");
printf("|         ~~      ~~~~     / / /        ~~~~~~~~~~~~~~~|\n");
printf("|    ~~~~    ~~~~         / / /   ~~      ~~~          |\n");
printf("|    ~~   ~~    ~~~~~~   / / /   ~~~~  ~~~~~~~~~~~~~~~~|\n");
printf("|  ~~   ~~  ~~~~        / / /   ~~ ~~   ~~~~~~~~~~~~~~ |\n");
printf("|     ~~ ~~~~~~~~~~~~  / / /   ~~   ~~~~~~~~~~~~~~~~~~ |\n");
printf("|   ~~~~   ~~~~~~~~   / / /      ~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("----------------------`/_/------------------------------\n");

usleep(150000);
system ("cls");

printf(" _____________________________________________________\n");
printf("|                                                      |\n");
printf("|            ~~~~~~~~  ______  ~~~~~~~~~~~~~~~~~~~     |\n");
printf("|      ~~~~~~       .-'      `-..         ~~~~~~~~     |\n");
printf("| ~~      ~~      .'            .`.  ~~~~~~~~~~~~~~~~  |\n");
printf("|           ~~~ ; /  prod        ``´           ~~~~~~  |\n");
printf("| ~~  ~~~~      ;  name[25];       ;`   ~~~~~~~~~~~~   |\n");
printf("| ~~ ~~~~~~     |  quantity, produc|;     ~~~~~~~~     |\n");
printf("| ~~    ~~~~    ; price, sellingp  ;|                  |\n");
printf("|        ~~     '`                / ;     ~~~~~~~~~~~~ |\n");
printf("| ~~            ` `.products;  .' /             ~~~~~  |\n");
printf("|  ~~~   ~~~~     `.`-._____.-' .'  ~~~~~~~~~~~~~~~~~~ |\n");
printf("|  ~~~~   ~~~~  ~~  / /`_____.-'          ~~~~~~~~~~~~ |\n");
printf("| ~~      ~~   ~~  / / /          ~~     ~~~~~~~~~~~~~ |\n");
printf("|  ~~    ~~~~     / / /      ~~~~~~      ~~~~~~~~~~    |\n");
printf("|   ~~~~~~    ~~ / / /     ~~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|         ~~    / / /        ~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|    ~~~~      / / /   ~~      ~~~               ~~~~~ |\n");
printf("|    ~~   ~~  / / /   ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|  ~~   ~~   / / /   ~~ ~~   ~~~~~~~~~~~~~~            |\n");
printf("|     ~~    / / /   ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("|   ~~~~   / / /      ~~~~~~~~~~~~~~~~~~~~~~    ~~     |\n");
printf("-----------`/_/-----------------------------------------\n");

usleep(150000);
system ("cls");

printf(" _____________________________________________________\n");
printf("|                                                      |\n");
printf("|  ~~~~~~~~  ______  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("|         .-'      `-..         ~~~~~~~~               |\n");
printf("| ~~    .'            .`.  ~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("| ~~~~ / struct prod   ``´           ~~~~~~~~          |\n");
printf("| ~~  ;  oduct_name[25]; ;`   ~~~~~~~~~~~~~~~~~~~~~~   |\n");
printf("| ~~  | gned int quantit |;     ~~~~~~~~       ~~~~    |\n");
printf("| ~~  ; providerprice, se;|               ~~~~~~       |\n");
printf("|     '`   ddata;       / ;     ~~~~~~~~~~~~~~~~~~~~~~ |\n");
printf("| ~~  ` `. ddata prod.' /             ~~~~~~~~~~       |\n");
printf("|  ~~~  `.`-._____.-' .'  ~~~~~~~~~~~~~~~~~~~~~~~~~~   |\n");
printf("|  ~~~~   / /`_____.-'          ~~~~~~~~~~~~~~~~~~     |\n");
printf("| ~~     / / /                 ~~~~~~~~~~~~~~~~        |\n");
printf("|  ~~   / / /                  ~~~~~~~~~~              |\n");
printf("|      / / /     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~        |\n");
printf("|     / / /        ~~~~~~~~~~~~~~~~~~~~~~~~~~          |\n");
printf("|    / / /   ~~      ~~~               ~~~~~~~~~~~~~~  |\n");
printf("|   / / /   ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     |\n");
printf("|  / / /   ~~ ~~   ~~~~~~~~~~~~~~                      |\n");
printf("| / / /   ~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf(" / / /   ~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
printf("-`/_/---------------------------------------------------\n");
 usleep(100000);
 system("cls");
    }
  }
  if (num==6){
     system("CLS");
        printf("    ______________________________________________________________________________\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL PURCHASED:                                                        |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL PURCHASED:                                                        |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL PURCHASED:                                                        |\n");
        printf("    |                                                                            |\n");
        printf("    |    PROFIT:                                                                 |\n");
        usleep(50000);
        system("cls");
        printf("    ______________________________________________________________________________\n");
        printf("    |                                     REPORT                                 |\n");
        printf("    |                                                                            |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS PURCHASED FROM THE PROVIDER:                         |\n");
        printf("    |                                                                            |\n");
        printf("    |    AMOUNT OF PRODUCTS THAT HAVE BEEN SOLD TO THE CUSTOMERS:                |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL SOLD:                                                             |\n");
        printf("    |                                                                            |\n");
        printf("    |    TOTAL PURCHASED:                                                        |\n");
        printf("    |                                                                            |\n");
        printf("    |    PROFIT:                                                                 |\n");
        printf("    |                                                                            |\n");
        usleep(50000);
         system("cls");
    }
}
