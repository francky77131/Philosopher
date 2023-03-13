/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_th.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franck <franck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 19:43:04 by franck            #+#    #+#             */
/*   Updated: 2023/03/13 19:43:06 by franck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philosopher.h"

void	ft_print(t_philo *philo, char *str)
{
	// Verrouille les mutex de write et death_check pour éviter les conflits de sortie
	pthread_mutex_lock(&philo->big_brother->write);
	pthread_mutex_lock(&philo->big_brother->death_check);
	// Si tous les philosophes n'ont pas terminé leur dîner, affiche le message avec le temps actuel, l'index du philosophe et la chaîne de caractères
	if (philo->big_brother->who_finished != philo->big_brother->number_of_philosophers)
		printf("%ld %d %s\n", get_time() - philo->big_brother->start_time, philo->index, str);
	// Déverrouille les mutex
	pthread_mutex_unlock(&philo->big_brother->death_check);
	pthread_mutex_unlock(&philo->big_brother->write);
}

void	eat(t_philo *philo)
{
	// Verrouille le premier fourchette
	pthread_mutex_lock(philo->first_fork);
	// Affiche le message que le philosophe a pris une fourchette
	ft_print(philo, "has taken a fork");
	// Verrouille le deuxième fourchette
	pthread_mutex_lock(philo->sec_fork);
	// Affiche le message que le philosophe a pris la deuxième fourchette
	ft_print(philo, "has taken a fork");
	// Affiche le message que le philosophe mange
	ft_print(philo, "is eating");
	// Verrouille le mutex death_check pour mettre à jour le temps jusqu'à la mort du philosophe
	pthread_mutex_lock(&philo->big_brother->death_check);
	philo->big_brother->time_until_death[philo->index - 1] = get_time() + philo->big_brother->time_to_die;
	// Déverrouille le mutex death_check
	pthread_mutex_unlock(&philo->big_brother->death_check);
	// Fait une pause pour simuler que le philosophe mange
	ft_usleep(philo->big_brother->time_to_eat);
	// Incrémente le compteur de dîners pour ce philosophe
	philo->dinner_count++;
	// Si le philosophe a atteint le nombre maximum de dîners, incrémente le nombre de philosophes qui ont terminé leur dîner
	if (philo->dinner_count == philo->big_brother->number_of_eat)
		philo->big_brother->who_finished++;
	// Déverrouille le deuxième fourchette
	pthread_mutex_unlock(philo->sec_fork);
	// Déverrouille le premier fourchette
	pthread_mutex_unlock(philo->first_fork);
}

void	ft_sleep(t_philo *philo)
{
	// Affiche le message que le philosophe dort
	ft_print(philo, "is sleeping");
	// Fait une pause pour simuler que le philosophe dort
	ft_usleep(philo->big_brother->time_to_sleep);
}

void	think(t_philo *philo)
{
	// Affiche le message que le philosophe pense
	ft_print(philo, "is thinking");
}

void	*philo_th(t_philo *philo)
{
	// Si nous n'avons qu'un seul philosophe dans la table, il n'a personne avec qui partager les fourchettes, donc il pense et quitte.
	if (philo->big_brother->number_of_philosophers == 1)
	{
		ft_print(philo, "is thinking"); // Affiche que le philosophe est en train de penser
		ft_print(philo, "has taken a fork"); // Affiche que le philosophe a pris une fourchette (qui n'existe pas)
		free(philo); // Libère l'espace mémoire alloué pour ce philosophe
		return (NULL); // Retourne NULL pour indiquer que la fonction est terminée
	}
	// Tant que le nombre de philosophes ayant terminé de manger est inférieur au nombre total de philosophes dans la table, les philosophes continuent de manger, penser et dormir.
	while (philo->big_brother->who_finished < philo->big_brother->number_of_philosophers)
	{
		// Si le nombre de philosophes ayant terminé de manger est inférieur au nombre total de philosophes dans la table, le philosophe pense.
		if (philo->big_brother->who_finished < philo->big_brother->number_of_philosophers)
			think(philo); // Appelle la fonction think() pour que le philosophe pense
		// Si le nombre de philosophes ayant terminé de manger est inférieur au nombre total de philosophes dans la table, le philosophe mange.
		if (philo->big_brother->who_finished < philo->big_brother->number_of_philosophers)
			eat(philo); // Appelle la fonction eat() pour que le philosophe mange
		// Si le nombre de philosophes ayant terminé de manger est inférieur au nombre total de philosophes dans la table, le philosophe dort.
		if (philo->big_brother->who_finished < philo->big_brother->number_of_philosophers)
			ft_sleep(philo); // Appelle la fonction ft_sleep() pour que le philosophe dorme
	}
	free(philo); // Libère l'espace mémoire alloué pour ce philosophe
	return (NULL); // Retourne NULL pour indiquer que la fonction est terminée
}

