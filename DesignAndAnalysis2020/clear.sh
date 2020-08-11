#!/bin/bash
ls | grep -E '.aux|.log|.toc' | xargs rm -f
