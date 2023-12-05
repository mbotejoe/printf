#include "main.h"

/************************* WRITE HANDLE *************************/
/**
 * handle_write_char - Handles character printing
 * by Joe Mbote G
 * @c: Character to be printed
 * @buffer: Buffer array for print handling
 * @flags: Active formatting flags
 * @width: Width specifier
 * @precision: Precision specifier
 * @size: Size specifier
 *
 * Return: Number of characters printed.
 */
int handle_write_char(char c, char buffer[],
                      int flags, int width, int precision, int size)
{
    /* Character is stored at the left, and padding is applied to the right of the buffer */
    int i = 0;
    char padding_char = ' ';

    UNUSED(precision);
    UNUSED(size);

    if (flags & F_ZERO)
        padding_char = '0';

    buffer[i++] = c;
    buffer[i] = '\0';

    if (width > 1)
    {
        buffer[BUFF_SIZE - 1] = '\0';

        for (i = 0; i < width - 1; i++)
            buffer[BUFF_SIZE - i - 2] = padding_char;

        if (flags & F_MINUS)
            return write(1, &buffer[0], 1) + write(1, &buffer[BUFF_SIZE - i - 1], width - 1);
        else
            return write(1, &buffer[BUFF_SIZE - i - 1], width - 1) + write(1, &buffer[0], 1);
    }

    return write(1, &buffer[0], 1);
}

/************************* WRITE NUMBER *************************/
/**
 * write_number - Handles the printing of numbers
 * @is_negative: Flag indicating if the number is negative
 * @ind: Index where the number starts in the buffer
 * @buffer: Buffer array for print handling
 * @flags: Active formatting flags
 * @width: Width specifier
 * @precision: Precision specifier
 * @size: Size specifier
 *
 * Return: Number of characters printed.
 */
int write_number(int is_negative, int ind, char buffer[],
                 int flags, int width, int precision, int size)
{
    int length = BUFF_SIZE - ind - 1;
    char padding_char = ' ', extra_char = 0;

    UNUSED(size);

    if ((flags & F_ZERO) && !(flags & F_MINUS))
        padding_char = '0';

    if (is_negative)
        extra_char = '-';
    else if (flags & F_PLUS)
        extra_char = '+';
    else if (flags & F_SPACE)
        extra_char = ' ';

    return write_num(ind, buffer, flags, width, precision,
                     length, padding_char, extra_char);
}

/**
 * write_num - Writes a number using a buffer
 * @ind: Index where the number starts in the buffer
 * @buffer: Buffer array
 * @flags: Active formatting flags
 * @width: Width specifier
 * @prec: Precision specifier
 * @length: Number length
 * @padd: Padding character
 * @extra_c: Extra character
 *
 * Return: Number of printed characters.
 */
int write_num(int ind, char buffer[],
              int flags, int width, int prec,
              int length, char padd, char extra_c)
{
    int i, padd_start = 1;

    if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0' && width == 0)
        return 0; /* printf(".0d", 0) - no characters are printed */

    if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
    {
        buffer[ind] = padd = ' '; /* Width is displayed with padding ' ' */
    }

    if (prec > 0 && prec < length)
        padd = ' ';

    while (prec > length)
    {
        buffer[--ind] = '0';
        length++;
    }

    if (extra_c != 0)
        length++;

    if (width > length)
    {
        for (i = 1; i < width - length + 1; i++)
            buffer[i] = padd;

        buffer[i] = '\0';

        if (flags & F_MINUS && padd == ' ') /* Assign extra char to the left of the buffer */
        {
            if (extra_c)
                buffer[--ind] = extra_c;

            return write(1, &buffer[ind], length) + write(1, &buffer[1], i - 1);
        }
        else if (!(flags & F_MINUS) && padd == ' ') /* Extra char to the left of the buffer */
        {
            if (extra_c)
                buffer[--ind] = extra_c;

            return write(1, &buffer[1], i - 1) + write(1, &buffer[ind], length);
        }
        else if (!(flags & F_MINUS) && padd == '0') /* Extra char to the left of padding */
        {
            if (extra_c)
                buffer[--padd_start] = extra_c;

            return write(1, &buffer[padd_start], i - padd_start) +
                   write(1, &buffer[ind], length - (1 - padd_start));
        }
    }

    if (extra_c)
        buffer[--ind] = extra_c;

    return write(1, &buffer[ind], length);
}

