/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:07:07 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 20:37:41 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_pos(t_philo_id philo)
{
	return ((philo.id + philo.id % 2) % philo.philo->num);
}

int	ft_pos2(t_philo_id philo)
{
	if (philo.id % 2 == 0)
		return ((philo.id + 1) % philo.philo->num);
	return (philo.id);
}

int	gettime(void)
{
	struct timeval	tms;
	int				curr_time;

	gettimeofday(&tms, NULL);
	curr_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
	return (curr_time);
}

void	free_cycle(t_philo_id *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philos->philo->print_lock);
	pthread_mutex_destroy(&philos->philo->died.lock);
	pthread_mutex_destroy(&philos->philo->still_eating.lock);
	while (i < philos->philo->num)
		pthread_mutex_destroy(&philos->philo->forks_flag[i++].lock);
	free(philos->philo->forks_flag);
	free(philos);
}

void	state_one_two(t_philo_id *philo, int *state, int *prevstate, int flag)
{
	if (flag == 1)
	{
		if (philo->philo->num % 2 == 1)
		{
			if (philo->philo->time_to_die < philo->philo->time_to_sleep + \
				philo->philo->time_to_eat * 2)
				ft_delay(philo->philo->time_to_die - philo->philo->time_to_eat \
			- philo->philo->time_to_sleep);
			else
				ft_delay(philo->philo->time_to_eat);
		}
		printing(*philo, 4);
		*state = 0;
	}
	else if (flag == 2)
	{
		printing(*philo, 3);
		if (philo->philo->time_to_die < philo->philo->time_to_sleep
			+ philo->philo->time_to_eat)
			ft_delay(philo->philo->time_to_die - philo->philo->time_to_eat);
		else
			ft_delay(philo->philo->time_to_sleep);
		*prevstate = *state;
		*state = 1;
	}
}
