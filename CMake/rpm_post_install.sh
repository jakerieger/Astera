#!/bin/sh
# RPM post-install script for NthEngine
# This script runs ldconfig to update the shared library cache

if [ -x /sbin/ldconfig ]; then
    /sbin/ldconfig
fi
