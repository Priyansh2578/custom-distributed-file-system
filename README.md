# Custom Distributed File System (CDFS)

##  Description
This project is a Custom Distributed File System built in C using a client-server architecture. It allows storing and retrieving files through network communication.

---

##  Features
- TCP client-server communication
- File storage and retrieval system
- In-memory hashmap for fast lookup
- B-Tree based indexing concept
- Disk persistence support

---

## 🛠️Tech Stack
- Language: C
- Concepts: Data Structures, Networking
- Environment: Linux

---

##  How to Run

### Step 1: Compile Server
gcc server.c -o server

### Step 2: Run Server
./server

### Step 3: Compile Client
gcc client.c -o client

### Step 4: Run Client
./client

---

##  Project Flow
1. Client sends request to server  
2. Server processes the request  
3. Data is stored/retrieved from memory/disk  
4. Response is sent back to client  

---

##  Learning Outcome
- Understanding of system-level programming  
- Hands-on experience with sockets and networking  
- Implementation of data structures in real-world systems  

---

##  GitHub Repository
https://github.com/Priyansh2578/custom-distributed-file-system
