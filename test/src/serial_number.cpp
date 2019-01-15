#include "serial_number.h"

unsigned long long getSerialNumber() {
  int pipes[NUM_PIPES][2];

  // pipes for parent to write and read
  pipe(pipes[PARENT_READ_PIPE]);
  pipe(pipes[PARENT_WRITE_PIPE]);

  pid_t pid  = fork();
  if(pid > 0) {
    // child process
    dup2(CHILD_READ_FD, STDIN_FILENO);
    dup2(CHILD_WRITE_FD, STDOUT_FILENO);

    // close fds not required by child
    close(CHILD_READ_FD);
    close(CHILD_WRITE_FD);
    close(PARENT_READ_FD);
    close(PARENT_WRITE_FD);

    // execute the bash script to read serial number
    execl("/bin/bash","bash","/home/pi/shiny-engine/bash/serial_no.sh", NULL);
  } else if (pid == 0) {
    // parent process
    char buffer[200];
    int count;

    // close fds not required by parent
    close(CHILD_READ_FD);
    close(CHILD_WRITE_FD);

    // Write to child’s stdin
    write(PARENT_WRITE_FD, "2^32\n", 5);

    // Read from child’s stdout
    while((count = read(PARENT_READ_FD, buffer, sizeof(buffer)-1)) > 0) {
      buffer[count] = 0;
      buffer[count-1] = 0;
      std::string serial (buffer);
      // serial.insert(0, "F");
      std::stringstream sno;
      sno << serial;
      unsigned long long value;
      sno >> std::hex >> value;
      return value;
    }
  }
  return 0;
}
