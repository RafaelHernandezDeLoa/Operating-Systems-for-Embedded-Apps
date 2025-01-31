# Operating-Systems-for-Embedded-Apps

Consists of the linux programs my group and I had made in this course

Seminar #1 description: 
Create a shell application that can run simple commands from /usr/bin and at least 2 custom commands: {exit and help}

exit: will terminate your application

help: display some details about your application/


Seminar #2 description:
Implement a scheduler (MLFQ (Multi Level Feedback Queue) or Lottery) in C

Note on my Seminar #2: My group were tasked to implement a MLFQ scheduler with round robin queues


Seminar #3 descriptions:
We can have multiple consumers and multiple producers/vendors (TBD during the seminar).

Hint: use one condition variable (notification list) for the stock empty and another one for the stock full (stock full = 100 units).

Each consumer will be able to purchase up to 5 units.

Each producer will be able to deliver up to 20 units.

Print nice status updates (who is waiting, what is the current stock limit).

Consumers show up every TBD seconds. Each consumer is a new thread.

Producers show up every TBD seconds. Each producer is a new thread.


Seminar #4 description:
create your own custom linux driver

Hints:  

https://linux.die.net/man/3/writeLinks to an external site.\
https://linux.die.net/man/3/readLinks to an external site.\
man ioctl\
include fs.h and uaccess.h\
nice to have for ioctl:\
use a structure that is shared between the driver and the user space application\
have 2 integers inside the structure \
param_name (e.g.: READ_POINTER, WRITE_POINTER, RESET_READ_POINTER, RESET_WRITE_POINTER)\
param_value (e.g.: 0)\

Note on my Seminar #4: Our kernel is a hangman game and probably specify it a bit more......................
