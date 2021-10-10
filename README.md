## DeadLock detection Software in C language for Multi-threaded system with Fixed and common resources


# Problem Statement

We need to develop a program in C to simulate the following scenario involving detection of deadlocks among a pool of worker threads that will be started by this program. The simulation should determine whether a deadlock has occurred in a system of n identical resources and m threads. The program should take the following values as command line arguments:

* Types of resources that are available on the OS and that can be requested by threads. Example: A, B, C, etc. Name doesn’t really matter as long as it is unique.
* Maximum number of instances available for each of the above resources. Example: A=20, B=16, C=45, and so on.
* Maximum number of threads to use in the simulation.
* Deadlock detection check interval d in seconds.

Each thread, t, should decide a random set Rt of resources that it wants (e.g., 4 instances of resource A, 3 of resource B and 1 of resource C). Then, the thread t will request the resources (from Rt) one type at a time in a random order and with random pauses between making requests for different resource types. The thread t should hold onto all the resources R ⊂ Rt that it has acquired until it can get all of them, i.e., entire set Rt. Once a thread obtains all resources that it wanted, it should hold onto them for a random duration between (0.7d, 1.5d), then release them all at once. The thread should not terminate, but it should again decide a random set of resources to request, and repeat the process mentioned above.

The above pattern of working will lead to deadlocks among the worker threads. There should be a dedicated/separate thread t’ to check for deadlocks among worker threads every d seconds. It should report when a deadlock has occurred and do the following:

* Print the list of threads that are involved in the deadlock
* Terminate the worker threads involved in the deadlock. This should be done by using suitable heuristics. For example, you may use the number of resources that are currently held by a thread to decide which thread to terminate first. The terminated threads should be replaced by new ones.
* Track the time interval that is observed between the detected deadlocks.

You should use different heuristics for choosing worker threads to terminate and report which type of heuristic results in the longest and shortest average time between deadlocks. The simulation should continue to run until the user terminates it.

# About the code
Updating soon before 14 October,2021
*stay tuned!!*

## Acknowledgments

* [Dr. Balwinder Sodhi](https://cse.iitrpr.ac.in/sodhi/): for making me work on this in **Operating Systems** course.


