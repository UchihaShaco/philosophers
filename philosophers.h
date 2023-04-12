

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
	int current_time;
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int eat_must;
	int *philos;
	int still_eating;
	int died;
	char *forks_flag;
	pthread_mutex_t *forks;


}				t_philo;
typedef struct s_philo_id
{
	int id;
	int time_stamp;
	t_philo *philo;
}				t_philo_id;

int					num_large(char *str);
size_t				ft_strlen(const char *str);
int					ft_atoi(char const *str, int *status);

#endif