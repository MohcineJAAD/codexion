*This project has been created as part of the 42 curriculum by mjaad.*
*This project has been created as part of the 42 curriculum by mjaad.*
 
# Codexion
 
## Description
 
Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. `number_of_coders` coders sit in a circle around a shared Quantum Compiler, each with a USB dongle on their left and one on their right (as many dongles as coders).
 
Each coder cycles through three phases: **compiling**, **debugging**, and **refactoring**. To compile, a coder must acquire both neighboring dongles simultaneously. Once compiling is finished, both dongles are released, and the coder debugs, then refactors, then immediately tries to compile again.
 
A coder who fails to start compiling within `time_to_burnout` milliseconds of their last compile (or of the simulation start) **burns out**, which stops the whole simulation. The simulation also stops successfully once every coder has compiled at least `number_of_compiles_required` times.
 
Two scheduling policies decide who gets served next when multiple coders are waiting:
- **FIFO** — served in strict arrival order.
- **EDF** (Earliest Deadline First) — served by closest burnout deadline (`last_compile_start + time_to_burnout`), with coder ID as a tie-breaker.
Dongle pairs are granted **atomically** by a dedicated scheduler thread, rather than coders acquiring their two dongles one at a time. The goal of the project is to build correct, deadlock-free, starvation-free concurrent code in C using only pthreads primitives — no higher-level concurrency libraries, no libft, and a hand-rolled priority queue (heap) to drive the scheduling.
 
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
 
## Architecture overview
 
Three kinds of threads run concurrently:
 
- **Coder threads** (one per coder) — cycle through compile/debug/refactor. To compile, a coder registers itself on a single shared priority queue and sleeps until granted both dongles.
- **Scheduler thread** — the only thread that ever marks a dongle as taken. It repeatedly scans the shared queue and grants any coder both of its dongles atomically, in one step, whenever both are free and out of cooldown.
- **Monitor thread** — watches every coder's burnout deadline and stops the simulation the moment one is missed, or once every coder has compiled enough times.
## Blocking cases handled
 
### Deadlock prevention
The classic Dining Philosophers deadlock — every coder holding one dongle and waiting forever on the other — requires a **hold-and-wait** situation: acquiring one resource while blocked waiting for a second. This project removes hold-and-wait entirely rather than working around it. A coder never holds a single dongle in isolation: the scheduler thread grants both dongles to a coder in one atomic step (`ft_can_take_dongles` checks both are free before `ft_get_dongles` marks both as taken, under a single lock). It is structurally impossible for a coder to hold exactly one dongle while waiting on the other, so the circular-wait condition (one of Coffman's four necessary conditions for deadlock) can never arise.
 
### Starvation prevention (FIFO / EDF)
Both scheduling policies are implemented as comparator functions on a single min-heap shared by all coders. Under FIFO, the coder that has waited longest (`enqueue_time`) is always tried first. Under EDF, the coder closest to burnout (`last_compile_start + time_to_burnout`) is always tried first, with coder ID breaking ties. Because the scheduler scans the **entire** queue on every pass — not just the front — a coder blocked only by its own dongles' cooldown never blocks the coder behind it in priority order from being served in the meantime.
 
### Cooldown handling
After release, a dongle is unavailable until `dongle_cooldown` ms have passed (`ft_cooldown_remaining`). The scheduler thread never busy-waits on cooldown expiry: it computes the soonest moment any dongle currently cooling down will become available (`ft_earliest_dongle_free`) and sleeps exactly that long via `pthread_cond_timedwait`, waking itself up precisely when a retry might succeed — without relying on an external signal that a passive cooldown expiry can't provide.
 
### Precise burnout detection
Rather than polling on a fixed interval, the monitor thread computes the single earliest burnout deadline across all coders (`ft_find_earliest_deadline`) and sleeps exactly until that instant via `pthread_cond_timedwait`. This keeps burnout detection accurate to well within the required 10ms window while avoiding unnecessary wakeups when no coder is close to its deadline.
 
### Log serialization
Every log line goes through `ft_print_log`, which locks a single `print_mutex` around the `printf` call. This guarantees two messages can never interleave on the same line, no matter how many threads log concurrently.
 
## Thread synchronization mechanisms
 
### `pthread_mutex_t` — per-dongle mutex
Each dongle owns its own mutex, guarding only that dongle's `taken` flag and `released_at` timestamp. Keeping this fine-grained (one lock per dongle, instead of one lock for all dongles) means releasing dongle A never blocks a thread that only needs to touch dongle B — contention stays local to whichever dongle is actually contested.
 
### `pthread_mutex_t` + `pthread_cond_t` — scheduler mutex and condition
`sched_mutex` protects the single shared priority queue and each coder's `can_compile` flag. `sched_cond` is the channel coders sleep on while waiting to be granted dongles, and the channel the scheduler thread itself sleeps on while waiting for new arrivals, releases, or cooldown expiries. A coder joining the queue, a dongle being released, and a grant being made all broadcast on `sched_cond`, so the scheduler thread never misses a state change it should react to.
 
### `pthread_mutex_t` — stats mutex
`last_compile_start` and `compile_count` are written by a coder's own thread and read by the monitor thread computing deadlines. `stats_mutex` protects every access to both, preventing a torn or stale read of a burnout deadline.
 
### `pthread_mutex_t` — print mutex
A single `print_mutex` in `t_simulation` serializes every `printf` call across all coder threads and the monitor thread, guaranteeing clean, non-interleaved log lines.
 
### `pthread_mutex_t` — running mutex
The `running` flag is written once by the monitor and read continuously by every coder thread. `running_mutex` protects all reads and writes to it, preventing a stale read from letting a coder continue past shutdown.
 
### `while`, not `if`, around every condition wait
Every `pthread_cond_wait` / `pthread_cond_timedwait` in this project is wrapped in a `while` loop re-checking the actual condition, never a plain `if`. This guards against two distinct issues: spurious wakeups (POSIX permits a condition wait to return without any real signal ever being sent) and broadcasts intended for a different waiter (since `sched_cond` is shared by every coder, a broadcast wakes all of them, and each one must re-check its own `can_compile` flag before proceeding).
 
### `pthread_cond_broadcast`, not `pthread_cond_signal`
The scheduler always broadcasts rather than signals, because multiple coders share `sched_cond` for entirely different reasons (one may have just been granted dongles, another may simply need to notice a new queue state). Broadcasting wakes every waiter; the `while` re-check ensures only the coder actually intended for a grant proceeds, while every other coder safely goes back to sleep.
 
### Monitor thread
A separate thread runs `ft_monitor`, sleeping until the earliest burnout deadline or until woken early, then checking every coder for burnout and for overall completion. On detecting either, it takes `running_mutex`, flips `running` to 0, and broadcasts `sched_cond` — this is the single, thread-safe hand-off point between "simulation running" and "simulation stopping" that every other thread observes via `ft_is_running`.

## Resources

- [POSIX Threads Programming — Lawrence Livermore](https://hpc-tutorials.llnl.gov/posix/)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_cond_timedwait — Linux man page](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)
- [Operating Systems Theory — Core Dump](https://www.youtube.com/watch?v=7ge7u5VUSbE&list=PL9vTTBa7QaQPdvEuMTqS9McY-ieaweU8M)


### AI usage

AI was used as a teaching assistant throughout this project. It explained concepts (heap data structures, EDF scheduling, the pthread API, and cooldown liveness bugs), reviewed code that I wrote, identified bugs and explained why they were incorrect, and guided me through the fixes step by step. I wrote all the code myself; the AI did not write any production code directly.