# Design Documentation

## Scope

This document describes the internal design and behavior of the diagnostic logger, including data structures, indexing strategy, invariants, and implementation considerations.

It is intended for developers modifying or extending the module.

---

## Table of Contents

* [Data Structures](#data-structures)
* [Logical and Physical Indexing](#logical-and-physical-indexing)
* [Buffer State and Invariants](#buffer-state-and-invariants)
* [Write Operation](#write-operation)
* [Read Operation](#read-operation)
* [String Handling](#string-handling)
* [Error Handling](#error-handling)
* [Implementation Constraints](#implementation-constraints)
* [Known Risks](#known-risks)

## Data Structures

### `LogEntry`

Represents a single log entry.

* `uint32_t timestamp` — caller-provided value
* `LogLevel level` — severity level
* `char message[LOGGER_MESSAGE_MAX]` — fixed-size message buffer

---

### `Logger`

Maintains the internal state of the logger.

* `LogEntry entries[LOGGER_CAPACITY]` — fixed-size storage
* `uint32_t head` — free-running logical write counter
* `uint32_t tail` — free-running logical read counter (oldest entry)

---

## Logical and Physical Indexing

The logger uses **free-running logical counters** (`head`, `tail`) to track orderings.
These counters are not bounded by the buffer size.

To access the underlying array, logical positions are converted into physical indices:

```c
physical_index = logical_index & (LOGGER_CAPACITY - 1);
```

This requires `LOGGER_CAPACITY` to be a power of two.

---

## Buffer State and Invariants

### Entry Count

The number of stored entries is derived from the logical counters:

```c
count = head - tail;
```

### Valid States

* Empty: `head == tail`
* Full: `count == LOGGER_CAPACITY`
* Partially filled: `0 < count < LOGGER_CAPACITY`

### Invariant

The following condition must always be maintained:

```c
head - tail <= LOG_CAPACITY;
```
This invariant ensures that the buffer does not exceed its fixed capacity.

Equivalently:

```c
count <= LOG_CAPACITY;
```

Violating this invariant results in undefined behavior.

---

## Write Operation

When adding a log entry:

1. If the buffer is full, increment `tail` to discard the oldest entry
2. Compute the physical index from `head`
3. Write the entry into the buffer
4. Increment `head`

This ensures:

* constant-time insertion
* overwrite-on-full behavior
* preservation of ordering

---

## Read Operation

To retrieve an entry:

1. Compute `count = head - tail`
2. Validate that the requested logical index (`index`) is less than `count`
3. Compute logical position: `tail + index`
4. Convert to physical index using bit masking
5. Return the entry

### Index Mapping

The `index` parameter represents the logical position relative to the oldest entry:

* `index = 0` → oldest entry
* `index = count - 1` → newest entry

---

## String Handling

* Copy at most `LOGGER_MESSAGE_MAX - 1` characters from the input
* Explicitly set the last byte to `'\0'`
* Truncate input if it exceeds buffer capacity

This guarantees:

* no buffer overflow
* valid null-terminated strings
* deterministic memory usage

---

## Error Handling

The API returns a `LoggerStatus` value.

### Validation

Functions validate:

* `logger` pointer
* `message` pointer (for write)
* `outEntry` pointer (for read)
* index bounds

### Non-validated Inputs

* Timestamp values are accepted without validation
* Message content is not interpreted

---

## Implementation Constraints

* No dynamic memory allocation
* Single-threaded usage
* Buffer capacity must be power of two (for efficient index masking)
* Logical counters (`head`, `tail`) use unsigned integer wraparound semantics
* Counter type must satisfy: `capacity < (max_value_of_counter / 2)` to ensure modular subtraction remains unabiguous
* Invariant must always hold: `head - tail <= capacity`

---

## Known Risks

* Off-by-one errors in index validation
* Misinterpreting logical counters as physical indices
* Breaking the invariant `head - tail <= LOGGER_CAPACITY`
* Misconfiguring buffer size (non-power-of-two)
* Unsafe string handling if constraints are violated
* Ambiguous or inconsistent error handling
