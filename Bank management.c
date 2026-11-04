#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>   // for getch()
#include <math.h>	// for emi calculation formulae

//Structure for Account
struct Account
{
    char name[50];
    int accountNumber;
    int pin;
    float balance;
    float mutualFunds;
    float mediclaimCoverage;
};

// Function to mask PIN input (shows * instead of numbers)
int getMaskedPIN() 
{
    char ch;
    int pin = 0;
    while (1) 
	{
        ch = getch();
        if (ch == 13)  // Enter key
            break;
        else if (ch >= '0' && ch <= '9') 
		{  // Only digits allowed
            pin = pin * 10 + (ch - '0');
            printf("*");   // Print * instead of digit
        }
        else if (ch == 8) 
		{ // Backspace
            printf("\b \b");
            pin /= 10;
        }
    }
    return pin;
}

// Function to update balance in file
void updateBalance(struct Account user) 
{
    FILE *fp, *tempFile;
    struct Account temp;

    fp = fopen("accounts.txt", "r");
    tempFile = fopen("temp.txt", "w");

    if (fp == NULL || tempFile == NULL) 
	{
        printf("Error updating balance!\n");
        return;
    }

    // Copy all accounts to temp file, but update the matching one
    while (fscanf(fp, "%s %d %d %f", temp.name,
                  &temp.accountNumber,
                  &temp.pin,
                  &temp.balance) != EOF) 
				  {
        if (temp.accountNumber == user.accountNumber)
		{
            fprintf(tempFile, "%s %d %d %.2f\n", user.name,
                    user.accountNumber,
                    user.pin,
                    user.balance);
        } else 
		{
            fprintf(tempFile, "%s %d %d %.2f\n", temp.name,
                    temp.accountNumber,
                    temp.pin,
                    temp.balance);
        }
    }

    fclose(fp);
    fclose(tempFile);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

// Register a new account
void registerUser() 
{
    FILE *fp;
    struct Account newAccount;

    fp = fopen("accounts.txt", "a");  // Append mode (adds at end)
    if (fp == NULL) 
	{
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Register New Account ---\n");
    printf("Enter Name: ");
    scanf("%s", newAccount.name);
    printf("Enter Account Number: ");
    scanf("%d", &newAccount.accountNumber);
    printf("Set 4-digit PIN: ");
    newAccount.pin = getMaskedPIN();
    printf("\nEnter Initial Balance: ");
    scanf("%f", &newAccount.balance);

    // Save details into file (text format, human-readable)
    fprintf(fp, "%s %d %d %.2f\n", newAccount.name,
            newAccount.accountNumber,
            newAccount.pin,
            newAccount.balance);

    fclose(fp);
    printf("\nAccount Registered Successfully!\n");
}

// Banking operations after login
void bankingMenu(struct Account user) 
{
    int choice;
    float amount;

    while (1) {
        printf("\n--- Main Banking Menu ---\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Apply for Loan\n");
        printf("5. Mutual Funds\n");
        printf("6. Mediclaim\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
		{
            case 1:
                printf("Your Balance: %.2f\n", user.balance);
                break;

            case 2:
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                user.balance += amount;
                updateBalance(user);  // save new balance
                printf("Deposited successfully! New Balance: %.2f\n", user.balance);
                break;

            case 3:
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);
                if (amount > user.balance) {
                    printf("Insufficient balance!\n");
                } 
				else 
				{
                    user.balance -= amount;
                    updateBalance(user);  // save new balance
                    printf("Withdrawn successfully! \nRemaining Balance: %.2f\n", user.balance);
                }
                break;

            case 4:
                applyLoan(&user);
                break;

            case 5:
                mutualFundsMenu(&user);
                break;

            case 6:
                mediclaimMenu(&user);
                break;

            case 7:
                printf("Logging out...\n");
                return; // Exit to main screen

            default:
                printf("Invalid choice!\n");
        }
    }
}

// Login with account number & PIN
void loginUser()
{
    FILE *fp;
    struct Account temp;
    int accNo, pin, found = 0;

    fp = fopen("accounts.txt", "r");
    if (fp == NULL) {
        printf("No accounts found! Please register first.\n");
        return;
    }

    printf("\n--- Login ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter PIN: ");
    pin = getMaskedPIN();
    printf("\n");

    // Search in file
    while (fscanf(fp, "%s %d %d %f", temp.name,
                  &temp.accountNumber,
                  &temp.pin,
                  &temp.balance) != EOF) 
				  {
        if (temp.accountNumber == accNo && temp.pin == pin) 
		{
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("\nWelcome, %s!\n", temp.name);
        bankingMenu(temp); // Go to main banking menu
    } else {
        printf("\nInvalid Account Number or PIN!\n");
    }
}

void listUsers()
{
	printf("\n\nThis function prints all users available in the system.");
	FILE *fp;
	struct Account temp;
	fp = fopen("accounts.txt", "r");
    if (fp == NULL) 
	{
        printf("No accounts found! Please register first.\n");
        return;
    }
    
    printf("\n---List of Registers users---");
    printf("\n%-15s | %-15s", "NAME", "Account number");
    printf("\n-----------------------------");
    
	
	
	//Search and Show details into file (text format, human-readable)
	
	// Search in file
    while (fscanf(fp, "%s %d %d %f", temp.name,
                  &temp.accountNumber,
                  &temp.pin,
                  &temp.balance) != EOF)
				  {
                	printf("\n%-15s | %-15d ", temp.name,
           						temp.accountNumber);
			}

    fclose(fp);
    printf("\n\nAll Accounts are saved Successfully!\n");
}

// Loan facility
void applyLoan(struct Account *user) {
    float loanAmount, rate, emi;
    int time;

    printf("\n--- Loan Facility ---\n");
    printf("Enter Loan Amount: ");
    scanf("%f", &loanAmount);
    printf("Enter Time Period (in years): ");
    scanf("%d", &time);
    printf("Enter Annual Interest Rate (in %%): ");
    scanf("%f", &rate);

    // Convert annual rate to monthly rate and time to months
    float r = rate / (12 * 100);
    int n = time * 12;

    // EMI formula: EMI = P*r*(1+r)^n / ((1+r)^n - 1)
    emi = (loanAmount * r * pow(1 + r, n)) / (pow(1 + r, n) - 1);

    printf("\nYour Loan Details:\n");
    printf("Loan Amount: Rs.%.2f\n", loanAmount);
    printf("Time Period: %d years (%d months)\n", time, n);
    printf("Interest Rate: %.2f%%\n", rate);
    printf("Monthly EMI: Rs.%.2f\n", emi);
    printf("Total Payable: Rs.%.2f\n", emi * n);

    // Credit the loan amount to user’s balance
    user->balance += loanAmount;
    updateBalance(*user);

    printf("\n Loan Approved! Rs.%.2f has been credited to your account.\n", loanAmount);
    printf("New Balance: Rs.%.2f\n", user->balance);
}

//  MUTUAL FUNDS FEATURE
void mutualFundsMenu(struct Account *user) {
    int choice;
    float amount;
    float growthRate = 0.08; // 8% growth

    while (1) {
        printf("\n--- Mutual Funds ---\n");
        printf("1. Invest Money\n");
        printf("2. Redeem (Sell) Mutual Funds\n");
        printf("3. View Investment Value\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter amount to invest: ");
                scanf("%f", &amount);
                if (amount > user->balance)
                    printf("Insufficient Balance!\n");
                else {
                    user->balance -= amount;
                    user->mutualFunds += amount;
                    updateBalance(*user);
                    printf("Invested Successfully!\n");
                }
                break;

            case 2:
                printf("Enter amount to redeem: ");
                scanf("%f", &amount);
                if (amount > user->mutualFunds)
                    printf("Not enough mutual funds!\n");
                else {
                    user->mutualFunds -= amount;
                    user->balance += amount;
                    updateBalance(*user);
                    printf("Redeemed Successfully!\n");
                }
                break;

            case 3:
                printf("\nYour Mutual Fund Investment: %.2f\n", user->mutualFunds);
                printf("Estimated Value After Growth (8%%): %.2f\n",
                       user->mutualFunds * (1 + growthRate));
                break;

            case 4:
                return;

            default:
                printf("Invalid Choice!\n");
        }
    }
}

// NEW : MEDICLAIM MENU
void mediclaimMenu(struct Account *user) {
    int choice;
    float premium, claim;

    while (1) {
        printf("\n--- Mediclaim / Health Insurance ---\n");
        printf("1. Buy Mediclaim Policy\n");
        printf("2. Claim Insurance\n");
        printf("3. View Policy Details\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter Premium Amount to Pay: ");
                scanf("%f", &premium);
                if (premium > user->balance)
                    printf("Not Enough Balance!\n");
                else {
                    user->balance -= premium;
                    user->mediclaimCoverage += premium * 5; // 5x coverage
                    updateBalance(*user);
                    printf("\n Mediclaim Purchased!\nCoverage Added: ?%.2f\n", premium * 5);
                }
                break;

            case 2:
                printf("Enter Claim Amount: ");
                scanf("%f", &claim);
                if (claim > user->mediclaimCoverage)
                    printf("Insufficient Coverage!\n");
                else {
                    user->mediclaimCoverage -= claim;
                    user->balance += claim;
                    updateBalance(*user);
                    printf(" Claim Approved! %.2f Credited.\n", claim);
                }
                break;

            case 3:
                printf("\nMediclaim Coverage: %.2f\n", user->mediclaimCoverage);
                break;

            case 4:
                return;

            default:
                printf("Invalid Choice.\n");
        }
    }
}

// Main Menu (Before Login/Register)
int main() 
{
    int choice;
    while (1) 
	{
        printf("\n=== Bank Management System ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. List Users\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
		{
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: listUsers(); break;
            case 4: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
