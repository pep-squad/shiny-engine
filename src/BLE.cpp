#include "BLE.h"
// sudo apt-get install libboost-all-dev
BLE::BLE() {}

void BLE::scan() {
  // int outfd[2];
  // int infd[2];

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

    // execute the bash script to read ble packets
    execl("/bin/bash","bash","/home/pi/shiny-engine/bash/ble_scan.sh", NULL); // change to some common place later (/usr/bin)
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
      extractBuffer(buffer, &packets);
    }
  }
}

void BLE::extractBuffer(char buffer[], std::queue<std::string> *packets) {
  std::string str;
  char *t;
  int start = false;
  for (t = buffer; (*t) != '\0'; t++) {
    if ((*t) == '>' && start == 0) {
      start = true;
      str.clear();
    } else if ((*t) == '>' && start == true) {
      start = false;
      packets->push(str);
      t--;
    } else if (start == true) {
      if (((*t) >= 48 && (*t) <= 57) || ((*t) >= 65 && (*t) <= 70)) {
        str += (*t);
      }
    }
  }
}
