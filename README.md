# Linux System Fetch & Analytics Utility

A lightweight, efficient C library and command-line utility designed for Linux systems to gather hardware specifications, disk usage data, and software package metrics. This project bypasses heavy external dependencies by interacting directly with Linux system files (`/proc`, `/sys`), POSIX APIs (`statvfs`), and system directories. Memory management is handled via a custom dynamic array implementation (`dynarray`).

## Features

* **CPU Information**: Parses `/proc/cpuinfo` and `/sys/devices/...` to extract the exact processor model name, total core/thread count, and maximum hardware frequency (with fallback parsing).
* **Disk Space Analytics**: Utilizes the POSIX `statvfs` API to calculate total space, free space, and actual available space for non-root users in Gigabytes (GB), including real-time percentage metrics.
* **Package Manager Tracking**:
    * **APT (Debian/Ubuntu)**: Parses `/var/lib/dpkg/status` to count fully installed native packages.
    * **Flatpak (Sandboxed apps)**: Scans system (`/var/lib/flatpak/app`) and user-level (`~/.local/share/flatpak/app`) directories to count installed Flatpak applications.
* **Custom Memory Management**: Integrates a dynamic array wrapper (`dynarray`) to manage structured data types and strings safely without memory leaks.

## Project Structure

```text
├── cpu_info.c / cpu_info.h       # CPU hardware metrics extraction
├── disk_info.c / disk_info.h     # Disk usage calculations via statvfs
├── apt.c / apt.h                 # APT package parser
├── flatpak.c / flatpak.h         # Flatpak application counter
├── dnf.h                         # Stub for RedHat/Fedora package manager support
└── dynarray/                     # Custom safe dynamic array library
```

## How It Works

### 1. Hardware Monitoring
Instead of executing heavy shell commands, the utility opens and streams low-level kernel interfaces directly:
* **CPU Model**: Extracts fields matching `"model name"`.
* **CPU Cores**: Counts active `"processor"` entries.
* **CPU Frequency**: Targets the hardware sysfs tree at `/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq` before falling back to `/proc/cpuinfo`.

### 2. File System Tracking
By capturing the `HOME` environment variable, the disk utility invokes `statvfs()` on the user's home partition to calculate file system block architectures:
* **Formula**: `Space (GB) = (f_blocks * f_frsize) / (1024 * 1024 * 1024)`

## Technical Implementation Details

* **Language**: C (C99/C11 standard compatible).
* **Safe Parsing**: Explicit boundary checks during string manipulations (`strncmp`, `strchr`, `sizeof` buffers) to mitigate buffer overflow risks.
* **Memory Safety**: Every structure allocation paired with explicit lifecycle cleanup functions (e.g., `free_cpu_info`, `free_disk_info`) wrapping the custom `dafree` routine.
* **Error Handling**: Full defensive checks on file descriptors (`fopen` validations) and environment variables (`getenv("HOME")` safety).

## Requirements

* **OS**: Linux (Ubuntu, Debian, Fedora, or any standard distribution).
* **Compiler**: `gcc` or `clang`.
