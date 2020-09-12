# EECS 281: Lab 1 - Music Sorting

For this lab, you will be familiarizing yourself with the `get_opt` function, as well as our Makefile and the autograder.  To accomplish this task, there are two files we provide for you: `sorting.h` and `lab1.cpp`.  There is nothing for you to do in `sorting.h`, but you need to have it in the same directory as `lab1.cpp` for the code to compile.  Make sure to step through `lab1.cpp` to find and complete all the TODO statements.  There is one test file for you to test your code on in the folder.

`lab1.cpp` contains a definition for a `MusicLibrary` object.  This object will read a CSV file, put the entries into a vector, and then sort the vector and print the top n songs based on several command line options that will be processed with `get_opt`.  Carefully read the comments and observe what is happening in the code; there are a lot of concepts contained in the functionality that will help you later in the course.

`sorting.h` contains the `Song` definition, as well as an overloaded `operator<<` for printing the songs.  Please look around the file, but there’s nothing that you have to do in the code.

Once you have completed the coding portion, take a look at the Makefile.  There are a few TODO statements in the Makefile as well.  If the Makefile is confusing, don’t worry; we will go over it in lab.  Once you have a grasp of how to use it, compile your code and use the command  `make fullsubmit`.

Finally, submit your code to the autograder.  When you run the make fullsubmit command in the Makefile, there will be a tarball in the directory that contains all of the files.  Go to the autograder, click ‘choose file’ under Lab 1, and find the tarball in your file system.  Then upload your submission and watch as the test cases run!

Make sure that all code files submitted to the autograder include the assignment identifier listed on the first page of this assignment.





