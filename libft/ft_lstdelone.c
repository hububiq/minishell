/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:26:44 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/23 10:56:52 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Takes as a parameter a node and frees the memory of the node’s content
	using the function ’del’ given as a parameter and free the node. 
	The memory of ’next’ must not be freed.
*/

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
