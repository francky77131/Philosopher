/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franck <franck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 19:43:15 by franck            #+#    #+#             */
/*   Updated: 2023/03/13 19:43:16 by franck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philosopher.h"

/*La fonction ft_usleep prend un paramètre time qui est le temps de sommeil en millisecondes, 
elle utilise ensuite la fonction usleep de la bibliothèque standard pour mettre en pause 
le thread d'exécution en cours pendant la durée spécifiée en microsecondes.

La fonction get_time utilise la structure timeval de la bibliothèque 
sys/time.h pour récupérer l'heure actuelle en millisecondes depuis 
l'epoch Unix (1er janvier 1970). Elle renvoie ensuite ce temps en millisecondes. 
Cette fonction est utilisée pour calculer le temps écoulé entre deux événements dans le programme, 
par exemple pour déterminer la durée 
d'un repas ou d'un sommeil pour un philosophe.*/

void	ft_usleep(int time)
{
	usleep(time * 1000);
}

time_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
