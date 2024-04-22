#include "../../include/scop.h"

/**
 * @brief Check if a string is a valid token
 * @param to_check string to check
 * @return 0 if not valid, else the value of the token
*/
u16 is_valid_token(char **valid_tokens, char *to_check)
{
	for (u32 i = 0; valid_tokens[i]; ++i) {
		if (ft_strcmp(to_check, valid_tokens[i]) == 0) {
			// ft_printf_fd(1, ORANGE"Token %s is valid, val %d"RESET, to_check, (1 << i));
			return (1 << i);
		}
	}
	return (0);
}

/**
 * @brief Get string after token
 * @param to_fill_ptr string to fill
 * @param line line to parse
 * @return TRUE if success, else FALSE
*/
s8 get_str_after_token(char **to_fill_ptr, char **line)
{
	if (line[0] == NULL || *line[0] == '\0' || line[1] != NULL) {
		ft_printf_fd(2, RED"Error: Invalid object name\n"RESET);
		display_double_char(line);
		return (FALSE);
	}
	*to_fill_ptr = ft_strdup(line[0]);
	return (TRUE);
}
