# Project I: File Compression with Huffman

**- Name: El Mehdi Hamte - MrFall**  
**- Date: 26-11-2026**

---

## 1. Introduction

This project implements **file compression and decompression** using the **Huffman algorithm** in the C programming language.

The goal is to minimize the number of bits needed to represent characters by assigning shorter codes to frequent characters and longer codes to rare characters.

---

## 2. Huffman Algorithm Explanation

### **1. Character Frequency Calculation**
We count how many times each character appears in the input file.

### **2. Building the Huffman Tree**
- Leaves = characters and their frequencies  
- Internal nodes = sum of children  
- Always merge the **two smallest nodes first** using a Min-Heap

### **3. Generating Huffman Codes**
Each path from root to leaf generates a code:

- **Left → 0**  
- **Right → 1**

### **4. File Encoding**
- Replace each character with its Huffman code  
- Bits are grouped into bytes  
- Last byte may contain **padding** → stored in a `.meta` file  

### **5. File Decoding**
- Rebuild the Huffman tree from `codes.map`  
- Decode the compressed bit stream  
- Remove padding  
- Restore the original text exactly

---

## 3. Project Files and Their Functions

| File | Description |
|------|-------------|
| `huffman.h` | Node structure + Huffman function prototypes |
| `huffman.c` | Huffman tree creation, code generation, encode/decode logic |
| `minheap.h` | Heap structure and prototypes |
| `minheap.c` | Min-Heap implementation (insert, extract-min, build-heap) |
| `main.c` | CLI interface: `encode` and `decode` commands |
| `Makefile` | Automates compilation (`make`) |

---

## 4. How to Use the Program

### **1. Create your input file**
```bash
echo "ABRACADABRA" > input.txt
```

---

### **2. Compile the project**
```bash
gcc main.c huffman.c minheap.c -o huff
```

(or simply)

```bash
make
```

---

### **3. Compress a file**
```bash
./huff encode input.txt compressed.huf codes.map
```

This generates:
- `compressed.huf` → compressed binary file  
- `codes.map` → Huffman codes used  
- `compressed.huf.meta` → padding info  

---

### **4. Decompress**
```bash
./huff decode compressed.huf compressed.huf.meta codes.map mrfall.txt
```

---

### **5. Check output**
```bash
cat mrfall.txt
```

**Expected:**
```
ABRACADABRA
```

---

## 5. Test Results

| File | Original Size | Compressed Size | Compression Ratio |
|------|---------------|-----------------|------------------|
| `input.txt` (ABRACADABRA) | 11 bytes | 3 bytes | 72% saved |

> Compression becomes more effective with **larger and more repetitive input data**.

---

## 6. Educational Videos

Learn Huffman Coding visually:

[![Huffman Coding](https://img.youtube.com/vi/co4_ahEDCho/0.jpg)](https://www.youtube.com/watch?v=co4_ahEDCho)
