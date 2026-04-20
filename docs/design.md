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

* Copy at most `LOG_MESSAGE_MAX - 1` characters from the input
* Explicitly set the last byte to `'\0'`
* Truncate input if it exceeds buffer capacity

This guarantees:

* no buffer overflow
* valid null-terminated strings
* deterministic memory usage

---

## Error Handling

The API returns a `logger_status_t` value.

### Validation

Functions validate:

* `logger` pointer
* `message` pointer (for write)
* `out_entry` pointer (for read)
* index bounds

### Non-validated Inputs

* Timestamp values are accepted without validation
* Message content is not interpreted

---

## Implementation Constraints

* No dynamic memory allocation
* Single-threaded usage
* `LOG_CAPACITY` must be power of two
* `size_t` wraparound is assumed to be well-defined

---

## Known Risks

* Off-by-one errors in index validation
* Misinterpreting logical counters as physical indices
* Breaking the invariant `head - tail <= LOG_CAPACITY`
* Misconfiguring buffer size (non-power-of-two)
* Unsafe string handling if constraints are violated
* Ambiguous or inconsistent error handling
