/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damboule <damboule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:50:14 by damboule          #+#    #+#             */
/*   Updated: 2020/03/11 18:40:05 by damboule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			ft_initi(t_salle *room)
{
	int		a;

	a = 0;
	while (a < T_SIZE)
	{
		room[a].liens = NULL;
		room[a].salle = NULL;
		a++;
	}
}

int				fill_link(t_salle *room, char **l_rooms)
{
	unsigned long		f_index;
	unsigned long		s_index;

	f_index = t_hash(l_rooms[0], room);
	s_index = t_hash(l_rooms[1], room);
	if ((room[f_index].salle == NULL || room[s_index].salle == NULL))
		return (-1);
	room[f_index].liens->out = (void *)t_hash(l_rooms[1], room);
	room[s_index].liens->out = (void *)t_hash(l_rooms[0], room);
	out_add_tolist(&room[f_index].liens, NULL, 1);
	out_add_tolist(&room[s_index].liens, NULL, 1);
	return (1);
}

int				create_room(t_salle *room, char *hash, t_out **n_index,
t_stack *info)
{
	unsigned long	index;

	if (ft_chrlen(hash, '-') > 0)
		info->tiret = -1;
	index = t_hash(hash, room);
	if (room[index].salle != NULL)
		return (-1);
	out_init(&room[index].liens, 0);
	room[index].salle = ft_strdup(hash, 0);
	(*n_index)->out = (void *)index;
	out_add_tolist(n_index, NULL, 1);
	room->uses = 0;
	room->free = 0;
	room->lenght = 0;
	room->ascend = 0;
	room->ant_numb = 0;
	room->nb_salle = 0;
	room->salle_prev[0] = 0;
	room->salle_prev[1] = 0;
	return (1);
}

int				check_rformat(char *to_check)
{
	char	**split;

	split = ft_strsplit(to_check, ' ');
	if (ft_strtab(split) == 3 && is_number(split[1], 1)
			&& is_number(split[2], 1)
			&& split[0][0] != '#' && split[0][0] != 'L' && free_all(split, 0))
		return (1);
	free_all(split, 0);
	return (-1);
}

int				check_lformat(char *to_check, t_salle **rooms, int truth,
t_stack *info)
{
	char	**split;

	if (truth != 2 && truth != 1)
		return (0);
	split = ft_strsplit(to_check, '-');
	if (ft_chrlen(to_check, '-') > 1)
		info->tiret = -1;
	if (ft_strtab(split) != 2 && free_all(split, 0))
		return (0);
	if (ft_strcmp(split[0], split[1]) == 0)
		return (1);
	if (fill_link(*rooms, split) == -1 && free_all(split, 0))
		return (-1);
	free_all(split, 0);
	return (1);
}
