# Testing Strategy

## Overview

This document outlines the testing approach for the diagnostic logger.

Testing is performed using a simple C test harness and focuses on correctness, safety, and adherence to the defined API contract.

---

## Table of Contents

* [Testing Goals](#testing-goals)
* [Test Categories](#test-categories)
* [Invariant Testing](#invariant-testing)
* [Test Approach](#test-approach)
* [Future Improvements](#future-improvements)

---

## Testing Goals

* Verify correct behavior under normal usage
* Validate edge cases and boundary conditions
* Ensure memory safety
* Confirm logical ordering of entries
* Validate API error handling

---

## Test Categories

### Initialization

* Logger initializes with:
  * `count == 0`
  * `head == tail`
* Buffer is empty after initialization

---

### Insertion

* Single entry is stored correctly
* Multiple entries maintain correct order
* Entry data (timestamp, level, message) is preserved

---

### Capacity Handling

* Buffer fills to `LOGGER_CAPACITY`
* `loggerGetCount()` returns correct values
* No overflow occurs

---

### Overwrite Behavior

* Inserting beyond capacity:
  * Oldest entry is overwritten
  * New entries are retained
* Logical ordering remains correct after wrap

---

### Retrieval

* Valid indices return correct entries
* Invalid indices return `LOGGER_ERR_INVALID_INDEX`
* Logical mapping is preserved:
  * `index = 0` → oldest entry
  * `index = count - 1` → newest entry

---

### Logical vs Physical Indexing

* Retrieval uses logical indexing relative to `tail`
* Physical array layout does not affect returned order
* Correct behavior is maintained after wrapping

---

### Count Validation

* `loggerGetCount()` returns:
  * `0` after initialization
  * correct value after inserts
  * capped value at full capacity
* Count remains bounded by `LOGGER_CAPACITY`

---

### String Handling

* Short messages are stored unchanged
* Messages equal to buffer size are handled correctly
* Overlong messages are truncated safely
* Stored messages are always null-terminated

---

### Error Handling

* `NULL` Errors:
  * `LOGGER_ERR_NULL_LOGGER` returned for `NULL` logger
  * `LOGGER_ERR_NULL_MESSAGE` returned for `NULL` message (write)
  * `LOGGER_ERR_NULL_ENTRY` returned for `NULL` output buffer (read)
  * `LOGGER_ERR_NULL_COUNT` returned for `NULL` output count (read)
* `LOGGER_ERR_INVALID_INDEX` returned for:
  * index out of range
* `LOGGER_ERR_INVALID_LEVEL` returned for:
  * invalid log level

---

## Invariant Testing

Tests should verify that the following condition is always maintained:

```c
head - tail <= LOGGER_CAPACITY;
```
Violations indicate incorrect buffer management.

---

## Test Approach

* Use direct function calls
* Validate results with assertations or conditional checks
* Output results for visibility during development

---

## Future Improvements

* Introduce automated test validation
* Integrate with a unit testing framework
* Add CI-based test execution
