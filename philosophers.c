
#include "philosophers.h"

int ft_pos(int num)
{
	return (num + num % 2);
}
int ft_pos2(int num)
{
	if (num % 2 == 0)
		return (num + 1);
	return (num);
}
int ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}
void fork_release(t_philo *philo, int id)
{
	pthread_mutex_unlock(&philo->forks[id]);
	pthread_mutex_unlock(&philo->forks[(id + 1) % philo->num]);
}
int fork_handle(t_philo *philo, int id)
{

			struct timeval tms;
	if (philo->forks_flag[id] == 0 && philo->forks_flag[(id + 1) % philo->num] == 0)
		return (0);
	// if (id % 2 == 0)
	// {
	pthread_mutex_lock(&philo->forks[id]);
	philo->forks_flag[id] = 0;

			gettimeofday(&tms, NULL);
			int current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
	printf("\033[0;31m %d %d has taken a fork\n\033", current_time, id);
	if (philo->forks_flag[(id + 1) % philo->num] == 0)
	{
		philo->forks_flag[id] = 1;
		pthread_mutex_unlock(&philo->forks[id]);
		return 0;
	}

	pthread_mutex_lock(&philo->forks[(id + 1) % philo->num]);
	philo->forks_flag[(id + 1) % philo->num] = 0;
			gettimeofday(&tms, NULL);
			current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
	printf("\033[0;31m %d %d has taken a fork\n\033", current_time, id);
	fork_release(philo, id);
	return (1);
	// }
	// else
	// {
	// 	pthread_mutex_lock(&philo->forks[(id + 1) % philo->num]);
	// 	pthread_mutex_lock(&philo->forks[id]);
	// }
	fork_release(philo, id);
}

int init_philo(t_philo *philo, char **argv)
{
	int status;
	int i = 0;
	philo->num = ft_atoi(argv[1], &status);
	if (philo->num == 0)
		return (ft_error("Error: No philosphers\n"));

	philo->time_to_die = ft_atoi(argv[2], &status);
	philo->time_to_eat = ft_atoi(argv[3], &status);
	philo->time_to_sleep = ft_atoi(argv[4], &status);
	philo->eat_must = -1;
	if (argv[5])
		philo->eat_must = ft_atoi(argv[5], &status);
	philo->philos = malloc(sizeof(int) * philo->num);
	if (!philo->philos)
		return (ft_error("Error: malloc failed\n"));
	philo->forks = malloc(sizeof(pthread_mutex_t) * philo->num);
	philo->forks_flag = malloc(sizeof(char) * philo->num);
	philo->died = 0;
	philo->still_eating = philo->num;
	if (!philo->forks)
		return (ft_error("Error: malloc failed\n"));
	while (i < philo->num)
	{
		pthread_mutex_init(&philo->forks[i], NULL);
		i++;
	}
	memset(philo->forks_flag, 1, philo->num);
	return (0);
}
int death_check(t_philo_id philos_id)
{

	struct timeval tms;
	gettimeofday(&tms, NULL);
	int current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
	if (philos_id.time_stamp != -1 && current_time - philos_id.time_stamp >= philos_id.philo->time_to_die +5)
	{
		philos_id.philo->died = philos_id.id;
		return (1);
	}
	return (0);
}
void gettime_p(t_philo *philo)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	philo->current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
}
void *p_life(void *data)
{
	t_philo_id *philo;
	int state;
	int prevstate;
	int food;
	food = 0;
	state = 0;
	philo = (t_philo_id *)data;
	struct timeval tms;
	gettimeofday(&tms, NULL);
	int current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
	philo->time_stamp = philo->philo->current_time;
	while (food != philo->philo->eat_must && philo->philo->died == 0 && !death_check(*philo))
	{
		// gettime_p(philo->philo);
		if (state == 0)
		{
			if (!fork_handle(philo->philo, philo->id))
			{
				state = 1;
				continue;
			}
			// philo->philo->forks_flag[(philo->id + 1) % philo->philo->num] = 0;

			gettimeofday(&tms, NULL);
			current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
			philo->time_stamp = current_time;
			printf("\033[0;32m %d %d is eating\n\033", philo->time_stamp, philo->id);
			if (philo->philo->time_to_die < philo->philo->time_to_eat)
				usleep(philo->philo->time_to_die * 1000);
			else
				usleep(philo->philo->time_to_eat * 1000);
			philo->philo->forks_flag[philo->id] = 1;
			// pthread_mutex_unlock(&philo->philo->forks[philo->id]);
			// pthread_mutex_unlock(&philo->philo->forks[(philo->id + 1) % philo->philo->num]);
			philo->philo->forks_flag[(philo->id + 1) % philo->philo->num] = 1;
			food++;
			prevstate = state;
			state = 2;
		}
		else if (state == 1)
		{
			if (prevstate != 1)
			{
			gettimeofday(&tms, NULL);
			current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
			printf("\033[0;34m %d %d is thinking\n\033", current_time, philo->id);
			// usleep(5);
			}
			prevstate = state;
			state = 0;
		}
		else if (state == 2)
		{
			gettimeofday(&tms, NULL);
			current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
			printf("\033[0;33m %d %d is sleeping\n", current_time, philo->id);
			if (philo->philo->time_to_die < philo->philo->time_to_sleep + philo->philo->time_to_eat)
				usleep((philo->philo->time_to_die - philo->philo->time_to_eat) * 1000);
			else
				usleep(philo->philo->time_to_sleep * 1000);
			prevstate = state;
			state = 1;
		}
	}

	philo->philo->still_eating--;
	return (NULL);
}

void philos_start(t_philo *philos_data)
{
	t_philo_id *philos_id;
	philos_id = malloc(sizeof(t_philo_id) * philos_data->num);
	pthread_t *threads;
	threads = malloc(sizeof(pthread_t) * philos_data->num);
	int i = 0;
	// if (!threads)
	// 	return (ft_error("Error: malloc failed\n"));
	// struct timeval tms;
	// gettimeofday(&tms, NULL);
	// philos_data->current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000; //

	printf("3\n");
	while (i < philos_data->num)
	{
		philos_id[i].id = i;
		philos_id[i].philo = philos_data;
		philos_id[i].time_stamp = -1;
		pthread_create(&threads[i], NULL, &p_life, &philos_id[i]);
		i++;
	}

	printf("4\n");
	while (philos_data->still_eating > 0)
	{
		if (philos_data->died > 0)
		{
			while (i > 0)
			{
				pthread_detach(threads[i - 1]);
				i--;
			}
			struct timeval tms;
			gettimeofday(&tms, NULL);
			philos_data->current_time = tms.tv_usec / 1000 + tms.tv_sec * 1000;
			printf("%d %d died\n", philos_data->current_time, philos_data->died);
			break;
		}
	}
	printf("5\n");
	// exit(0);
	while (i > 0)
	{
		pthread_join(threads[i - 1], NULL);
		i--;
	}
}

int main(int argc, char **argv)
{
	t_philo philo;

	if (argc < 5 || argc > 6)
		return (ft_error("Error: wrong number of arguments\n"));
	if (num_large(argv[1]) || num_large(argv[2]) || num_large(argv[3]) || num_large(argv[4]) || (argc == 6 && num_large(argv[5])))
		return (ft_error("Error: invalid parameters\n"));
	if (init_philo(&philo, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	philos_start(&philo);

	return (0);
}
