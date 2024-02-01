# WatchDog Library

WatchDog is a C library designed to protect critical sections of code within a user's program. It ensures the integrity and stability of the program by monitoring its execution and restarting the process in case of a crash or unexpected termination.

## Features

- **Automatic process restart:** If the program crashes during the protected section, WatchDog automatically restarts the process, providing resilience against unexpected failures.
- **Customizable timeout:** Users can specify a timeout value for graceful termination of the WatchDog process, allowing for controlled shutdown if necessary.

## Getting Started

To integrate WatchDog into your project, follow these steps:

1. Clone the WatchDog repository to your local machine
2. Include the `watch_dog.h` header file in your project:

```c
#include "watch_dog.h"
```
3. Call the WDStart function at the beginning of the critical section you want to protect. Pass the path of the executable of your program as a parameter:
```c
WDStart("/path/to/your/program");
```
4. Call the 'WDStop' function at the end of the critical section, specifying a timeout value for graceful termination:
```c
WDStop(30);
```
## Example
```c
#include "watch_dog.h"

int main() {
    // Start WatchDog
    WDStart("/path/to/your/program");

    // Protected code...

    // Stop WatchDog without a timeout
    WDStop(0);

    return 0;
}
```
