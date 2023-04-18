/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:07:07 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/18 10:08:55 by jalwahei         ###   ########.fr       */
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

void	fork_release(t_philo *philo, int id)
{
	pthread_mutex_unlock(&philo->forks[id]);
	pthread_mutex_unlock(&philo->forks[(id + 1) % philo->num]);
}

void	free_cycle(t_philo_id *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philos->philo->print_m);
	while (i < philos->philo->num)
		pthread_mutex_destroy(&philos->philo->forks[i++]);
	free(philos->philo->forks_flag);
	free(philos->philo->forks);
	free(philos);
}
