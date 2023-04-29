/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:10:07 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 20:21:33 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	death_check(t_philo_id philos_id)
{
	int				curr_time;

	curr_time = gettime();
	if (curr_time - philos_id.time_stamp >= philos_id.philo->time_to_die)
	{
		th_set(&philos_id.philo->died, philos_id.id + 1, 0);
		return (1);
	}
	return (0);
}

void	philo_detach_die(t_philo *p_data, pthread_t *threads, int i)
{
	while (th_get(&p_data->still_eating, 1) > 0)
	{
		if (th_get(&p_data->died, 1) > 0)
		{
			pthread_mutex_lock(&p_data->print_lock);
			printf("%d %d died\n", gettime() - p_data->mt, \
			th_get(&p_data->died, 1));
			pthread_mutex_unlock(&p_data->print_lock);
			break ;
		}
	}
	while (i > 0)
	{
		pthread_join(threads[i - 1], NULL);
		i--;
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

	pthread_mutex_lock(&philo.philo->print_lock);
	curr_time = gettime();
	if (msg == 1)
		printf("%d %d has taken a fork\n", curr_time - \
		philo.philo->mt, philo.id + 1);
	else if (msg == 2)
		printf("%d %d is eating\n", curr_time - philo.philo->mt, philo.id + 1);
	else if (msg == 3)
		printf("%d %d is sleeping\n", curr_time - philo.philo->mt, philo.id + 1);
	else if (msg == 4)
		printf("%d %d is thinking\n", curr_time - philo.philo->mt, philo.id + 1);
	pthread_mutex_unlock(&philo.philo->print_lock);
}

int	init_philo(t_philo *philo, char **argv)
{
	int	status;
	int	i;

	i = 0;
	philo->num = ft_atoi(argv[1], &status);
	philo->time_to_die = ft_atoi(argv[2], &status);
	philo->time_to_eat = ft_atoi(argv[3], &status);
	philo->time_to_sleep = ft_atoi(argv[4], &status);
	philo->eat_must = -1;
	if (argv[5])
		philo->eat_must = ft_atoi(argv[5], &status);
	philo->forks_flag = malloc(sizeof(t_locks) * philo->num);
	pthread_mutex_init(&philo->died.lock, NULL);
	pthread_mutex_init(&philo->still_eating.lock, NULL);
	philo->still_eating.val = philo->num;
	philo->died.val = 0;
	while (i < philo->num)
	{
		pthread_mutex_init(&philo->forks_flag[i].lock, NULL);
		philo->forks_flag[i++].val = 1;
	}
	pthread_mutex_init(&philo->print_lock, NULL);
	return (0);
}
