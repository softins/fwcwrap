/*-----------------------------------------------------------------------------
 * Copyright (C) 2006, Tony Mountifield
 *-----------------------------------------------------------------------------
 *
 * File:	cmd.h
 *
 * Description:	Invoke a command without using the shell
 *
 * Author:	Tony Mountifield
 * Date:	08-Jun-2006
 *
 *-----------------------------------------------------------------------------
 */

int cmd(char *argv[]);
FILE *cmdpopen(char *argv[]);
int cmdpclose(FILE *fp);
