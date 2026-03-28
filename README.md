# 🚀 Custom Distributed File System (CDFS)

## 📌 Overview
CDFS is a custom-built distributed file system implemented in C using a client-server architecture.  
It supports key-value storage with efficient data retrieval using advanced data structures.

Additionally, a Python-based API and web interface are built on top of the system to enable easy interaction through a browser.

---

## ⚙️ Core Features
- TCP client-server communication (C)
- In-memory HashMap for fast key lookup
- B-Tree indexing for structured storage
- Disk persistence support (AOF)
- Multi-layer architecture (System + API + UI)

---

## 🌐 Web Interface Features
- Send Ping request  
- SET key-value pairs  
- GET stored values  
- DELETE keys  
- Real-time response display  

---

## 🏗️ System Architecture

User (Browser)  
↓  
Web Interface (HTML/CSS)  
↓  
Python API (Flask)  
↓  
C Server (CDFS Core)  
↓  
Data Structures (HashMap + B-Tree)  

---

## ▶️ How to Run

### 1️⃣ Compile Server
```bash
gcc server.c btree.c hashmap.c -o server
