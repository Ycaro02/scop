#include "stack_string.h"

int	ft_get_next_line(int fd, t_sstring *line)
{
    static t_sstring	buffer;
    t_sstring           tmp;
    char			    *newline_ptr;
    int			        read_result;
    char			c;

    if (fd < 0 || !line)
        return (-1);
    while (1) {
        read_result = read(fd, &c, 1);
        push_sstring(&buffer, c);

        newline_ptr = ft_strchr(buffer.data, '\n');
        if (newline_ptr) {
            *newline_ptr = '\0';
            clear_sstring(line);
            concat_sstring(line, buffer.data);
            /* save buffer before reset */
            concat_sstring(&tmp, newline_ptr + 1);
            clear_sstring(&buffer);
            concat_sstring(&buffer, tmp.data);
            clear_sstring(&tmp);
            ft_printf_fd(1, "buffer: %s\n", buffer.data);
            ft_printf_fd(1, "Line : %s\n", line->data);
            return (1);
        }

        // read_result = read(fd, &c, 1);
        // push_sstring(&buffer, c);
        // if (c == '\n') {
            // break;
        // }
        if (read_result <= 0) {
            if (read_result < 0) {
                ft_printf_fd(1, ORANGE"EOF\n"RESET);
                return (0);
            }
        }
    }
    return (1);
}