/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:05:44 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 20:36:18 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_handle(t_philo_id *philo)
{
	if (philo->has_one_fork == 0)
	{
		if (th_get(&philo->philo->forks_flag[ft_pos(*philo)], 0) == 0)
		{
			pthread_mutex_unlock(&philo->philo->forks_flag \
		[ft_pos(*philo)].lock);
			return (0);
		}
		th_set(&philo->philo->forks_flag[ft_pos(*philo)], 0, 1);
		philo->has_one_fork = 1;
	}
	if (th_get(&philo->philo->forks_flag[ft_pos2(*philo)], 0) == 0)
	{
		pthread_mutex_unlock(&philo->philo->forks_flag[ft_pos2(*philo)].lock);
		return (0);
	}
	th_set(&philo->philo->forks_flag[ft_pos2(*philo)], 0, 1);
	philo->has_one_fork = 0;
	return (2);
}

void	state_zero(t_philo_id *philo, int *state, int *prevstate, int *food)
{
	philo->time_stamp = gettime();
	printing(*philo, 2);
	if (philo->philo->time_to_die < philo->philo->time_to_eat)
		ft_delay(philo->philo->time_to_die);
	else
		ft_delay(philo->philo->time_to_eat);
	th_set(&philo->philo->forks_flag[ft_pos(*philo)], 1, 0);
	th_set(&philo->philo->forks_flag[ft_pos2(*philo)], 1, 0);
	(*food)++;
	*prevstate = *state;
	*state = 2;
}

void	*p_life(void *data)
{
	t_philo_id	*philo;
	int			state;
	int			prevstate;
	int			food;

	food = 0;
	state = 0;
	philo = (t_philo_id *)data;
	philo->time_stamp = gettime();
	while (food != philo->philo->eat_must && th_get(&philo->philo->died, 1) == 0
		&& !death_check(*philo))
	{
		if (state == 0)
		{
			state = fork_handle(philo);
			if (state != 2)
				continue ;
			state_zero(philo, &state, &prevstate, &food);
		}
		else
			state_one_two(philo, &state, &prevstate, state);
	}
	th_set(&philo->philo->still_eating, \
	th_get(&philo->philo->still_eating, 0) - 1, 1);
	return (NULL);
}

void	philos_start(t_philo *p_data)
{
	t_philo_id	*philos_id;
	pthread_t	*threads;
	int			i;

	philos_id = malloc(sizeof(t_philo_id) * p_data->num);
	threads = malloc(sizeof(pthread_t) * p_data->num);
	i = 0;
	while (i < p_data->num)
	{
		philos_id[i].id = i;
		philos_id[i].philo = p_data;
		philos_id[i].time_stamp = -1;
		philos_id[i].has_one_fork = 0;
		i++;
	}
	i = 0;
	while (i < p_data->num)
	{
		pthread_create(&threads[i], NULL, &p_life, &philos_id[i]);
		i++;
	}
	philo_detach_die(p_data, threads, i);
	free(threads);
	free_cycle(philos_id);
}

int	main(int argc, char **argv)
{
	t_philo	philo;
	int		status;

	(void) argc;
	philo.mt = gettime();
	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments\n"));
	if (num_large(argv[1]) || num_large(argv[2]) || num_large(argv[3])
		|| num_large(argv[4]) || (argc == 6 && num_large(argv[5])) \
		|| ft_atoi(argv[1], &status) > 200)
		return (printf("Error: invalid parameters\n"));
	if (ft_atoi(argv[1], &status) == 1)
		return (printf("1 is died\n"));
	if (init_philo(&philo, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	philos_start(&philo);
	return (0);
}
