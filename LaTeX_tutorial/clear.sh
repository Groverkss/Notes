#!/bin/bash
ls | grep -E '.aux|.log|.pdf' | xargs rm -f
