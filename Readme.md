## Project I: File Compression with Huffman

**- Name: El Mehdi Hamte - MrFall**

**- Date: 26-11-2026**

## **1. Introduction**

In this project, we developed a Java program that compresses and decompresses text files using the **Huffman algorithm**.  
The goal is to reduce the number of bits used to represent characters based on their frequency in the text.


## **2. Huffman Algorithm Explanation**

1. **Character Frequency Calculation**  
   - Count how many times each character appears in the original file.

2. **Building the Huffman Tree**  
   - Leaves represent characters with their frequencies.  
   - Internal nodes do not contain characters; they represent the sum of the child nodes’ frequencies.  
   - Nodes with the smallest frequencies are merged first.

3. **Generating Codes**  
   - The path from the root to each leaf provides the **Huffman code**:  
     **- Left → 0**
     
     **- Right → 1**

4. **File Encoding**  
   - Each character is replaced by its binary code.  
   - The last byte may contain **padding**, stored in a `.meta` file.

5. **File Decoding**  
   - Rebuild the tree from the code file.  
   - Read bits and convert them back to the original characters.



## **3. Project Files and Their Functions**

| File | Function |
|------|---------|
| `HuffmanNode.java` | Defines a node in the tree: character, frequency, children. Includes `isLeaf()` method. |
| `MinHeap.java` | Min-heap data structure for efficiently extracting minimum frequency nodes. Methods: `insert`, `extractMin`, `buildHeap`. |
| `HuffmanCoding.java` | Contains all algorithm methods: `buildFrequencyTable`, `buildHuffmanTree`, `generateCodes`, `encodeFile`, `decodeFile`, `readCodesFromFile`, `writeCodesToFile`, `rebuildTreeFromCodes`. |
| `Main.java` | Main program to run from the command line: `encode` and `decode` commands. |


## **4. Testing Methodology**

**1. Prepare a test file:**
```
echo "ABRACADABRA" > input.txt
```

**2. Compile all files:**
```
javac *.java
```

**3. Encode the file:**
```
java Main encode input.txt compressed.huf codes.map
```

**4. Decode the file:**
```
java Main decode compressed.huf compressed.huf.meta codes.map result.txt
```

**5. Verify the output:**
```
cat result.txt
```
> **The output should match the original text.**

## **5. Test Results**

| Original File       | Original Size | Compressed Size | Compression Ratio |
|-------------------|---------------|----------------|-----------------|
| `input.txt` (ABRACADABRA) | 11 bytes      | 3 bytes        | 72% saved       |

> **Note:** Compression efficiency depends on character frequency in the original file.


## **6. Conclusion** 

- Java program successfully compresses and decompresses text files using Huffman coding.  
- The program passed all tests with **efficient compression and no data loss**.  
- Code is **well-organized** and modular.  
- Additional **diagrams, charts, or videos** can enhance understanding.


## **7. Educational Videos** 

**Learn more about Huffman coding with this video:**

[![Huffman Coding Video](https://img.youtube.com/vi/co4_ahEDCho/0.jpg)](https://www.youtube.com/watch?v=co4_ahEDCho)