/**
 * write_unsgnd - Writes an unsigned number
 * @is_negative: Flag indicating if the number is negative
 * @ind: Index where the number starts in the buffer
 * @buffer: Array of characters
 * @flags: Formatting flags specifier
 * @width: Width specifier
 * @precision: Precision specifier
 * @size: Size specifier
 *
 * Return: Number of written characters.
 */
int write_unsgnd(int is_negative, int ind,
                 char buffer[],
                 int flags, int width, int precision, int size)
{
    /* The number is stored at the buffer's right and starts at position i */
    int length = BUFF_SIZE - ind - 1, i = 0;
    char padd = ' ';

    UNUSED(is_negative);
    UNUSED(size);

    if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
        return 0; /* printf(".0d", 0) - no characters are printed */

    if (precision > 0 && precision < length)
        padd = ' ';

    while (precision > length)
    {
        buffer[--ind] = '0';
        length++;
    }

    if ((flags & F_ZERO) && !(flags & F_MINUS))
        padd = '0';

    if (width > length)
    {
        for (i = 0; i < width - length; i++)
            buffer[i] = padd;

        buffer[i] = '\0';

        if (flags & F_MINUS) /* Assign extra char to the left of the buffer [buffer>padd] */
        {
            return write(1, &buffer[ind], length) + write(1, &buffer[0], i);
        }
        else /* Assign extra char to the left of padding [padd>buffer] */
        {
            return write(1, &buffer[0], i) + write(1, &buffer[ind], length);
        }
    }

    return write(1, &buffer[ind], length);
}

/**
 * write_pointer - Writes a memory address
 * @buffer: Array of characters
 * @ind: Index where the number starts in the buffer
 * @length: Length of the number
 * @width: Width specifier
 * @flags: Formatting flags specifier
 * @padd: Padding character
 * @extra_c: Extra character
 * @padd_start: Index where padding should start
 *
 * Return: Number of written characters.
 */
int write_pointer(char buffer[], int ind, int length,
                  int width, int flags, char padd, char extra_c, int padd_start)
{
    int i;

    if (width > length)
    {
        for (i = 3; i < width - length + 3; i++)
            buffer[i] = padd;

        buffer[i] = '\0';

        if (flags & F_MINUS && padd == ' ') /* Assign extra char to the left of buffer */
        {
            buffer[--ind] = 'x';
            buffer[--ind] = '0';

            if (extra_c)
                buffer[--ind] = extra_c;

            return write(1, &buffer[ind], length) + write(1, &buffer[3], i - 3);
        }
        else if (!(flags & F_MINUS) && padd == ' ') /* Extra char to the left of buffer */
        {
            buffer[--ind] = 'x';
            buffer[--ind] = '0';

            if (extra_c)
                buffer[--ind] = extra_c;

            return write(1, &buffer[3], i - 3) + write(1, &buffer[ind], length);
        }
        else if (!(flags & F_MINUS) && padd == '0') /* Extra char to the left of padding */
        {
            if (extra_c)
                buffer[--padd_start] = extra_c;

            buffer[1] = '0';
            buffer[2] = 'x';

            return write(1, &buffer[padd_start], i - padd_start) +
                   write(1, &buffer[ind], length - (1 - padd_start) - 2);
        }
    }

    buffer[--ind] = 'x';
    buffer[--ind] = '0';

    if (extra_c)
        buffer[--ind] = extra_c;

    return write(1, &buffer[ind], BUFF_SIZE - ind - 1);
}

