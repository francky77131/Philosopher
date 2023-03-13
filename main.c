/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franck <franck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 19:43:38 by franck            #+#    #+#             */
/*   Updated: 2023/03/13 19:43:40 by franck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philosopher.h"

void	*philo_init(t_big_brother *big_brother, int i)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo)); // Alloue de la mémoire pour la structure du philosophe
	if (!philo) // Vérifie si l'allocation de mémoire a échoué
		return (NULL); // Si oui, renvoie NULL
	big_brother->time_until_death[i - 1]
		= big_brother->start_time + big_brother->time_to_die; // Définit le temps de mort du philosophe
	philo->index = i; // Affecte l'index du philosophe
	philo->dinner_count = 0; // Initialise le compteur de dîners du philosophe
	philo->first_fork = &big_brother->fork[i - 1]; // Affecte la première fourchette du philosophe
	if (i == big_brother->number_of_philosophers)
		philo->sec_fork = &big_brother->fork[0]; // Affecte la deuxième fourchette du philosophe (cas où il est le dernier)
	else
		philo->sec_fork = &big_brother->fork[i]; // Affecte la deuxième fourchette du philosophe
	philo->big_brother = big_brother; // Affecte le grand frère (l'objet t_big_brother) du philosophe
	big_brother->philo[i - 1] = *philo; // Affecte la structure du philosophe à l'index correspondant dans le tableau des philosophes du grand frère
	return (philo); // Renvoie la structure du philosophe initialisé
}

int	big_brother_init(t_big_brother *big_brother)
{
	int			i;

	i = 0;
	// On alloue de la mémoire pour les philosophes.
	big_brother->philo = malloc(sizeof (t_philo) * big_brother->number_of_philosophers);
	if (!big_brother->philo) // Si l'allocation de mémoire a échoué, on arrête le programme.
		return (0);
	// On alloue de la mémoire pour les fourchettes.
	big_brother->fork = malloc(sizeof (pthread_mutex_t) * big_brother->number_of_philosophers);
	if (!big_brother->fork) // Si l'allocation de mémoire a échoué, on libère la mémoire allouée pour les philosophes et on arrête le programme.
		return (0);
	// On alloue de la mémoire pour les temps jusqu'à la mort.
	big_brother->time_until_death = malloc(sizeof(time_t) * big_brother->number_of_philosophers);
	if (!big_brother->time_until_death) // Si l'allocation de mémoire a échoué, on libère la mémoire allouée pour les philosophes et les fourchettes et on arrête le programme.
		return (0);
	// On initialise chaque mutex correspondant à une fourchette.
	while (i < big_brother->number_of_philosophers)
		pthread_mutex_init(&big_brother->fork[i++], NULL);
	// On initialise les mutex de contrôle de l'écriture et de la surveillance de la mort des philosophes.
	pthread_mutex_init(&big_brother->write, NULL);
	pthread_mutex_init(&big_brother->death_check, NULL);
	// On initialise le nombre de philosophes ayant fini de manger à 0.
	big_brother->who_finished = 0;
	// On enregistre le temps de départ de la simulation.
	big_brother->start_time = get_time();
	return (1);
}

pthread_t *philo_launch(t_big_brother *big_brother)
{
    pthread_t *philo; // un pointeur vers les threads des philosophes
    int i; // un compteur utilisé dans les boucles

    i = 1;
    philo = malloc(sizeof(pthread_t) * big_brother->number_of_philosophers); // alloue de la mémoire pour les threads des philosophes
    if (!philo)
        return ((void *)NULL); // renvoie NULL si l'allocation de mémoire a échoué

    while (i <= big_brother->number_of_philosophers)
    {
        if (pthread_create(philo + (i - 1), NULL, (void *)philo_th, // crée un thread pour chaque philosophe impair en utilisant la fonction philo_th
                            philo_init(big_brother, i))) // initialise le philosophe avec les arguments nécessaires
            return ((void *)NULL); // renvoie NULL si la création du thread a échoué
        i += 2; // incrémente le compteur pour passer au philosophe suivant (pair)
    }

    i = 2;
    ft_usleep(5); // attend 5 microsecondes

    while (i <= big_brother->number_of_philosophers)
    {
        if (pthread_create(philo + (i - 1), NULL, (void *)philo_th, // crée un thread pour chaque philosophe pair en utilisant la fonction philo_th
                            philo_init(big_brother, i))) // initialise le philosophe avec les arguments nécessaires
            return ((void *)NULL); // renvoie NULL si la création du thread a échoué
        i += 2; // incrémente le compteur pour passer au philosophe suivant (impair)
    }

    return (philo); // renvoie le pointeur vers les threads des philosophes
}

void	death_cycle(t_big_brother *big_brother, int i)
{
	// Attendre un peu avant de commencer à vérifier le temps de vie des philosophes
	usleep (2000);
	
	while (1)
	{
		// Si on a vérifié tous les philosophes, retourner au premier philosophe
		if (i == big_brother->number_of_philosophers)
			i = 0;
		
		// Verrouiller le mutex pour accéder aux données liées à la mort des philosophes
		pthread_mutex_lock(&big_brother->death_check);
		
		// Si le temps de vie du philosophe courant est écoulé
		if (big_brother->time_until_death[i] <= get_time())
		{
			// Enregistrer le philosophe qui est mort
			big_brother->who_finished = big_brother->number_of_philosophers;
			
			// Déverrouiller le mutex
			pthread_mutex_unlock(&big_brother->death_check);
			
			// Attendre un peu avant d'afficher le message de mort
			usleep(10);
			
			// Afficher le message de mort du philosophe courant
			printf("%ld %d died\n", get_time() - big_brother->start_time, i + 1);
			
			// Retourner à la fonction appelante
			return ;
		}
		
		// Si tous les philosophes ont fini de manger
		if (big_brother->who_finished == big_brother->number_of_philosophers)
		{
			// Enregistrer le philosophe qui est mort
			big_brother->who_finished = big_brother->number_of_philosophers;
			
			// Déverrouiller le mutex
			pthread_mutex_unlock(&big_brother->death_check);
			
			// Attendre un peu avant de retourner à la fonction appelante
			usleep(10);
			
			// Retourner à la fonction appelante
			return ;
		}
		
		// Déverrouiller le mutex
		pthread_mutex_unlock(&big_brother->death_check);
		
		// Attendre un peu avant de passer au philosophe suivant
		usleep(500);
		
		// Passer au philosophe suivant
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_big_brother	*big_brother;
	pthread_t		*philo;

	big_brother = malloc(sizeof(t_big_brother));
	if (!big_brother)
		return (0);
	if (philo_parsing(big_brother, argc, argv) == 0) //si le parsing n'est pas bon je free et stop le prog.
	{
		printf(RED"Error\n"END);
		free(big_brother);
		return (0);
	}
	/*Si l'initialisation de la structure t_big_brother est réussie, 
	la fonction big_brother_init est appelée pour initialiser les données 
	nécessaires pour la simulation. Si l'initialisation réussit, la fonction philo_launch 
	est appelée pour créer des threads représentant les philosophes. Si les threads sont créés avec 
	succès, la fonction death_cycle est appelée pour surveiller les philosophes et gérer leur vie.*/
	if (big_brother_init(big_brother) == 1)
	{
		philo = philo_launch(big_brother);
		if (philo)
			death_cycle(big_brother, 0);
		philo_clear(big_brother, philo);
	}
	big_brother_clear(big_brother);
}
