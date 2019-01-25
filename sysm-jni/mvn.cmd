#
# This file is used to record maven build command for the project
#
# @Author ZeTao Wang
# @Date 2017-10-26
#

#
# clean this project by default (remove the /target )
#

# **recommend**, use maven wrapper--->Using Apache Maven 3.5.2
./mvnw clean
# if you have maven in PATH, you can also use this
# mvn clean

#
# compile all "*.class" and generate a "javasysm_generated.h" file
# and then output the header file to the hbdc-sysm-native/include
#

# **recommend**, use maven wrapper--->Using Apache Maven 3.5.2
./mvnw compile
# if you have maven in PATH, you can also use this
# mvn compile
