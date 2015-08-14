/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_symbols_32.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeddi <aeddi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/23 18:03:20 by aeddi             #+#    #+#             */
/*   Updated: 2015/08/14 15:40:46 by aeddi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <libft.h>
#include <nmotool.h>

static void	get_sect_names(char *sect_names[], struct segment_command *seg)
{
	static size_t	index = 0;
	size_t			count = 0;
	struct section	*sec;

	sec = (struct section *)(seg + 1);
	while (count < seg->nsects)
	{
		sect_names[index] = sec->sectname;
		sec++;
		count++;
		index++;
	}
	sect_names[index] = NULL;
}

static void	fill_list(struct symtab_command *sym_t, t_symlist **root, void *ptr)
{
	char						*str_t;
	struct nlist				*sym;
	t_symlist					*tmp;
	size_t						i;

	i = 0;
	str_t = (char *)ptr + sym_t->stroff;
	sym = (struct nlist *)((char *)ptr + sym_t->symoff);
	while (i < sym_t->nsyms)
	{
		if (!(sym[i].n_type & N_STAB))
		{
			if (!*root)
			{
				*root = add_sym(str_t + sym[i].n_un.n_strx, sym + i, 0, FALSE);
				tmp = *root;
			}
			else
			{
				tmp->next = add_sym(str_t + sym[i].n_un.n_strx, sym + i, tmp,
									TRUE);
				tmp = tmp->next;
			}
		}
		i++;
	}
}

void		find_symbols_32(t_head *headers, t_arg_nm *options)
{
	struct segment_command	*seg;
	t_symlist				*root;
	char					*sect_names[256];
	size_t					count;

	count = 0;
	seg = (struct segment_command *)(headers->mach32 + 1);
	root = NULL;
	while (count < headers->mach32->ncmds)
	{
		if (seg->cmd == LC_SEGMENT)
			get_sect_names(sect_names, seg);
		if (seg->cmd == LC_SYMTAB)
			fill_list((struct symtab_command *)seg, &root, headers->mach32);
		seg = (struct segment_command *)((char *)seg + seg->cmdsize);
		count++;
	}
	if (root)
	{
		get_symbols_letters(root, sect_names);
		print_list(root, options, FALSE);
		free_symlist(root);
	}
}
