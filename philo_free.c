/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franck <franck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 19:44:04 by franck            #+#    #+#             */
/*   Updated: 2023/03/13 19:44:05 by franck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosopher.h"

void	big_brother_clear(t_big_brother *big_brother)
{
	int	i;

	i = 1;
	pthread_mutex_destroy(&big_brother->write);
	pthread_mutex_destroy(&big_brother->death_check);
	while (i < big_brother->number_of_philosophers)
		pthread_mutex_destroy(&big_brother->fork[i++]);
	free(big_brother->time_until_death);
	free(big_brother->fork);
	free(big_brother);
}

void	philo_clear(t_big_brother *big_brother, pthread_t *philo)
{
	int	i;

	i = 0;
	while (i < big_brother->number_of_philosophers)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	free(&philo[0]);
	free(&big_brother->philo[0]);
	return ;
	philo = NULL;
}
