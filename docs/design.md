# Design Documentation

## Scope

This document describes the internal design and behavior of the diagnostic logger, including data structures, indexing strategy, invariants, and implementation considerations.

It is intended for developers modifying or extending the module.

---

## Data Structures

### `log_entry_t`

Represents a single log entry.

* `uint32_t timestamp` — caller-provided value
* `log_level_t level` — severity level
* `char message[LOG_MESSAGE_MAX]` — fixed-size message buffer

---

### `logger_t`

Maintains the internal state of the logger.

* `log_entry_t entries[LOG_CAPACITY]` — fixed-size storage
* `size_t head` — free-running logical write counter
* `size_t tail` — free-running logical read counter (oldest entry)

---

## Logical and Physical Indexing

The logger uses **free-running logical counters** (`head`, `tail`) to track orderings.
These counters are not bounded by the buffer size.

To access the underlying array, logical positions are converted into physical indices:

```c
physical_index = logical_index & (LOG_CAPACITY - 1);
```

This requires `LOG_CAPACITY` to be a power of two.

---

## Buffer State and Invariants

### Entry Count

The number of stored entries is derived from the logical counters:

```c
count = head - tail;
```

### Valid States

* Empty: `head == tail`
* Full: `count == LOG_CAPACITY`
* Partially filled: `0 < count < LOG_CAPACITY`

### Invariant

The following condition must always be maintained:

```c
head - tail <= LOG_CAPACITY;
```
This invariant ensures that the buffer does not exceed its fixed capacity.

Equivalently:o I need to updateoIneedtoupdate

```c
count <= LOG_CAPACITY;
```

Violating this invariant results in undefined behavior.

---

