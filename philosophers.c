/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:05:44 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/18 10:46:00 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_handle(t_philo_id philo)
{
	if (philo.philo->forks_flag[ft_pos2(philo)] == 0 ||	philo.philo->forks_flag[ft_pos(philo)] == 0)
		return (0);
	pthread_mutex_lock(&philo.philo->forks[ft_pos(philo)]);
	philo.philo->forks_flag[ft_pos(philo)] = 0;
	printing(philo, 1);
	if (philo.philo->forks_flag[ft_pos2(philo)] == 0)
	{
		philo.philo->forks_flag[ft_pos(philo)] = 1;
		pthread_mutex_unlock(&philo.philo->forks[ft_pos(philo)]);
		return (1);
	}
	pthread_mutex_lock(&philo.philo->forks[ft_pos2(philo)]);
	philo.philo->forks_flag[(philo.id + 1) % philo.philo->num] = 0;
	printing(philo, 1);
	return (2);
}

int	init_philo(t_philo *philo, char **argv)
{
	int	status;
	int	i;

	i = 0;
	philo->num = ft_atoi(argv[1], &status);
	// if (phil)
	philo->time_to_die = ft_atoi(argv[2], &status);
	philo->time_to_eat = ft_atoi(argv[3], &status);
	philo->time_to_sleep = ft_atoi(argv[4], &status);
	philo->eat_must = -1;
	if (argv[5])
		philo->eat_must = ft_atoi(argv[5], &status);
	philo->forks = malloc(sizeof(pthread_mutex_t) * philo->num);
	philo->forks_flag = malloc(sizeof(char) * philo->num);
	philo->died = 0;
	philo->still_eating = philo->num;
	while (i < philo->num)
		pthread_mutex_init(&philo->forks[i++], NULL);
		// i++;
	pthread_mutex_init(&philo->print_m, NULL);
	memset(philo->forks_flag, 1, philo->num);
	return (0);
}

void	state_zero(t_philo_id *philo, int *state, int *prevstate, int *food)
{
	philo->time_stamp = gettime();
	printing(*philo, 2);
	if (philo->philo->time_to_die < philo->philo->time_to_eat)
		ft_delay(philo->philo->time_to_die);
	else
		ft_delay(philo->philo->time_to_eat);
	philo->philo->forks_flag[philo->id] = 1;
	philo->philo->forks_flag[(philo->id + 1) % philo->philo->num] = 1;
	fork_release(philo->philo, philo->id);
	(*food)++;
	*prevstate = *state;
	*state = 2;
}

void	state_two(t_philo_id *philo, int *state, int *prevstate)
{
	printing(*philo, 3);
	if (philo->philo->time_to_die < philo->philo->time_to_sleep\
	+ philo->philo->time_to_eat)
		ft_delay(philo->philo->time_to_die - philo->philo->time_to_eat);
	else
		ft_delay(philo->philo->time_to_sleep);
	*prevstate = *state;
	*state = 1;
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
	while (food != philo->philo->eat_must && philo->philo->died == 0 \
	&& !death_check(*philo))
	{
		if (state == 0)
		{
			state = fork_handle(*philo);
			if (state != 2)
			continue ;
			state_zero(philo, &state, &prevstate, &food);
		}
		else if (state == 1)
		{
			printing(*philo, 4);
			state = 0;
		}
		else if (state == 2)
			state_two(philo, &state, &prevstate);
	}
	return (philo->philo->still_eating--, NULL);
}

void philos_start(t_philo *p_data)
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
		i++;
	}
	i = 0;
	while (i < p_data->num)
	{
		pthread_create(&threads[i], NULL, &p_life, &philos_id[i]);
		i++;
	}
	philo_detach_die(p_data, threads, i);
	free_cycle(philos_id);
	free(threads);

}

int main(int argc, char **argv)
{
	t_philo philo;
	(void) argc;
	philo.mt = gettime();

	int status;
	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments\n"));
	if (num_large(argv[1]) || num_large(argv[2]) || num_large(argv[3]) 
		|| num_large(argv[4]) || (argc == 6 && num_large(argv[5])) || ft_atoi(argv[1], &status) > 200)
		return (printf("Error: invalid parameters\n"));
	// if (ft_atoi(argv[1], &status) == 1) // currently hardcode for 1 philo
	// 	return (printf("1 is died\n"));
	if (init_philo(&philo, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	philos_start(&philo);
	return (0);
}
