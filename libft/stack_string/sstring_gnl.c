#include "stack_string.h"

int	ft_get_next_line(int fd, t_sstring *line)
{
    int     read_result;
    char    c;

    empty_sstring(line);
    if (fd < 0 || !line)
        return (-1);
    while (1) {
        read_result = read(fd, &c, 1);
        push_sstring(line, c);
        if (c == '\n') {
            // ft_printf_fd(1, "Line : %s %u\n", line->data, line->size);
            return (1);
        }
        if (read_result <= 0) {
            ft_printf_fd(1, ORANGE"EOF\n"RESET);
            return (0);
        }
    }
    return (1);
}