# Scheduler

# Comp 737 Project

# Nirjhar Mukherjee

# https://github.com/nirjharm/Scheduler

# Introduction

This project aims to provide a set of tools to create task sets, execute them using a discrete
time step executor, and display the resulting schedule in an easy to understand, user-friendly
HTML graphical document. The tools also allow in depth customization of the types of tasks
created and the execution configurations.

# Running the Whole thing

Running the setup is fairly straightforward.

First pull the code from the git repo, and edit the **randomgen.cpp** file to your desirable state,
to produce the taskset.


Next run: (It automatically runs make)

```
./geninput.sh N //Here N is the # of tasksets you want, so for 1 set: ./geninput.sh 1
```
This generated N files, input0 through inputN-1 in the inputfiles folder

Finally, run:

**./run.sh inputX** // to draw the schedule for the Xth imput file, for eg: ./run.sh input

*Not inputfiles/input0, just the name of the schedule in the inputfiles folder, it can be xyzad
for all it cares

And then just open the Result.html created
