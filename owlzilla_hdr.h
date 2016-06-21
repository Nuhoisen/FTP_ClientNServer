/*
 * R Jesse Chaney
 */

#ifndef __OWLZILLA_HDR_H
# define __OWLZILLA_HDR_H

// This will be the backlog argument. The maximum length
// to which the queue of pending connections for sockfd
// may grow. This is for the socket select implementation only.
# define LISTENQ 100

# define MAXLINE 1000  // max text line length
# define MAX_CLIENTS 10

# define SEND_FILE_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// This is a list of the available commands for the client.
// You can learn more about them by typing 'help' into the client.
# define CMD_EXIT            "exit"

# define CMD_REMOTE_HOME     "home"
# define CMD_LOCAL_HOME      "lhome"

# define CMD_REMOTE_CHDIR    "cd "
# define CMD_REMOTE_DIR      "dir"
# define CMD_REMOTE_PWD      "pwd"

# define CMD_LOCAL_CHDIR     "lcd "
# define CMD_LOCAL_DIR       "ldir"
# define CMD_LOCAL_PWD       "lpwd"

# define CMD_PUT_TO_SERVER   "put "
# define CMD_GET_FROM_SERVER "get "
# define CMD_HELP            "help"
# define CMD_HASH            "hash"


// This is the command to use with popen() to get a
// directory listing.
# define CMD_LS_POPEN    "ls -lFABh --group-directories-first"

# define PROMPT "==> "

# define RETURN_ERROR "**ERROR**"
# define DIR_SEP "/"
# define EOT_CHAR '\04'

#endif // __OWLZILLA_HDR_H
