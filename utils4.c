
#include "philosophers.h"

void	th_set(t_locks *lock, int val, int locked)
{
	if (!locked)
		pthread_mutex_lock(&lock->lock);
	lock->val = val;
	// printf("set lock %d \n", lock->val);
	pthread_mutex_unlock(&lock->lock);
}

int	th_get(t_locks *lock, int unlock)
{
	int	val;

	pthread_mutex_lock(&lock->lock);
	val = lock->val;
	if (unlock)
		pthread_mutex_unlock(&lock->lock);
	return (val);
}

