#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

/**
 * Print a usage message and exit
 */
void usageExit(void)
{
    fprintf(stderr, "usage: calc [expression]\n");
    exit(1);
}

int main(int argc, char **argv)
{

    // fractions
    mpf_set_default_prec(100); // precision (max number of decimal-points)

    mpf_t result, a, b;
    mpf_inits(result, a, b, NULL);
    mpf_set_str(a, "8", 10); // string
    mpf_set_ui(b, 3); // integer
    mpf_add(result, a, b); // addition
    mpf_out_str(stdout, 10, 0, result); // print
    // mpf_out_str(stdout, 10, num_of_decimal_points, result);
    puts("");
    mpf_mul(result, a, b); // multiplication
    mpf_out_str(stdout, 10, 0, result);
    puts("");
    mpf_div(result, a, b); // division (a/b)
    mpf_out_str(stdout, 10, 0, result);
    puts("");

    mpf_clear(result);
    mpf_clear(a);
    mpf_clear(b);

    // decimals
    mpz_t result2, a2, b2;
    mpz_inits(result2, a2, b2, NULL);
    mpz_set_str(a2, "-8", 10); // get from string
    mpz_set_ui(b2, 3); // get from integer

    printf("a = ");
    mpz_out_str(stdout, 10, a2);
    printf("\nb = ");
    mpz_out_str(stdout, 10, b2);


    mpz_add(result2, a2, b2); // addition (a+b)
    printf("\na+b = ");
    mpz_out_str(stdout, 10, result2); // print
    printf("\na-b = ");
    mpz_sub(result2, a2, b2); // substitution (a-b)
    mpz_out_str(stdout, 10, result2);
    printf("\na*b = ");
    mpz_mul(result2, a2, b2); // multiplication (a*b)
    mpz_out_str(stdout, 10, result2);
    printf("\na/b = ");
    mpz_div(result2, a2, b2); // division (a/b)
    mpz_out_str(stdout, 10, result2); // missing the remainder
    printf("\na^3 = "); //
    mpz_pow_ui(result2, a2, 3); // power (a^3)
    mpz_out_str(stdout, 10, result2);
    printf("\n9! = ");
    mpz_fac_ui(result2, 9); // factorial (9!)
    mpz_out_str(stdout, 10, result2);
    puts("");
    // mpz_ui_pow_ui(result2, 6, 2); // power (6^2)
    // mpz_out_str(stdout, 10, result2);
    // puts("");

    mpz_clear(result2);
    mpz_clear(a2);
    mpz_clear(b2);

    return 0;
}
