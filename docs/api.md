# API Reference

## Overview

This document describes the public interface for the diagnostic logger module.

All functions operate on a caller-provided `Logger` instance and return a `LoggerStatus` to indicate success or failure.

---

## Types

### `LoggerStatus`

Represents the result of an API operation.

```c
typedef enum {
  LOGGER_OK = 0,
  LOGGER_ERR_NULL,
  LOGGER_ERR_INVALID_INDEX
} LoggerStatus;
```

---

### `LogLevel`

Represents the severity of a log entry.

```c
typedef enum {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR
} LogLevel;
```

---

### `LogEntry`

Represents a single log entry.

```c
typedef struct {
  uint32_t timestamp;
  LogLevel level;
  char message[LOG_MESSAGE_MAX];
} LogEntry;
```

---

### `Logger`

Represents the logger state.

```c
typedef struct {
  LogEntry entries[LOG_CAPACITY];
  size_t head;
  size_t tail;
} Logger;
```

---

## Functions

### `loggerInit`

```c
void loggerInit(Logger *logger);
```

Initializes the logger state.

**Parameters:**

* `logger` — pointer to logger instance

**Behavior:**

* Sets `head` and `tail` to zero
* Clears internal state as needed

**Notes:**

* The caller must provide valid memory for `logger`
* Behavior is undefined if `logger` is `NULL`

---

### `loggerAppend`

```c
LoggerStatus loggerAppend(
  Logger *logger,
  uint32_t timestamp,
  LogLevel level,
  const char *message
);
```

Adds a log entry.

**Parameters:**

* `logger` — logger instance
* `timestamp` — caller-provided time value
* `level` — severity
* `message` — null-terminated string

**Returns:**

* `LOGGER_OK` on success
* `LOGGER_ERR_NULL` if `logger` or `message` is `NULL`

**Behavior:**

* Writes entry at the current write position
* Overwrites the oldest entry if the buffer is full
* Truncates message if it exceeds buffer size
* Ensures null termination

---

### `loggerGetEntry`

```c
LoggerStatus loggerGetEntry(
  const Logger *logger,
  size_t index,
  LogEntry *out_entry
);
```

Retrieves a log entry by logical index.

**Parameters:**

* `logger` — logger instance
* `index` — logical index relative to the oldest entry
* `out_entry` — output buffer

**Returns:**

* `LOGGER_OK` on success
* `LOGGER_ERR_NULL` if `logger` or `out_entry` is `NULL`
* `LOGGER_ERR_INVALID_INDEX` if `index` is out of range

**Index Semantics:**

* `index = 0` → oldest entry
* `index = logger_count(...) - 1` → newest entry

**Notes:**

* `index` refers to a logical position, not a physical array index

---

### `loggerGetCount`

```c
size_t loggerGetCount(const Logger *logger);
```

Returns the number of stored entries.

**Parameters:**

* `logger` — logger instance

**Returns:**

* Number of valid entries
* Returns `0` of `logger` is `NULL`

**Behavior:**

* Count is derived from internal state (`head - tail`)
* Does not modify logger state

---

### `loggerClear`

```c
void logger_clear(Logger *logger);
```

Clears all stored entries.

**Parameters:**

* `logger` — logger instance

**Behaviors:**

* Resets `head` and `tail`
* Effectively removes all entries

**Notes:**

* Behavior is undefined if `logger` is `NULL`
