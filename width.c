#include "main.h"

/**
 * get_width - Determines the width for formatting printed output
 * by Joe Mbote
 * @format: Format string specifying how to print the arguments
 * @i: Index of the current character in the format string
 * @list: Variable arguments list
 *
 * Return: Calculated width based on the specified format.
 */
int get_width(const char *format, int *i, va_list list)
{
    int current_index;
    int width = 0;

    for (current_index = *i + 1; format[current_index] != '\0'; current_index++)
    {
        if (is_digit(format[current_index]))
        {
            /* Accumulate digits to determine the width */
            width *= 10;
            width += format[current_index] - '0';
        }
        else if (format[current_index] == '*')
        {
            /* Handle '*' to fetch width from variable arguments */
            current_index++;
            width = va_arg(list, int);
            break;
        }
        else
        {
            /* Exit loop if the character is neither a digit nor '*' */
            break;
        }
    }

    *i = current_index - 1;

    return (width);
}

