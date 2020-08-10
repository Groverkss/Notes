#!/bin/bash
ls | grep -E '.aux|.log|.toc|.out' | xargs rm -f
