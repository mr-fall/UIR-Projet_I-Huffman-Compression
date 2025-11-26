import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) {
        if(args.length<1){
            System.out.println("Usage:");
            System.out.println("encode <inputFile> <outputFile> <codesFile>");
            System.out.println("decode <compressedFile> <metaFile> <codesFile> <outputFile>");
            return;
        }

        String cmd = args[0];
        try{
            if(cmd.equalsIgnoreCase("encode")){
                Map<Character,Integer> freq = HuffmanCoding.buildFrequencyTable(args[1]);
                HuffmanNode root = HuffmanCoding.buildHuffmanTree(freq);
                Map<Character,String> codeMap = new HashMap<>();
                HuffmanCoding.generateCodes(root,"",codeMap);

                HuffmanCoding.writeCodesToFile(codeMap,args[3]);
                HuffmanCoding.encodeFile(args[1],args[2],codeMap);

                System.out.println("Encoding done.");

            }else if(cmd.equalsIgnoreCase("decode")){
                HuffmanCoding.decodeFile(args[1],args[2],args[3],args[4]);
                System.out.println("Decoding done.");
            }else{
                System.out.println("Unknown command.");
            }
        }catch(Exception e){
            e.printStackTrace();
        }
    }
}
