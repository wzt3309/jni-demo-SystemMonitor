#
# This file is used to record maven build command for the project
# And this project should to be build in linux-x64(use gcc)
#
# @Author ZeTao Wang
# @Date 2017-10-26
#

#
# clean this project by default (remove the /target )
# and remove hbdc-sysm-jni/target/lib storing *.so which
# compiled by this project
#

# **recommend**, use maven wrapper--->Using Apache Maven 3.5.2
./mvnw clean [-Dtest=true]
# if you have maven in PATH, you can also use this
# mvn clean [-Dtest=true]

#
# compile *.c sources to shared library libhbdcsysm-amd64-linux.so
# and then copy it to the hbdc-sysm-jni/target/lib
# (now we only use gcc in x64 linux platform, maybe we will cross platform
# in the future)
#

# **recommend**, use maven wrapper--->Using Apache Maven 3.5.2
./mvnw compile [-Dtest=true]
# if you have maven in PATH, you can also use this
# mvn compile [-Dtest=true]