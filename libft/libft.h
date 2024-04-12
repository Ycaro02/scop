/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfour <nfour@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:43:41 by nfour             #+#    #+#             */
/*   Updated: 2024/04/12 16:49:15 by nfour            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
#define LIBFT_H

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "limits.h"
#include <fcntl.h>
#include <errno.h>

#include "basic_define.h"
#include "printf_fd/ft_printf.h"
#include "list/linked_list.h"
#include "math/vec2.h"
#include "math/vec3.h"

#define BUFFER_SIZE 1

#define OUT_OF_UINT32 (uint64_t)(UINT32_MAX + 1)

/* Libft */
char    *get_next_line(int fd); /* Get next line */
char    *ft_strjoin_gnl(char *s1, char *s2);
uint64_t array_to_uint32(const char *nptr);
char	*ft_ultoa(unsigned long n);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_atoi(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *det, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
size_t	ft_strlen(const char *str);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *str, char c);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strdup(const char *s);
char	*ft_itoa(int n);
char	*ft_ltoa(long n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int 	ft_strcpy(char* dst, char *src, int len);	
void 	ft_free_tab(char **tab);
char    *ft_strjoin_free(char *s1, char *s2, char option);
int 	ft_lower_strcmp(char *s1, char *s2);
int 	ft_strcmp(char *s1, char *s2);
void 	free_incomplete_array(void **array, int max);
int8_t	str_is_digit(char *str);
int8_t	str_is_hexa(char *str);
int		ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	free_double_char(char **array);
void	display_double_char(char **array);
u32		double_char_size(char **array);
u8		str_is_float(char *str);
float	ft_atof(char *str);

/* Math */
uint32_t abs_diff(uint32_t a, uint32_t b);
uint32_t max_uint32(uint32_t a, uint32_t b);
uint32_t min_uint32(uint32_t a, uint32_t b);



#endif