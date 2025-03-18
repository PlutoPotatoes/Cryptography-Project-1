# Cryptography-Project-1
Cryptography Project 1

This project should function as intended with the grading script.
However, just to be careful it works as follows after running make:

./proj <operation> <filename> > <output filename>

Where operation is one of the following {hexdump, enc-base64, dec-base64}.

Unfortunately I was having difficulties with the EOF signifier when using dec-base64.
Meaning there may be extra characters added onto the end of the decoded file when the file was created on a windows device.

Created by Ryan and Danny
