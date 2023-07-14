/*-----------------------------------------------------------------------------
 * Copyright (C) 2006, Tony Mountifield
 *-----------------------------------------------------------------------------
 *
 * File:	cmd.c
 *
 * Description:	Invoke a command without using the shell
 *
 * Author:	Tony Mountifield
 * Date:	08-Jun-2006
 *
 *-----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int cmd(char *argv[])
{
	pid_t pid;
	int status;
	int fd;
	struct rlimit limits;
	struct sigaction sig;

	if ((pid = fork()) < 0)
		return -1;

	if (pid == 0) {
		/* child */

		sigemptyset(&sig.sa_mask);
		sig.sa_flags = 0;
		sig.sa_handler = SIG_DFL;
		sigaction(SIGPIPE, &sig, NULL);		/* default PIPE signals */
		sigaction(SIGTERM, &sig, NULL);		/* default TERM signals */
		sigaction(SIGHUP, &sig, NULL);		/* default HUP signals */

		getrlimit(RLIMIT_NOFILE, &limits);	/* find max FD */
		for (fd = 3; fd < limits.rlim_max; fd++)
			close(fd);			/* close all files except standard */

		execv(argv[0], argv);
		_exit(127);
		/*NOTREACHED*/
		return -1;
	}

	/* parent */
	while (waitpid(pid, &status, 0) < 0)
		if (errno != EINTR)
			return -1;

	return status;
}

static pid_t pids[256];

FILE *cmdpopen(char *argv[])
{
	pid_t pid;
	int fd;
	int fds[2];
	struct rlimit limits;
	struct sigaction sig;

	if (pipe(fds) < 0)
		return NULL;

	if ((pid = fork()) < 0)
		return NULL;

	if (pid == 0) {
		/* child */

		close(fds[0]);
		if (fds[1] != STDOUT_FILENO) {
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);
		}

		sigemptyset(&sig.sa_mask);
		sig.sa_flags = 0;
		sig.sa_handler = SIG_DFL;
		sigaction(SIGPIPE, &sig, NULL);		/* default PIPE signals */
		sigaction(SIGTERM, &sig, NULL);		/* default TERM signals */
		sigaction(SIGHUP, &sig, NULL);		/* default HUP signals */

		getrlimit(RLIMIT_NOFILE, &limits);	/* find max FD */
		for (fd = 3; fd < limits.rlim_max; fd++)
			close(fd);			/* close all files except standard */

		execv(argv[0], argv);
		_exit(127);
		/*NOTREACHED*/
		return NULL;
	}

	/* parent */

	close(fds[1]);
	pids[fds[0]] = pid;

	return fdopen(fds[0], "r");
}

int cmdpclose(FILE *fp)
{
	pid_t pid;
	int fd;
	int status;

	fd = fileno(fp);

	if (!(pid = pids[fd]))
		return -1;

	pids[fd] = 0;

	if (fclose(fp) == EOF)
		return -1;

	/* parent */
	while (waitpid(pid, &status, 0) < 0)
		if (errno != EINTR)
			return -1;

	return status;
}
