
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_util.h"


typedef struct loan_month
{
    float payment;
    float interest;
    float principal;
    float balance;
} LOAN_MONTH;


int main(int argc, char* argv[])
{

    // Parse Command Line Arguments
    // prog.exe <amount> <interest> <months> [extra]

    // Check the number of parameters
    // It's should be safe to say that argv[0] exists because it is the name of this program
    if (argc < 4)
    {
        printf("Error: missing parameters\n");
        printf("Usage: %s <amount> <interest> <months> [extra]\n", argv[0]);
        return EXIT_FAILURE;
    }

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

    // Convert the principal amount to number of cents to make the math easier
    int amount = (int)(famount * 100);

    // Calculate the default monthly payment
    double rate = interest / 100 / 12;
    double pterm = pow(1 + rate, months);
    double fpayment = (amount * rate * pterm) / (pterm - 1) / 100;
    printf("Default Payment: $%.4f\n", fpayment);

    // Create the amortization table
    // Add one extra month to show the original balance
    LOAN_MONTH* atable = (LOAN_MONTH*)malloc((months+1) * sizeof(LOAN_MONTH));
    if (atable == NULL)
    {
        printf("Error: unable to allocate amortization table\n");
        return EXIT_FAILURE;
    }

    atable[0].payment = 0.0;
    atable[0].interest = 0.0;
    atable[0].principal = 0.0;
    atable[0].balance = famount;
    int i = 1;
    while(atable[i-1].balance > fpayment)
    {
        atable[i].payment = fpayment;
        atable[i].interest = atable[i-1].balance * rate;
        atable[i].principal = fpayment - atable[i].interest;
        atable[i].balance = atable[i-1].balance - atable[i].principal;
        i += 1;
    }
    atable[i].interest = atable[i-1].balance * rate;
    atable[i].principal = atable[i-1].balance;
    atable[i].payment = atable[i].interest + atable[i].principal;
    atable[i].balance = 0;
    int n_payments = i;

    LOAN_MONTH* t2 = (LOAN_MONTH*)malloc((n_payments) * sizeof(LOAN_MONTH));
    if (t2 == NULL)
    {
        printf("Error: unable to allocate amortization table\n");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < n_payments; i++)
    {
        t2[i].payment = atable[i].payment;
        t2[i].interest = atable[i].interest;
        t2[i].principal = atable[i].principal;
        t2[i].balance = atable[i].balance;
    }



    // Print the first 5 months and the last 5 months of the amortization table
    printf("  #     Payment   Interest  Principal       Balance\n");
    printf("---  ----------  ---------  ---------  ------------\n");
    for(int i = 0; i < 5; i++)
    {
        printf("%3.d  %10.2f  %9.2f  %9.2f  %12.2f\n",
               i,
               t2[i].payment,
               t2[i].interest,
               t2[i].principal,
               t2[i].balance);
    }
    printf("...         ...        ...        ...           ...\n");
    for(int i = n_payments-4; i < n_payments+1; i++)
    {
        printf("%3.d  %10.2f  %9.2f  %9.2f  %12.2f\n",
               i,
               t2[i].payment,
               t2[i].interest,
               t2[i].principal,
               t2[i].balance);
    }

    free(atable);
}
