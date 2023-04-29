/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 20:20:11 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 20:20:12 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	th_set(t_locks *lock, int val, int locked)
{
	if (!locked)
		pthread_mutex_lock(&lock->lock);
	lock->val = val;
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
