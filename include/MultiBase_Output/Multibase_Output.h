#ifndef _MULTIBASE_OUTPUT_H_
#define _MULTIBASE_OUTPUT_H_

// 2 <= base <= 10 OR base == 16
void displayInBase(int n, int base)
{
    if(n > 0)
    {
        // output the digits in (n / base)
        displayInBase(n / base, base);

        // output the remainder
        int rem = n % base;
        if(rem < 10)
            std::cout << rem;
        else
            std::cout << (char)(65 + rem - 10);
    }
}

#endif