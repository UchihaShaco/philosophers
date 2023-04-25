/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:07:07 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/25 18:09:41 by jalwahei         ###   ########.fr       */
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
	// usleep(1000);
	// pthread_mutex_unlock(&philos->philo->still_eating.lock);
	// pthread_mutex_unlock(&philos->philo->died.lock);
	pthread_mutex_destroy(&philos->philo->print_lock);
	pthread_mutex_destroy(&philos->philo->died.lock);
	pthread_mutex_destroy(&philos->philo->still_eating.lock);

	while (i < philos->philo->num)
		pthread_mutex_destroy(&philos->philo->forks_flag[i++].lock);
	free(philos->philo->forks_flag);
	free(philos);
}
