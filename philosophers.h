/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 06:05:59 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/16 06:23:49 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_philo
{
	char			*forks_flag;
	pthread_mutex_t	print_m;
	pthread_mutex_t	*forks;
	int				curr_time;
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_must;
	int				still_eating;
	int				died;
	int				mt;
}				t_philo;
typedef struct s_philo_id
{
	t_philo	*philo;
	int		id;
	int		time_stamp;
	int		state;
}				t_philo_id;

void	state_zero(t_philo_id *philo, int *state, int *prevstate, int *food);
void	philo_detach_die(t_philo *p_data, pthread_t *threads, int i);
void	ft_delay(int ms);
void	printing(t_philo_id philo, int msg);
int		num_large(char *str);
size_t	ft_strlen(const char *str);
int		ft_atoi(char const *str, int *status);
void	free_cycle(t_philo_id *philos);
void	fork_release(t_philo *philo, int id);
int		gettime(void);
int		ft_pos2(t_philo_id philo);
int		ft_pos(t_philo_id philo);
int		death_check(t_philo_id philos_id);
#endif