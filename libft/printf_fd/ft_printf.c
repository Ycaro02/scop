/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfour <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:57:22 by nfour             #+#    #+#             */
/*   Updated: 2022/10/05 18:55:39 by nfour            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void ft_check_p(unsigned long nbr, int *count, int fd)
{
	if (nbr == 0)
	{
		*count += write(fd, "(nil)", 5);
	}
	else
	{
		*count += write(fd, "0x", 2);
		ft_put_p_nbr(nbr, count, fd);
	}
}

static void ft_put_float_double_nbr(double nbr, int *count, int fd) {
    int		int_part = 0;
	double	decimal_part = (double)0;

	if (nbr < 0) {
        nbr = -nbr;
        *count += ft_putchar_fd('-', fd);
    }

    int_part = (int)nbr;
    decimal_part = nbr - int_part;

    // Affichage de la partie entière
    if (int_part >= 10) {
        ft_put_float_double_nbr(int_part / 10, count, fd);
    }
    *count += ft_putchar_fd(int_part % 10 + '0', fd);

    // Affichage de la virgule si nécessaire
    if (decimal_part > 0 && *count > 0) {
        *count += ft_putchar_fd('.', fd);
    }

    // Affichage de la partie décimale
    if (decimal_part > 0 && *count > 0) {
        for (int i = 0; i < 6; ++i) { // Afficher jusqu'à 6 décimales
            decimal_part *= 10;
            *count += ft_putchar_fd((int)decimal_part + '0', fd);
            decimal_part -= (int)decimal_part;
        }
    }
}
static int ft_continue_display(const char *s, int i, va_list params, int fd)
{
	int nb;

	nb  = 0;
	if (s[i] == 'x')
		ft_put_lx_nbr(va_arg(params, unsigned int), &nb, fd);
	else if (s[i] == 'X')
		ft_put_bigx_nbr(va_arg(params, unsigned int), &nb, fd);
	else if (s[i] == 'f')
		ft_put_float_double_nbr(va_arg(params, double), &nb, fd);
	else if (s[i] == 'p')
		ft_check_p(va_arg(params, unsigned long), &nb, fd);
	else
	{
		nb = write(fd, "%", 1);
		nb += ft_putchar_fd(s[i], fd);
	}
	return (nb);
}

static int ft_display(const char *s, int i, va_list params, int fd)
{
	int	count;

	count = 0;
	i++;
	if (s[i] == 'd')
		count = ft_putlnbr(va_arg(params, int), fd);
	else if (s[i] == 's')
		count = ft_putlstr(va_arg(params, char *), fd);
	else if (s[i] == 'c')
		count = ft_putchar_fd(va_arg(params, int), fd);
	else if (s[i] == 'i')
		count = ft_put_i_nbr(va_arg(params, long long int), fd);
	else if (s[i] == 'u')
		ft_put_lu_nbr(va_arg(params, unsigned long), &count, fd);
	else if (s[i] == '%')
	{
		ft_putchar_fd('%', fd);
		count++;
	}
	else
		count += ft_continue_display(s, i, params, fd);
	return (count);
}

static int ft_print_loop(const char *s, va_list params, int big_len, int fd)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			if (s[i + 1] == '\0')
			{
				big_len += write(fd, "%", 1);
				return (big_len);
			}
			big_len += ft_display(s, i, params, fd);
			i++;
		}
		else
		{
			ft_putchar_fd(s[i], fd);
			big_len++;
		}
		i++;
	}
	return (big_len);
}

int	ft_printf_fd(int fd, const char *s, ...)
{
	size_t	big_len;
	va_list	params;

	big_len = 0;
	va_start(params, s);
	big_len = ft_print_loop(s, params, big_len, fd);
	va_end(params);
	return (big_len);
}