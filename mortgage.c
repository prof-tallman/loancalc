
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_util.h"


// Each row in the amortization table
typedef struct loan_month
{
    float payment;
    float interest;
    float principal;
    float balance;
} LOAN_MONTH;


int main(int argc, char* argv[])
{
    // ------------- READ AND CONVERT THE CMDLINE PARAMETERS -------------------

    // Parse Command Line Arguments
    // prog.exe <amount> <interest> <months> [extra]

    // Check the number of parameters
    // It's should be safe to say that argv[0] exists because it is the name of this program
    if (argc < 4)
    {
        printf("Error: missing parameters\n");
        printf("Usage: %s <amount> <interest> <months>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initial parameters
    float famount = 0.0;    // original loan amount--will convert to integer
    float interest = 0.0;   // interest rate as a percent 4.2%
    int months = 0;         // number of months for the loan
    int rc = EXIT_FAILURE;

    // Check that the parameters can be converted from strings to numbers
    rc = to_float(argv[1], &famount);
    if (rc != EXIT_SUCCESS)
    {
        printf("Error: parameter 'amount' is invalid\n");
        return EXIT_FAILURE;
    }
    rc = to_float(argv[2], &interest);
    if (rc != EXIT_SUCCESS)
    {
        printf("Error: parameter 'interest' is invalid\n");
        return EXIT_FAILURE;
    }
    rc = to_integer(argv[3], &months);
    if (rc != EXIT_SUCCESS)
    {
        printf("Error: parameter 'months' is invalid\n");
        return EXIT_FAILURE;
    }

    // Check that the numbers are within a reasonable range
    if (famount < 0.0 || famount > 10000000)
    {
        printf("Error: parameter is out of range 0.0 < 'amount' < 10000000\n");
        return EXIT_FAILURE;
    }
    if (interest < 0.0 || interest > 30.0)
    {
        printf("Error: parameter is out of range 0.0 < 'interest' < 30.0 \n");
        return EXIT_FAILURE;
    }
    if (months < 0 || months > 600)
    {
        printf("Error: parameter is out of range 0 < 'months' < 600\n");
        return EXIT_FAILURE;
    }

    // --------------------- RUN THE BASIC LOAN MATH ---------------------------

    // Convert the principal amount to number of cents to make the math easier
    int amount = (int)(famount * 100);

    // Calculate the default monthly payment
    double rate = interest / 100 / 12;
    double pterm = pow(1 + rate, months);
    double apayment = (amount * rate * pterm) / (pterm - 1);
    double fpayment = apayment / 100;
    printf("\nMortgage Amortization Calculator Program\n");
    printf("Default Payment: $%.2f\n", fpayment);

    // ------------------ CALCULATE THE AMORTIZATION TABLE ------------------------

    // Create the amortization table
    // Add one extra month to show the original balance
    LOAN_MONTH* at1 = (LOAN_MONTH*)malloc((months+1) * sizeof(LOAN_MONTH));
    if (at1 == NULL)
    {
        printf("Error: unable to allocate amortization table\n");
        return EXIT_FAILURE;
    }

    // The first month shows the loan before any payments have been made
    at1[0].payment = 0.0;
    at1[0].interest = 0.0;
    at1[0].principal = 0.0;
    at1[0].balance = famount;
    
    // Calculate all the regular payments, stopping when the loan balance is
    // less than a single month's payment.
    int i = 1;
    while(at1[i-1].balance > fpayment)
    {
        at1[i].payment = fpayment;
        at1[i].interest = at1[i-1].balance * rate;
        at1[i].principal = fpayment - at1[i].interest;
        at1[i].balance = at1[i-1].balance - at1[i].principal;
        i += 1;
    }

    // The last month probably has a lesser amount due
    at1[i].interest = at1[i-1].balance * rate;
    at1[i].principal = at1[i-1].balance;
    at1[i].payment = at1[i].interest + at1[i].principal;
    at1[i].balance = 0;
    int n_payments = i;

    // If any extra was applied to the principal, the loan will be paid off
    // early. This means that the actual amortization table will be shorter.
    LOAN_MONTH* at2 = (LOAN_MONTH*)malloc((n_payments) * sizeof(LOAN_MONTH));
    if (at2 == NULL)
    {
        printf("Error: unable to allocate amortization table\n");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < n_payments; i++)
    {
        at2[i].payment = at1[i].payment;
        at2[i].interest = at1[i].interest;
        at2[i].principal = at1[i].principal;
        at2[i].balance = at1[i].balance;
    }

    // Now that we've copied the raw caluclations into the final table (at2),
    // we can go ahead and deallocate the original, scratch amortization table.
    free(at1);

    // ------------------ OUTPUT THE AMORTIZATION TABLE ------------------------

    // ASCII Art thanks to:
    //  https://patorjk.com/software/taag/#p=display&f=Slant&t=Mortgage%20Table
    printf("\n----------------------------------------"
           "----------------------------------------\n");
    printf("|      __  ___           __                            ______      __    __    |\n");
    printf("|     /  |/  /___  _____/ /_____ _____ _____ ____     /_  __/___ _/ /_  / /__  |\n");
    printf("|    / /|_/ / __ \\/ ___/ __/ __ `/ __ `/ __ `/ _ \\     / / / __ `/ __ \\/ / _ \\ |\n");
    printf("|   / /  / / /_/ / /  / /_/ /_/ / /_/ / /_/ /  __/    / / / /_/ / /_/ / /  __/ |\n");
    printf("|  /_/  /_/\\____/_/   \\__/\\__, /\\__,_/\\__, /\\___/    /_/  \\__,_/_.___/_/\\___/  |\n");
    printf("|                        /____/      /____/                                    |\n");
    printf("----------------------------------------"
           "----------------------------------------\n\n");

    // Print the first 5 months and the last 5 months of the amortization table
    printf("  #     Payment   Interest  Principal       Balance\n");
    printf("---  ----------  ---------  ---------  ------------\n");
    for(int i = 0; i < 5; i++)
    {
        printf("%3.d  %10.2f  %9.2f  %9.2f  %12.2f\n",
               i,
               at2[i].payment,
               at2[i].interest,
               at2[i].principal,
               at2[i].balance);
    }
    printf("...         ...        ...        ...           ...\n");
    for(int i = n_payments-4; i < n_payments+1; i++)
    {
        printf("%3.d  %10.2f  %9.2f  %9.2f  %12.2f\n",
               i,
               at2[i].payment,
               at2[i].interest,
               at2[i].principal,
               at2[i].balance);
    }

    free(at2);
    printf("\nMortgage program complete\n\n");
}
