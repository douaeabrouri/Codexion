*This project has been created as part of the 42 curriculum by doabrour.*

# Codexion

## Description

Codexion is a multithreaded simulation project focused on thread synchronization, resource sharing, scheduling algorithms, and concurrency management using POSIX threads (pthreads).

In the simulation, multiple coders compete for shared resources called **dongles** in order to compile their code. Each coder repeatedly performs the following actions:

1. Acquire the required dongles.
2. Compile.
3. Debug.
4. Refactor.

The project introduces several real-world concurrency challenges such as:

- Deadlock prevention
- Starvation prevention
- Resource cooldown management
- Burnout detection
- Thread-safe logging
- Scheduler-based resource allocation

Two scheduling strategies are supported:

- **FIFO (First In First Out)**
- **EDF (Earliest Deadline First)**

The goal of the project is to understand how concurrent systems behave and how synchronization mechanisms can be used to coordinate multiple threads safely.

---

# Features

- Multithreaded coder simulation
- Shared dongle management using mutexes
- Burnout monitoring thread
- FIFO scheduler
- EDF scheduler
- Dongle cooldown handling
- Precise timestamped logging
- Compile-count completion detection
- Deadlock prevention strategy
- Thread-safe simulation shutdown

---

# Instructions

## Compilation

```bash
gcc $(find . -name "*.c") -I. -lpthread -o codexion
```

or with warnings enabled:

```bash
gcc $(find . -name "*.c") -I. -Wall -Wextra -Werror -lpthread -o codexion
```

---

## Execution

```bash
./codexion \
<number_of_coders> \
<time_to_burnout> \
<time_to_compile> \
<time_to_debug> \
<time_to_refactor> \
<number_of_compiles_required> \
<dongle_cooldown> \
<FIFO|EDF>
```

### Example

```bash
./codexion 5 800 200 100 100 3 50 FIFO
```

### Parameters

| Parameter | Description |
|------------|------------|
| number_of_coders | Number of coder threads |
| time_to_burnout | Maximum time a coder can go without compiling |
| time_to_compile | Time spent compiling |
| time_to_debug | Time spent debugging |
| time_to_refactor | Time spent refactoring |
| number_of_compiles_required | Required compilations before completion |
| dongle_cooldown | Cooldown applied after releasing a dongle |
| FIFO / EDF | Scheduling strategy |

---

# Scheduling Algorithms

## FIFO

FIFO (First In First Out) schedules coders according to the order in which they requested access to the compilation resources.

The first waiting coder is always selected first.

Example:

```text
Request order:

Coder 3
Coder 1
Coder 2

Execution order:

Coder 3
Coder 1
Coder 2
```

---

## EDF

EDF (Earliest Deadline First) selects the coder whose deadline is the closest.

The deadline is calculated as:

```text
deadline = last_compile + time_to_burnout
```

The coder with the smallest deadline receives priority.

Example:

```text
Coder 1 deadline = 5000
Coder 2 deadline = 2000
Coder 3 deadline = 8000

Selected coder:

Coder 2
```

---

# Blocking Cases Handled

## Deadlock Prevention

The classical deadlock scenario occurs when every coder acquires one dongle and waits forever for another.

To prevent this, coders acquire dongles using different locking orders:

```text
Even coders:
    Left → Right

Odd coders:
    Right → Left
```

This breaks the circular wait condition and prevents deadlock.

---

## Coffman's Conditions

Deadlocks require the following four conditions:

1. Mutual Exclusion
2. Hold and Wait
3. No Preemption
4. Circular Wait

The implementation breaks the Circular Wait condition, preventing deadlock formation.

---

## Starvation Prevention

Starvation is reduced by using scheduling policies that guarantee waiting coders eventually receive execution opportunities.

FIFO naturally provides fairness.

EDF prioritizes coders that are closest to burnout.

---

## Dongle Cooldown Handling

After a dongle is released, it enters a cooldown period.

A coder cannot immediately reuse a dongle before its cooldown expires.

Each dongle stores:

```c
last_release
```

The elapsed time is checked before the dongle becomes available again.

---

## Precise Burnout Detection

A dedicated monitor thread continuously checks:

```text
current_time - last_compile
```

If this value exceeds:

```text
time_to_burnout
```

the coder burns out and the simulation terminates.

---

## Log Serialization

Logging is protected using a dedicated mutex.

This prevents messages from different threads from interleaving.

Example:

Incorrect:

```text
100 Coder 1 is compi
100 Coder 2 is debuging
ling
```

Correct:

```text
100 Coder 1 is compiling
120 Coder 2 is debugging
```

---

# Thread Synchronization Mechanisms

## pthread_mutex_t

Mutexes are used to protect shared resources.

Protected data includes:

- Dongles
- Logging output
- Scheduler state
- Monitor state
- Simulation termination flag

Example:

```c
pthread_mutex_lock(&dongle->mutex);

/* critical section */

pthread_mutex_unlock(&dongle->mutex);
```

---

## Shared Resource Synchronization

Each dongle owns a dedicated mutex:

```c
typedef struct s_dongle
{
    pthread_mutex_t mutex;
} t_dongle;
```

Only one coder may hold a dongle at a time.

---

## Monitor Synchronization

The monitor thread shares state with coder threads.

The simulation termination flag is protected by a mutex to prevent race conditions.

Example:

```c
int is_simulation_over(t_data *data);
void set_simulation_over(t_data *data);
```

These helper functions guarantee synchronized access.

---

## Race Condition Prevention

Without synchronization:

```c
data->simulation_end = 1;
```

and

```c
while (!data->simulation_end)
```

can create undefined behavior because multiple threads access the same variable simultaneously.

Using mutexes ensures:

```text
One thread writes.
Other threads read safely.
```

---

## Thread-Safe Communication

Communication between coders and the monitor occurs through shared synchronized state:

- simulation_end
- last_compile
- finish_compile
- dongle cooldown timestamps

All accesses are protected by synchronization primitives to ensure visibility between threads.


# Resources

## POSIX Threads

- https://man7.org/linux/man-pages/man7/pthreads.7.html
- https://man7.org/linux/man-pages/man3/pthread_create.3.html
- https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html

## AI Usage

- Understanding pthread concepts
- Understanding deadlocks and Coffman's conditions
- Understanding FIFO and EDF scheduling
- Reviewing synchronization strategies
- Clarifying project requirements