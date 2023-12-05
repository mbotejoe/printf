#include "main.h"

/**
 * get_precision - Determines the precision for printing
 * by mbote joe
 * @format: Formatted string for printing arguments
 * @i: Index of the argument list
 * @list: Argument list
 *
 * Return: Precision value.
 */
int get_precision(const char *format, int *i, va_list list)
{
    int curr_i = *i + 1;
    int precision = -1;

    /* Check for the presence of precision specifier */
    if (format[curr_i] != '.')
        return precision;

    precision = 0;

    /* Extract precision value from the format string */
    for (curr_i += 1; format[curr_i] != '\0'; curr_i++)
    {
        if (is_digit(format[curr_i]))
        {
            precision *= 10;
            precision += format[curr_i] - '0';
        }
        else if (format[curr_i] == '*')
        {
            curr_i++;
            precision = va_arg(list, int);
            break;
        }
        else
        {
            break;
        }
    }

    *i = curr_i - 1;

    return precision;
}

