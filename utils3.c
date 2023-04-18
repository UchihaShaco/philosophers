/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:10:07 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/18 10:07:54 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	death_check(t_philo_id philos_id)
{
	int				curr_time;

	curr_time = gettime();
	if (curr_time - philos_id.time_stamp > philos_id.philo->time_to_die)
	{
		philos_id.philo->died = philos_id.id + 1;
		return (1);
	}
	return (0);
}

void	philo_detach_die(t_philo *p_data, pthread_t *threads, int i)
{
	while (p_data->still_eating > 0)
	{
		if (p_data->died > 0)
		{
			while (i > 0)
			{
				pthread_join(threads[i - 1], NULL);
				i--;
			}
			while (i > 0)
			{
				pthread_detach(threads[i - 1]);
				i--;
			}
			p_data->curr_time = gettime();
			printf("%d %d died\n", p_data->curr_time - p_data->mt, p_data->died);
			break ;
		}
	}
}

void	ft_delay(int ms)
{
	struct timeval	tms;
	int				current_time;
	int				start_time;

	ms = ms - 1;
	gettimeofday(&tms, NULL);
	current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000 ;
	start_time = current_time;
	while (ms >= current_time - start_time)
	{
		gettimeofday(&tms, NULL);
		current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000 ;
	}
}

void	printing(t_philo_id philo, int msg)
{
	int				curr_time;

	curr_time = gettime();
	pthread_mutex_lock(&philo.philo->print_m);
	if (msg == 1)
		printf("%d %d has taken a fork\n", curr_time - \
		philo.philo->mt, philo.id + 1);
	else if (msg == 2)
		printf("%d %d is eating\n", curr_time - philo.philo->mt, philo.id + 1);
	else if (msg == 3)
		printf("%d %d is sleeping\n", curr_time - philo.philo->mt, philo.id + 1);
	else if (msg == 4)
		printf("%d %d is thinking\n", curr_time - philo.philo->mt, philo.id + 1);
	pthread_mutex_unlock(&philo.philo->print_m);
}
