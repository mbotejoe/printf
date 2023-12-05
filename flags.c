#include "main.h"

/**
 * get_flags - Determines active formatting flags
 * by Joe Mbote
 * @format: Format string indicating how to print the arguments
 * @i: Index of the current character in the format string
 *
 * Return: Calculated flags based on the active formatting options.
 */
int get_flags(const char *format, int *i)
{
    /* The possible flags: - + 0 # ' ' */
    /* Corresponding values:  1 2 4 8  16  */
    int j, current_index;
    int flags = 0;
    const char FLAGS_CHARS[] = {'-', '+', '0', '#', ' ', '\0'};
    const int FLAGS_VALUES[] = {F_MINUS, F_PLUS, F_ZERO, F_HASH, F_SPACE, 0};

    for (current_index = *i + 1; format[current_index] != '\0'; current_index++)
    {
        for (j = 0; FLAGS_CHARS[j] != '\0'; j++)
        {
            if (format[current_index] == FLAGS_CHARS[j])
            {
                flags |= FLAGS_VALUES[j];
                break;
            }
        }

        if (FLAGS_CHARS[j] == 0)
            break;
    }

    *i = current_index - 1;

    return (flags);
}

