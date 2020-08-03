#!/bin/bash
ls | grep -E -v '.tex|.sh|.jpeg' | xargs rm -f
