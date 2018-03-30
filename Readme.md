**In-Memory vs Disk-Based Searching**

Goal: To investigate time efficiency issues associated with in-memory versus disk-based searching strategies.

*Search Modes*
- In-Memory Linear Search
- In-Memory Binary Search
- On-Disk Linear Search
- On-Disk Binary Search

*Program Execution*

`
cc main.c
`

`
./a.out searchMode keyFileName seekFileName
`

Example: 

`./a.out --disk-lin key.db seek.db`

Expected output for key.db and seek.db is in output.txt (only the time varies for different search modes)