#include <stdio.h>

#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int pipe_out_fd;
int pipe_in_fd;

char * pipe_out_file = "/var/run/mud/webhook";
char * pipe_in_file = "/var/run/mud/api";

/*
 * setup_pipes
 * 
 * create a named pipe to support sending information to external program
 */
void setup_pipes() {
  mkfifo(pipe_out_file, 0666);
  pipe_out_fd = open(pipe_out_file, O_RDWR);

  // attempt to open the pipe_in_file
  pipe_in_fd = open(pipe_in_file, O_RDWR);
  // if pipe_in_fd is -1, we'll attempt to handle that again later
}

void teardown_pipes() {
  close(pipe_out_fd);
  close(pipe_in_fd);
}

int postMessage(char *msg) {
  return write(pipe_out_fd, msg, strlen(msg)+1);
}

int getMessage(char *buffer, int size) {
  int rv;
  int bytes_read = 0;
  fd_set set;
  struct timeval timeout;

  if( pipe_in_fd == -1 ) { // file wasn't available on initial startup, try again
    pipe_in_fd = open(pipe_in_file, O_RDONLY);

    if( pipe_in_fd == -1 ) {
      return 0;
    }
  }

  FD_ZERO(&set);
  FD_SET(pipe_in_fd, &set);

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  rv = select(pipe_in_fd + 1, &set, NULL, NULL, &timeout);

  if( rv == -1 ) {
    perror("select");
  }
  else if( rv == 0 ) {
    //printf("timeout\n");
  }
  else {
    bytes_read = read(pipe_in_fd, buffer, size);
  }

  return bytes_read;
}

