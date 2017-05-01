/*
** ftrace.h for ftrace in /home/zeng_d
**
** Made by David Zeng
** Login   <zeng_d@epitech.net>
**
** Started on  Mon May  1 18:54:32 2017 David Zeng
** Last update Mon May  1 18:54:32 2017 David Zeng
*/

#ifndef FTRACE_H_
# define FTRACE_H_

# include <stdio.h>
# include <stdlib.h>
# include <sys/ptrace.h>
# include <sys/reg.h>
# include <stdbool.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/user.h>
# include <signal.h>

# define SYSCALL 0x050f

typedef struct			s_proc
{
  pid_t				pid;
  struct user_regs_struct	regs;
  int				status;
}				t_proc;

typedef struct			s_syscall_proto
{
  unsigned int			argc;
  char				*name;
}				t_syscall_proto;

pid_t				fork_process(char *argv[]);
void				trace_process(pid_t pid);
void				trace_syscall(t_proc *proc);
void				trace_function(t_proc *proc);
t_syscall_proto			get_syscall_proto(int num);

#endif /* !FTRACE_H_ */