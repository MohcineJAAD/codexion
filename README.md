*This project has been created as part of the 42 curriculum by mjaad.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. N coders sit in a circle around a shared Quantum Compiler. Each coder alternates between three phases: **compiling**, **debugging**, and **refactoring**.

To compile, a coder must simultaneously hold two USB dongles — one from the left and one from the right. There are exactly as many dongles as coders, placed between each pair of neighbors. If a coder fails to start compiling within `time_to_burnout` milliseconds since their last compile (or since the simulation started), they burn out and the simulation ends.

The simulation also ends successfully when every coder has compiled at least `number_of_compiles_required` times.

Two scheduling policies are supported for dongle arbitration:
- **FIFO**: dongles are granted in arrival order.
- **EDF** (Earliest Deadline First): dongles are granted to the coder whose burnout deadline is closest.

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. `scheduler` must be exactly `fifo` or `edf`.

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders and dongles |
| `time_to_burnout` | Max ms a coder can go without starting to compile |
| `time_to_compile` | Ms spent compiling (holds 2 dongles) |
| `time_to_debug` | Ms spent debugging |
| `time_to_refactor` | Ms spent refactoring |
| `number_of_compiles_required` | Compiles needed per coder to end successfully |
| `dongle_cooldown` | Ms a dongle is unavailable after being released |
| `scheduler` | `fifo` or `edf` |

### Example

```bash
./codexion 5 800 200 100 100 3 100 fifo
./codexion 5 800 200 100 100 3 100 edf
```

### Log format

```
timestamp_in_ms coder_id action
```

Actions: `has taken a dongle`, `is compiling`, `is debugging`, `is refactoring`, `burned out`.

## Blocking cases handled

### Deadlock prevention
A classic deadlock occurs when every coder holds their left dongle and waits for the right one indefinitely. This is resolved by making the last coder (id == number_of_coders) acquire dongles in reverse order (right first, then left). This breaks the circular wait condition, one of Coffman's four necessary conditions for deadlock.

### Starvation prevention (FIFO)
Under FIFO scheduling, coders are served strictly in arrival order using a min-heap ordered by `enqueue_time`. No coder can be skipped — the one who waited longest always goes next.

### Starvation prevention (EDF)
Under EDF scheduling, the coder closest to burning out is always served first. As a coder waits longer without compiling, their deadline becomes more urgent, so they naturally move to the front of the queue. This guarantees liveness provided the simulation parameters are feasible.

### Cooldown liveness bug
After a dongle is released, it enters a cooldown period during which it cannot be acquired. A coder at the front of the queue (`heap[0]`) that finds the dongle in cooldown must not call `pthread_cond_wait` — because no signal will come once the cooldown expires (the dongle is already free, nobody holds it). Instead, `pthread_cond_timedwait` is used with a timeout equal to the exact remaining cooldown time, so the coder wakes itself up precisely when the dongle becomes available.

### Precise burnout detection
The monitor thread checks all coders every 1ms. When `ft_get_time() - last_compile_start > time_to_burnout`, the coder is marked as burned out, the log is printed, and the simulation is shut down. The 1ms polling interval ensures the burnout message appears within the required 10ms window.

### Log serialization
All log output goes through `ft_print_log`, which locks `print_mutex` before calling `printf` and unlocks after. This guarantees no two messages ever interleave on the same line, regardless of how many threads write concurrently.

## Thread synchronization mechanisms

### `pthread_mutex_t` — dongle mutex
Each dongle has its own `mutex`. It is locked at the start of `ft_acquire` and unlocked at the end. While the mutex is held, the coder inserts itself into the dongle's priority queue and waits for its turn. This prevents race conditions on the heap and the `released_at` timestamp.

### `pthread_mutex_t` — print mutex
A single `print_mutex` in `t_simulation` serializes all calls to `printf`. Any thread that wants to log must lock this mutex first.

### `pthread_mutex_t` — running mutex
The `running` flag in `t_simulation` is read and written by multiple threads (monitor sets it to 0, coders read it). A dedicated `running_mutex` protects all accesses to this flag, preventing torn reads and writes.

### `pthread_cond_t` — per-coder condition variable
Each coder has a `pthread_cond_t cond`. When a coder cannot acquire a dongle (either not at `heap[0]` or dongle is in cooldown), it releases the dongle mutex and sleeps on its own condition variable. When a dongle is released (`ft_fifo_release` / `ft_edf_release`), the coder at `heap[0]` is signaled to wake up and retry. When the simulation stops, the monitor signals all coders to unblock them.

### `pthread_cond_timedwait` — cooldown awareness
When a coder is at `heap[0]` but the dongle is still in cooldown, `pthread_cond_timedwait` is used instead of `pthread_cond_wait`. The timeout is set to the exact moment the cooldown expires (`current_time + remaining_ms`), so the coder wakes up precisely when it can proceed — without relying on an external signal that will never come.

### Monitor thread
A dedicated monitor thread is created with `pthread_create` and runs `ft_monitor`. It polls coder state every 1ms independently of the coder threads. When it detects burnout or completion, it sets `running = 0` (under `running_mutex`) and signals all coder condition variables to unblock any waiting threads.

## Resources

- [POSIX Threads Programming — Lawrence Livermore](https://hpc-tutorials.llnl.gov/posix/)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_cond_timedwait — Linux man page](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)

### AI usage

AI (Claude and ChatGPT) was used as a teaching assistant throughout this project. It explained concepts (heap data structures, EDF scheduling, the pthread API, and cooldown liveness bugs), reviewed code that I wrote, identified bugs and explained why they were incorrect, and guided me through the fixes step by step. I wrote all the code myself; the AI did not write any production code directly.