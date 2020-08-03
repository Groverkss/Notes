#!/bin/bash
ls | grep -E '.aux|.log|.pdf|.toc' | xargs rm -f
