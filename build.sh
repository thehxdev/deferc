#!/usr/bin/env sh

CPU_ARCH="$(uname -m)"

log_run() {
	echo "[I] $@"
	$@
}

log_run cc -std=c99 -Og -g3 main.c "defer_${CPU_ARCH}.s"
