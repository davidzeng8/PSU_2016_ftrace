/*
** function_binary.c for function_binary in /home/zeng_d
**
** Made by David Zeng
** Login   <zeng_d@epitech.net>
**
** Started on  Wed May  3 16:20:29 2017 David Zeng
** Last update Wed May  3 16:20:29 2017 David Zeng
*/

#include "ftrace.h"

static bool	can_access(char const *file)
{
  struct stat	statbuf;

  return (!(stat(file, &statbuf) != 0 ||
	    S_ISDIR(statbuf.st_mode) ||
	    access(file, F_OK | X_OK) != 0));
}

static int	open_bin(char const *file)
{
  char		path[256];
  char		*p;
  char		buf[256];

  if (strchr(file, '/'))
    return (-1);
  strcpy(path, getenv("PATH"));
  p = strtok(path, ":");
  while (p)
    {
      strcpy(buf, p);
      strcat(buf, "/");
      strcat(buf, file);
      if (can_access(buf))
	return (open(buf, O_RDONLY));
      p = strtok(NULL, ":");
    }
  return (-1);
}

char		*read_file(char const *filename, bool should_close)
{
  static int	fd = -1;
  char		*file;

  if (should_close && fd != -1)
    {
      close(fd);
      fd = -1;
      return (NULL);
    }
  if ((fd = open(filename, O_RDONLY)) == -1 && (fd = open_bin(filename)) == -1)
    {
      fprintf(stderr, "Couldn't open file %s\n", filename);
      exit(1);
    }
  file = mmap(NULL, lseek(fd, 0, SEEK_END), PROT_READ, MAP_SHARED, fd, 0);
  if (!file)
    {
      fprintf(stderr, "Couldn't read file %s\n", filename);
      close(fd);
      exit(1);
    }
  return (file);
}

static char	*get_name(Elf64_Sym *sym, int symsize, char *strtab,
			  unsigned int addr)
{
  int		i;

  i = -1;
  while (++i < symsize)
    {
      if (sym[i].st_value == addr)
	return (strdup(&strtab[sym[i].st_name]));
    }
  return (NULL);
}

char		*function_binary(unsigned long addr)
{
  static char	*file = NULL;
  int		i;
  t_elf		elf;

  if (!file)
    file = read_file(g_prog, false);
  elf.symsize = 0;
  elf.elf = (Elf64_Ehdr *)file;
  if (!IS_ELF(file))
    exit(1);
  elf.shdr = (Elf64_Shdr *)(file + elf.elf->e_shoff);
  elf.ststrtab = file + elf.shdr[elf.elf->e_shstrndx].sh_offset;
  i = -1;
  while (++i < elf.elf->e_shnum)
    {
      if (elf.shdr[i].sh_type == SHT_SYMTAB)
	{
	  elf.sym = (Elf64_Sym *)(file + elf.shdr[i].sh_offset);
	  elf.symsize = elf.shdr[i].sh_size / sizeof(Elf64_Sym);
	}
      if (elf.shdr[i].sh_type == SHT_STRTAB &&
	  strncmp(&elf.ststrtab[elf.shdr[i].sh_name], ".strtab", 7) == 0)
	elf.strtab = file + elf.shdr[i].sh_offset;
    }
  return (get_name(elf.sym, elf.symsize, elf.strtab, addr));
}
