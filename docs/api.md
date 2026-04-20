# API Reference

## Overview

This document describes the public interface for the diagnostic logger module.

All functions operate on a caller-provided `logger_t` instance and return a `logger_status_t` to indicate success or failure.

---

## Types

### `logger_status_t`

Represents the result of an API operation.

```c
typedef enum {
  LOGGER_OK = 0,
  LOGGER_ERR_NULL,
  LOGGER_ERR_INVALID_INDEX
} logger_status_t;
```

---

### `log_level_t`

Represents the severity of a log entry.

```c
typedef enum {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} log_level_t;
```

---

### `log_entry_t`

Represents a single log entry.

```c
typedef struct {
  uint32_t timestamp;
  log_level_t level;
  char message[LOG_MESSAGE_MAX];
} log_entry_t;
```

---

### `logger_t`

Represents the logger state.

```c
typedef struct {
  log_entry_t entries[LOG_CAPACITY];
  size_t head;
  size_t tail;
} logger_t;
```

---

## Functions

### `logger_init`

```c
void logger_init(logger_t *logger);
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

### `logger_log`

```c
logger_status_t logger_log(
  logger_t *logger,
  uint32_t timestamp,
  log_level_t level,
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

  
