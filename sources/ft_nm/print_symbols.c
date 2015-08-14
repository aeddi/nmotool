/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeddi <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/13 14:52:18 by aeddi             #+#    #+#             */
/*   Updated: 2015/08/13 06:59:46 by plastic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <libft.h>
#include <nmotool.h>

static t_symlist	*sort_list(t_symlist *root)
{
	short			check;
	t_symlist		*iter;

	check = 1;
	while (check)
	{
		check = 0;
		iter = root;
		while (iter->next)
		{
			if (ft_strcmp(iter->name, iter->next->name) > 0 && ++check)
				swap_symbols(iter, iter->next);
			iter = iter->next;
		}
	}
	return (root);
}

static boolean_t	is_printable(t_symlist *iter, t_arg_nm *opt)
{
	boolean_t	external;
	boolean_t	undefined;

	if (!(iter->n_type & N_EXT))
		external = TRUE;
	else
		external = FALSE;
	if ((iter->n_type & N_TYPE) == N_UNDF)
		undefined = TRUE;
	else
		undefined = FALSE;
	if (opt->o_ext && !external)
		return FALSE;
	if (opt->o_undef && !undefined)
		return FALSE;
	if (opt->n_undef && undefined)
		return FALSE;
	return TRUE;
}

static void			print_sym(t_symlist *iter, t_arg_nm *opt, boolean_t len64)
{
	if (!opt->p_short)
	{
		if (iter->n_value)
			print_ptr_to_hex(iter->n_value, FALSE, len64);
		else if (!iter->n_value && len64)
			ft_putstr("                ");
		else if (!iter->n_value)
			ft_putstr("        ");
		ft_putchar(' ');
		ft_putchar(iter->letter);
		ft_putchar(' ');
	}
	ft_putendl(iter->name);
}

void				print_list(t_symlist *root, t_arg_nm *opt, boolean_t len64)
{
	t_symlist		*iter;

	if (!opt->p_unsort)
		root = sort_list(root);
	if (opt->p_rev)
		iter = get_tail(root);
	else
		iter = root;
	while (iter)
	{
		if (is_printable(iter, opt))
			print_sym(iter, opt, len64);
		if (opt->p_rev)
			iter = iter->prev;
		else
			iter = iter->next;
	}
}
