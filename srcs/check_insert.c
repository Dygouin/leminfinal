/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_insert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danglass <danglass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 13:15:56 by dygouin           #+#    #+#             */
/*   Updated: 2020/05/05 14:19:26 by danglass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static int		special_insert(t_salle **rooms, t_stack *info, t_out **index,
		char *line)
{
	char	**splited;

	if (check_rformat(line) == -1 || ft_chrlen(line, ' ') >= 3)
		return (1);
	splited = ft_strsplit(line, ' ');
	if (create_room(*rooms, splited[0], index, info) == -1)
		return (1);
	if (info->ways == 0)
	{
		info->n_start = ft_strdup(splited[0], 0);
		info->index_start = t_hash(splited[0], *rooms);
	}
	if (info->ways == 1)
	{
		info->n_end = ft_strdup(splited[0], 0);
		info->index_end = t_hash(splited[0], *rooms);
	}
	free_all(splited, 0);
	info->ways = 3;
	return (0);
}

static int		check_hash(t_stack *info, char *line)
{
	if (line[1] != '#')
		return (1);
	else if (info->fourmies == -1)
		return (0);
	else if (ft_strcmp((line), "##start") == 0
			&& info->n_start == NULL && info->ways == 3)
	{
		info->ways = 0;
		return (1);
	}
	else if (ft_strcmp((line), "##end") == 0
			&& info->n_end == NULL && info->ways == 3)
	{
		info->ways = 1;
		return (1);
	}
	else if (ft_strncmp((line), "##", 2) == 0 &&
		ft_strcmp((line), "##end") != 0 &&
			ft_strcmp((line), "##start") != 0)
		return (1);
	else
		return (0);
	return (1);
}

static int		what_line(t_salle **rooms, t_out **index, char *line,
		t_stack *info)
{
	char	**splited;
	int		*truth;

	truth = &info->truth;
	if (ft_chrlen(line, ' ') >= 3)
		return (1);
	if (check_rformat(line) == 1 && *truth == 1 && info->step == STEP_ONE)
	{
		splited = ft_strsplit(line, ' ');
		if (create_room(*rooms, splited[0], index, info) == -1)
		{
			free_all(splited, 0);
			return (0);
		}
		free_all(splited, 0);
		return (1);
	}
	else if (check_lformat(line, rooms, *truth, info) == 1)
	{
		info->step = 2;
		if (*truth == 1)
			*truth = 2;
		return (1);
	}
	return (0);
}

int				check_ants(t_stack *info, char *line, int *truth)
{
	if (((info->fourmies != -1 || !is_number(line, 1))
		|| ((info->fourmies = ft_atoi_check(line)) <= 0)))
		return (1);
	*truth = 1;
	return (0);
}

void			check_insert(t_out **reads, t_out **index, t_salle **rooms,
t_stack *info)
{
	char	*line;

	line = NULL;
	while (free_reset(line) && get_next_line(0, &line) > 0 && line != NULL)
	{
		if (line[0] == '#')
		{
			if ((!check_hash(info, line)
				|| (!(info->fourmies) && line[1] == '#')) && free_reset(line))
				break ;
		}
		else if ((info->ways == 0 || info->ways == 1) && info->step == STEP_ONE)
		{
			if (special_insert(rooms, info, index, line) && free_reset(line))
				break ;
		}
		else if (is_number(line, 0))
		{
			if (check_ants(info, line, &info->truth) && free_reset(line))
				break ;
		}
		else if (what_line(rooms, index, line, info) == 0 && free_reset(line))
			break ;
		out_add_tolist(reads, line, 0);
	}
}
