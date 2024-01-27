#!/bin/bash
# DESCRIPTION: Wrap a verilator call to run a docker container
#
# Copyright 2020 by Stefan Wallentowitz. This program is free software; you
# can redistribute it and/or modify it under the terms of either the GNU
# Lesser General Public License Version 3 or the Perl Artistic License
# Version 2.0.
# SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0

# Pull newest image 
docker pull verilator/verilator:4.038 > /dev/null

# Get id of container
id=$(docker ps -aqf "name=verilator")

# When no user is specified, the default user is root. This makes it so files
# created by docker are owned by root, which we do not want. The solution is to
# add a user identical to ourselves in the docker environment. The three following
# lines do this.
# 
# Solution found here: 
# https://stackoverflow.com/questions/67995208/docker-username-interactive-result-in-i-have-no-name-error

docker cp $id:/etc/passwd /tmp > /dev/null
echo "orbit:x:""$(id -u)"":""$(id -g)""orbit:/tmp:/bin/bash" >> /tmp/passwd
docker cp /tmp/passwd $id:/etc/passwd > /dev/null

# Get the directory this script is located
root_dir=$(realpath $3)

# Run the docker image with the user we "made" above
docker run -e CCACHE_DIR=/work/.ccache -ti -v ${root_dir}:/work -w /work/$2 --user $(id -u):$(id -g) verilator/verilator:$1 "${@:4}"
