# Diagnostic Logger

## Purpose

This project was developed to demonstrate low-level programming concepts in C, including:

* memory-safe data handling
* circular buffer (ring buffer) implementation
* API design and responsibility boundaries
* defensive programming practices
* test-driven development in a constrained environment

___

## Overview

This project implements a fixed-size diagnostic logging system in C, designed for embedded and resource-constrained environments.

The logger stores recent log entries in memory and overwrites the oldest entries when capacity is exceeded. The implementation prioritizes deterministic behavior, memory safety, and clear API boundaries.

---

## Table of Contents

* [Purpose](#purpose)
* [Overview](#overview)
* [Features](#features)
* [Design Summary](#design-summary)
* [Constraints](#constraints)
* [File Structure](#file-structure)
* [Usage](#usage)
* [Testing](#testing)
* [Documentation](#documentation)
* [Future Improvements](#future-improvements)

---

## Features

* Fixed-size in-memory logger
* Overwrite-on-full behavior
* Chronological retrieval (oldest to newest)
* Safe string handling with truncation and null termination
* Consistent error handling
* Portable, standard C implementation

---

## Design Summary

The logger is implemented using a fixed-size circular buffer.

### Key Concepts

* **Head (write position):** Free-running logical counter for the next write
* **Tail (read position):** Free-running logical counter for the oldest entry
* **Logical vs Physical Indexing:** Logical positions are mapped to the underlying buffer using efficient index wrapping (see [design documentation](docs/design.md#logical-and-physical-indexing) for details)

### Indexing Strategy

The buffer capacity must be a power of two:

```c
physical_index = logical_index & (LOG_CAPACITY - 1);
```

### Entry Count

```c
count = head - tail;
```

The buffer is:
* **Empty:** `head == tail`
* **Full:** `count == LOG_CAPACITY`

For detailed implementation and invariants, see [docs/design.md](docs/design.md).

---

## Constraints

* No dynamic memory allocation
* Single-threaded usage (version 1)
* Buffer capacity must be a power of two
* Message length is fixed and truncated if necessary
* Timestamp values are provided by the caller

---

## File Structure

```
logger.h          Public API and data structures
logger.c          Implementation
test_logger.c     Test harness
docs/             Detailed documentation
```
---

## Usage

### Initialization

```c
logger_t logger;
logger_init(&logger);
```

### Add log entry

```c
logger_log(&logger, timestamp, LOG_INFO, "System started");
```

### Retrieve Entry

```c
log_entry_t entry;
if (logger_get_entry(&logger, index, &entry) == LOGGER_OK) {
    // use entry
} else {
    // handle error
}
```

---

## Testing

The test harness validates:

* Initialization
* Insertion
* Buffer wrapping
* Chronological ordering
* String truncation
* Error handling

---

## Documentation

Detailed documentation is available in the `docs/` directory:

* `docs/design.md` — internal design decisions and structure
* 

---

## Future Improvements

* Thread-safe version
* Configurable buffer size
* Formatted logging support (`printf`-style)
* Severity filtering
* Persistant storage
* Structured logging formats (binary or JSON)
