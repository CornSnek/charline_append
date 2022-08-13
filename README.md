# charline_append
Program that can add characters to the beginning and the end of each line.

(Copied from main.c file)
Usage: Appends characters from beginning to end of a file for use with other programs.
Outputs to same file name appending with .output

Syntax: input_file_name -(a/r) -bBeginCharacters -eEndingCharacters -BE(0/1)(0/1)

Meaning: a/r means add or remove characters at beginning and end for each new line.
-b -e means the characters to add/remove from the beginning/ending surrounded by quotes.
-b and -e can also be empty. -BE means to append characters from the (B)eginning and (E)nding of file (Default 11 = true true)

Example: glsl -a -b'"' -e'\n"' adds \" to the beginning and \n" to the end for each line.
-BE11 is the default, which also adds the characters at the beginning and ending of the file. It outputs the file to glsl.output

Example: example_file

    This file will
    have quotes and newline (\n)
    between each line

Using the command `example_file -a -b'"' -e'\n"'`

adds " and \n" to the file example_file.output as

    "This file will\n"
    "have quotes and newline (\n)\n"
    "between each line\n"

Note: `-BE11` is the default for the command above.