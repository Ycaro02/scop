#include "../../include/scop.h"

/**
 * @brief Load file
 * @param path path to file
 * @return allocated char **, NULL if fail
*/
char	**load_file(char *path)
{
	t_sstring line;
	char	*map;
	int		fd;
	char	**dest;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_printf_fd(2, "Error\nfd incorect \n");
		return (NULL);
	}

	clear_sstring(&line);
	map = ft_strdup("");
	while (1) {
		// line = get_next_line(fd);
		int ret = ft_get_next_line(fd, &line);
		map = ft_strjoin_free(map, line.data, 'f');
		clear_sstring(&line);
		if (ret == 0){
			break ;
		}
	}

	ft_printf_fd(1, "map:\n%s",map);
	// free(line);
	close(fd);
	dest = ft_split_trim(map, '\n');
	free(map);
	return (dest);
}

