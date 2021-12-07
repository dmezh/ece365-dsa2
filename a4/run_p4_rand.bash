#!/bin/bash

in="test_rands\ntestrs\n"

./p4 < <(echo -e $in)

diff testrs testr_corrects
