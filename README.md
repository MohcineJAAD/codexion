# Codexion
 
## Description
 
Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. `number_of_coders` coders sit in a circle around a shared Quantum Compiler, each with a USB dongle on their left and one on their right (as many dongles as coders).
 
Each coder cycles through three phases: **compiling**, **debugging**, and **refactoring**. To compile, a coder must acquire both neighboring dongles simultaneously. Once compiling is finished, both dongles are released, and the coder debugs, then refactors, then immediately tries to compile again.
 
A coder who fails to start compiling within `time_to_burnout` milliseconds of their last compile (or of the simulation start) **burns out**, which stops the whole simulation. The simulation also stops successfully once every coder has compiled at least `number_of_compiles_required` times.
 
Two scheduling policies decide who gets a contested dongle:
- **FIFO** — served in strict arrival order.
- **EDF** (Earliest Deadline First) — served by closest burnout deadline (`last_compile_start + time_to_burnout`), with coder ID as a tie-breaker.
The goal of the project is to build correct, deadlock-free, starvation-free concurrent code in C using only pthreads primitives — no higher-level concurrency libraries, no libft, and a hand-rolled priority queue (heap) to drive the scheduling.
 
## Instructions
 
### Compilation
 
```bash
make
```
 
### Execution
 
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
 
All arguments are mandatory and must be non-negative integers (except `scheduler`). `scheduler` must be exactly `fifo` or `edf`. Invalid input (negative numbers, non-integers, overflow, or an unknown scheduler) is rejected.
 
### Arguments
 
| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders and dongles |
| `time_to_burnout` | Max ms a coder can go without starting to compile |
| `time_to_compile` | Ms spent compiling (holds 2 dongles) |
| `time_to_debug` | Ms spent debugging |
| `time_to_refactor` | Ms spent refactoring |
| `number_of_compiles_required` | Compiles needed per coder to end successfully |
| `dongle_cooldown` | Ms a dongle stays unavailable after being released |
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
 
Actions: `has taken a dongle`, `is compiling`, `is debugging`, `is refactoring`, `burned out`. Each state change is logged exactly once, in serialized order, with no interleaving between lines.
 
## Blocking cases handled
 
### Deadlock prevention (Coffman's conditions)
The classic deadlock scenario — every coder holding their left dongle and waiting forever on the right one — is a circular wait, one of Coffman's four necessary conditions for deadlock. It's broken by making the last coder (`id == number_of_coders`) acquire dongles in reverse order (right, then left). This removes the cycle in the wait-for graph: it's no longer possible for every coder to simultaneously hold one dongle and wait on the next.
 
### Starvation prevention (FIFO)
Coders are served strictly in arrival order via a min-heap ordered by `enqueue_time`. The coder who has been waiting longest is always at `heap[0]` and always served next — no coder can be skipped indefinitely.
 
### Starvation prevention (EDF)
The coder closest to burnout is always served first. Because a coder's deadline gets more urgent the longer it waits, it naturally rises to the front of the queue over time. Liveness (no starvation) is guaranteed as long as the given parameters are feasible for the ring topology.
 
### Cooldown handling
After release, a dongle is unavailable until `dongle_cooldown` ms have passed. A coder at the front of the queue (`heap[0]`) who finds the dongle still cooling down must not call `pthread_cond_wait`, since no other thread will signal it once the cooldown naturally expires — nobody else holds the dongle. Instead, `pthread_cond_timedwait` is used with a timeout equal to the exact remaining cooldown, so the coder wakes itself up precisely when the dongle becomes usable.
 
### Double-acquisition race
A `taken` flag on each dongle prevents a second coder from grabbing a dongle the instant the first coder is dequeued from the heap but hasn't yet updated shared state, closing a race window that plain heap ordering alone doesn't cover.
 
### Precise burnout detection
A dedicated monitor thread polls every coder's `last_compile_start` every 1ms. As soon as `ft_get_time() - last_compile_start > time_to_burnout`, the burnout is logged and the simulation shuts down. The 1ms polling interval keeps the log comfortably within the required 10ms detection window.
 
### Log serialization
Every log line goes through `ft_print_log`, which locks a single `print_mutex` around the `printf` call. This guarantees two messages can never interleave on the same line, no matter how many threads log concurrently.
 
## Thread synchronization mechanisms
 
### `pthread_mutex_t` — per-dongle mutex
Each dongle owns its own mutex, held while a coder inserts itself into that dongle's priority queue, checks cooldown state, or extracts itself once granted. This confines contention to a single dongle instead of serializing the whole simulation, and prevents races on the heap and on `released_at`.
 
### `pthread_mutex_t` — print mutex
A single `print_mutex` in `t_simulation` serializes every `printf` call across all coder threads and the monitor thread, guaranteeing clean, non-interleaved log lines.
 
### `pthread_mutex_t` — running mutex
The `running` flag is written once by the monitor and read continuously by every coder thread. `running_mutex` protects all reads and writes to it, preventing a torn or stale read from letting a coder continue past shutdown.
 
### `pthread_cond_t` — per-coder condition variable
Each coder has its own `cond`. When a coder isn't at `heap[0]` for its dongle, it waits on this condition variable, releasing the dongle mutex while asleep. When a dongle is released, the coder now at `heap[0]` is broadcast-woken to retry. On shutdown, the monitor broadcasts every coder's condition variable so no thread is left blocked forever.
 
`pthread_cond_broadcast` is used instead of `pthread_cond_signal` deliberately: it wakes every waiter, but the `while` loop around the wait re-checks `heap[0]` each time, so only the coder the scheduler actually intends to grant the dongle to proceeds — the scheduling *policy*, not the kernel's wakeup order, decides who gets in.
 
### `pthread_cond_timedwait` — cooldown-aware waiting
When the head-of-queue coder is blocked only by cooldown (not by another coder ahead of it), `pthread_cond_timedwait` is used with a deadline set to the exact cooldown expiry. This avoids the coder oversleeping (which would happen with a plain sleep) or hanging forever (which would happen with `pthread_cond_wait`, since nothing signals a cooldown expiry).
 
### Monitor thread
A separate thread runs `ft_monitor`, polling burnout and completion conditions independently of the coder threads. On detecting either, it takes `running_mutex`, flips `running` to 0, and broadcasts every coder's condition variable — this is the single, thread-safe hand-off point between "simulation running" and "simulation stopping" that every other thread observes via `ft_is_running`.
 
### Race condition example
Without `running_mutex`, a coder thread could read `running` mid-write by the monitor and observe a torn value on some architectures, or simply race on visibility without a memory barrier — potentially compiling one extra unnecessary cycle after shutdown was requested. Locking both the write (in the monitor) and every read (in `ft_is_running`) around the same mutex enforces a consistent, single view of that flag across threads.

## Resources

- [POSIX Threads Programming — Lawrence Livermore](https://hpc-tutorials.llnl.gov/posix/)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_cond_timedwait — Linux man page](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)
- [Operating Systems Theory — Core Dump](https://www.youtube.com/watch?v=7ge7u5VUSbE&list=PL9vTTBa7QaQPdvEuMTqS9McY-ieaweU8M)


### AI usage

AI was used as a teaching assistant throughout this project. It explained concepts (heap data structures, EDF scheduling, the pthread API, and cooldown liveness bugs), reviewed code that I wrote, identified bugs and explained why they were incorrect, and guided me through the fixes step by step. I wrote all the code myself; the AI did not write any production code directly.