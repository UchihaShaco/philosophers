/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:05:44 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/16 06:14:30 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	overflowcheck(long long val, int sign, char c)
{
	if (sign == 1)
	{
		if (val > __LONG_MAX__ / 10 || (val == __LONG_MAX__ / 10 \
		&& c - '0' > __LONG_MAX__ % 10))
			return (-1);
	}
	else
	{
		if (-val < -(__LONG_MAX__ - 1) / 10 || (-val == -(__LONG_MAX__ - 1) \
		/ 10 && c - '0' > __LONG_MAX__ % 10 + 1))
			return (0);
	}
	return (1);
}

int	ft_atoi(char const *str, int *status)
{
	long long	val;
	int			sign;
	int			i;

	val = 0;
	sign = 1;
	i = 0;
	if (status)
		*status = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if ((str[i] == '-' || str[i] == '+') && (str[i++] == '-'))
		sign *= -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (overflowcheck(val, sign, str[i]) != 1)
		{
			if (status)
				*status = 0;
			return (overflowcheck(val, sign, str[i]));
		}
		val *= 10;
		val += str[i++] - '0';
	}
	return ((int)val * sign);
}

int	ft_isstrdigit(char *str)
{
	if (*str && (*str == '+' || *str == '-'))
		str++;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	num_large(char *str)
{
	int	status;
	int	val;

	if (ft_isstrdigit(str))
	{
		val = ft_atoi(str, &status);
		if (!status)
			return (1);
		else
		{
			if (val < 0)
				return (1);
			return (0);
		}
	}
	return (1);
}

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (*str != '\0')
	{
		str++;
		count++;
	}
	return (count);
}