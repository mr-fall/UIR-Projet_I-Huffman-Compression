import java.io.*;
import java.util.*;

public class HuffmanCoding {

    public static Map<Character, Integer> buildFrequencyTable(String inputFile) throws IOException {
        Map<Character, Integer> freq = new HashMap<>();
        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFile))) {
            int b;
            while ((b = bis.read()) != -1) {
                char c = (char) b;
                freq.put(c, freq.getOrDefault(c, 0) + 1);
            }
        }
        return freq;
    }

    public static HuffmanNode[] buildNodesArray(Map<Character, Integer> freq) {
        HuffmanNode[] nodes = new HuffmanNode[freq.size()];
        int idx = 0;
        for (Map.Entry<Character, Integer> e : freq.entrySet())
            nodes[idx++] = new HuffmanNode(e.getKey(), e.getValue());
        return nodes;
    }

    public static HuffmanNode buildHuffmanTree(Map<Character, Integer> freq) {
        if (freq.isEmpty()) return null;
        HuffmanNode[] nodes = buildNodesArray(freq);
        MinHeap heap = MinHeap.buildHeap(nodes, nodes.length);

        if (heap.getSize() == 1) {
            HuffmanNode only = heap.extractMin();
            return new HuffmanNode(only.frequency, only, null);
        }

        while (heap.getSize() > 1) {
            HuffmanNode x = heap.extractMin();
            HuffmanNode y = heap.extractMin();
            heap.insert(new HuffmanNode(x.frequency + y.frequency, x, y));
        }
        return heap.extractMin();
    }

    public static void generateCodes(HuffmanNode root, String prefix, Map<Character, String> codeMap) {
        if (root == null) return;
        if (root.isLeaf()) {
            codeMap.put(root.character, prefix.length() > 0 ? prefix : "0");
            return;
        }
        generateCodes(root.left, prefix + "0", codeMap);
        generateCodes(root.right, prefix + "1", codeMap);
    }

    public static void writeCodesToFile(Map<Character, String> codeMap, String codesFile) throws IOException {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(codesFile))) {
            for (Map.Entry<Character, String> e : codeMap.entrySet())
                bw.write((int)(char)e.getKey() + "\t" + e.getValue() + "\n");
        }
    }

    public static Map<Character, String> readCodesFromFile(String codesFile) throws IOException {
        Map<Character, String> codeMap = new HashMap<>();
        try (BufferedReader br = new BufferedReader(new FileReader(codesFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split("\t");
                char c = (char) Integer.parseInt(parts[0]);
                codeMap.put(c, parts[1]);
            }
        }
        return codeMap;
    }

    // Encode file using bits, handling padding correctly
    public static void encodeFile(String inputFile, String outputFile, Map<Character, String> codeMap) throws IOException {
        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFile));
             FileOutputStream fos = new FileOutputStream(outputFile)) {

            StringBuilder bitBuffer = new StringBuilder();
            int b;
            while ((b = bis.read()) != -1) {
                char c = (char) b;
                bitBuffer.append(codeMap.get(c));

                while (bitBuffer.length() >= 8) {
                    fos.write((byte) Integer.parseInt(bitBuffer.substring(0, 8), 2));
                    bitBuffer.delete(0, 8);
                }
            }

            int remainingBits = bitBuffer.length();
            if (remainingBits > 0) {
                while (bitBuffer.length() < 8) bitBuffer.append('0');
                fos.write((byte) Integer.parseInt(bitBuffer.toString(), 2));
            }

            try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile + ".meta"))) {
                bw.write(Integer.toString(remainingBits > 0 ? remainingBits : 8));
            }
        }
    }

    // Rebuild Huffman tree from codes
    public static HuffmanNode rebuildTreeFromCodes(Map<Character, String> codeMap) {
        HuffmanNode root = new HuffmanNode('\0', 0);
        for (Map.Entry<Character, String> e : codeMap.entrySet()) {
            char c = e.getKey();
            String code = e.getValue();
            HuffmanNode cur = root;
            for (char bit : code.toCharArray()) {
                if (bit == '0') {
                    if (cur.left == null) cur.left = new HuffmanNode('\0', 0);
                    cur = cur.left;
                } else {
                    if (cur.right == null) cur.right = new HuffmanNode('\0', 0);
                    cur = cur.right;
                }
            }
            cur.character = c;
        }
        return root;
    }

    // Decode file using the meta info for last byte
    public static void decodeFile(String compressedFile, String metaFile, String codesFile, String outputFile) throws IOException {
        Map<Character, String> codeMap = readCodesFromFile(codesFile);
        HuffmanNode root = rebuildTreeFromCodes(codeMap);

        int lastBits = 8;
        try (BufferedReader br = new BufferedReader(new FileReader(metaFile))) {
            lastBits = Integer.parseInt(br.readLine().trim());
        }

        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(compressedFile));
             BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outputFile))) {

            HuffmanNode node = root;
            int currentByte;
            List<Integer> bytesList = new ArrayList<>();
            while ((currentByte = bis.read()) != -1) bytesList.add(currentByte);

            for (int i = 0; i < bytesList.size(); i++) {
                int val = bytesList.get(i);
                int bitsToRead = (i == bytesList.size() - 1) ? lastBits : 8;

                for (int j = 7; j >= 8 - bitsToRead; j--) {
                    int bit = (val >> j) & 1;
                    node = (bit == 0) ? node.left : node.right;

                    if (node.isLeaf()) {
                        bos.write((byte) node.character);
                        node = root;
                    }
                }
            }
        }
    }
}
