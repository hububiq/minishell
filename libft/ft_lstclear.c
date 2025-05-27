/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:27:38 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/23 10:56:08 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Deletes and frees the given node and every successor of that node, 
	using the function ’del’ and free(3).
	Finally, the pointer to the list must be set to NULL.
*/

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*buf;

	if (!lst || !del)
		return ;
	while ((*lst) != NULL)
	{
		buf = (*lst)->next;
		ft_lstdelone(*lst, del);
		(*lst) = buf;
	}
	*lst = NULL;
}
