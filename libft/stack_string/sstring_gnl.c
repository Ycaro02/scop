#include "stack_string.h"

int	ft_get_next_line(int fd, t_sstring *line)
{
    int     read_result = 1;
    char    c;

    empty_sstring(line);
    if (fd < 0 || !line)
        return (-1);
    while (read_result > 0) {
        read_result = read(fd, &c, 1);
        push_sstring(line, c);
		// ft_printf_fd(1, "Line : %s %u, res %d, char %d\n", line->data, line->size, read_result, c);
        if (read_result <= 0) {
            ft_printf_fd(1, ORANGE"EOF\n"RESET);
            return (0);
        }
        if (c == '\n') {
            return (1);
        }
    }
    return (0);
}

void sstring_rm_first_word(t_sstring *sstr, u32 end)
{
	t_sstring tmp;

	empty_sstring(&tmp);

	if ((sstr->size <= end - 1)) {
		clear_sstring(sstr);
		return;
	}

	tmp = fill_sstring(sstr->data + end + 1);
	clear_sstring(sstr);
	ft_printf_fd(1, "tmp: %s\n", tmp.data);
	*sstr = fill_sstring(tmp.data);

}

u32 sstring_search(t_sstring *sstr, char c)
{
	if (!sstr)
		return (0);
	for (uint16_t i = 0; i < sstr->size; ++i) {
		if (sstr->data[i] == c)
			return (i);
	}
	return (OUT_OF_SSTRING);
}

u8 sstring_read(t_sstring *sstr, int fd)
{
	int     read_result = 1;

	empty_sstring(sstr);
	if (fd < 0 || !sstr)
		return (0);
	read_result = read(fd, sstr->data, SSTRING_MAX - 1);
	if (read_result <= 0) {
		return (0);
	}
	return (1);
}

char **sstring_load_file(char *path)
{
	t_sstring	line;
	u8			ret_read = 1;
	char		*map = NULL, **dest = NULL;
	int 		fd = -1;	
	
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		ft_printf_fd(2, "Error\nfd incorect \n");
		return (NULL);
	}
	clear_sstring(&line);
	map = ft_strdup("");
	while (ret_read) {
		ret_read = sstring_read(&line, fd);
		map = ft_strjoin_free(map, line.data, 'f');
		clear_sstring(&line);	
		if (ret_read == 0){
			break ;
		}
		// ft_printf_fd(1, ORANGE"map|%s|"RESET,map);
	}

	ft_printf_fd(1, CYAN"File %s:"RESET"\n"PINK"|%s|"RESET,path,map);
	close(fd);
	dest = ft_split_trim(map, '\n');
	free(map);
	return (dest);
}