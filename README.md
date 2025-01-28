# Operating-Systems-for-Embedded-Apps

group/partners effort too, explain that we are group A1


Seminar #1 description: 
Create a shell application that can run simple commands from /usr/bin and at least 2 custom commands: {exit and help}

exit: will terminate your application

help: display some details about your application


Seminar #2 description:
Implement a scheduler (MLFQ (Multi Level Feedback Queue) or Lottery) in C

^ explain how in our group case we did MLFQ with the round robin method


Seminar #3 descriptions:
We can have multiple consumers and multiple producers/vendors (TBD during the seminar).

Hint: use one condition variable (notification list) for the stock empty and another one for the stock full (stock full = 100 units).

Each consumer will be able to purchase up to 5 units.

Each producer will be able to deliver up to 20 units.

Print nice status updates (who is waiting, what is the current stock limit).

Consumers show up every TBD seconds. Each consumer is a new thread.

Producers show up every TBD seconds. Each producer is a new thread.

