#!/bin/bash

fusermount -u to
rm -f *.log

./main "from/" to/
