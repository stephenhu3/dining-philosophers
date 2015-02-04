# DiningPhilosophers
Implements several strategies to avoid deadlock in the classic multithreaded scenario.

Left fork first and hold:
Because the synchronization strategy of this version starts with exclusively grabbing the left fork first
and then holding it until the right fork is available, this program results in deadlocks very frequently
where each philosopher is holding his left fork and waiting for his right fork to be available. In fact,
due to the circular nature of this holding strategy, each right fork is held as the left fork for each
philosopher. Therefore, each philosopher is holding one fork at the deadlock condition with no available
right fork to grab, hence no right fork to eat with and then put down. This is a stalemate where nothing 
happens and the philosophers hold onto their single left fork for eternity.

Resource ordering:
By ordering the forks in a non-circular fashion and having the fork precedence be dictated by the lower
numbered one, this prevents deadlocks by not having a single side of each philosophers' forks be grabbed
at a given moment. Thus, the deadlock mentioned above will not occur. While the napping time is random
between actions for the philosophers, the actions are somewhat predictable because the order of fork
acquisition is predetermined for each philosopher. Only the order between the philosophers' actions will be
randomized by the napping time each philosopher takes between actions.

Random:
This is the best strategy because the order of fork acquisition is randomized for each philosopher every turn.
There is no predefined fork aquisition order, which is even more non-deterministic than the resource ordering 
strategy. Combined with the philosopher's dropping of his current fork if the other is unavailable, this removes
the need to have the wait condition and allows the philosophers to complete their meals the fastest out of all the
strategies. With such a random nature to this program, and most importantly because a fork is never held when the
other is unavailable, the chance of deadlocking permanently is impossible, and with the chances of deadlocking 
momentarily extremely small, and with negligible consequence as the sequence randomizes and executes again.
