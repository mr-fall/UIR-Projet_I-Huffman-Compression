public class MinHeap {
    private HuffmanNode[] heap;
    private int size;
    private int capacity;

    public MinHeap(int capacity) {
        this.capacity = capacity;
        this.heap = new HuffmanNode[capacity];
        this.size = 0;
    }

    private void swap(int i, int j) {
        HuffmanNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    private void heapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].frequency < heap[smallest].frequency)
            smallest = left;
        if (right < size && heap[right].frequency < heap[smallest].frequency)
            smallest = right;
        if (smallest != i) {
            swap(i, smallest);
            heapify(smallest);
        }
    }

    public void insert(HuffmanNode node) {
        if (size == capacity) {
            HuffmanNode[] newHeap = new HuffmanNode[capacity * 2];
            System.arraycopy(heap, 0, newHeap, 0, capacity);
            heap = newHeap;
            capacity *= 2;
        }
        int i = size;
        heap[size++] = node;

        while (i != 0) {
            int parent = (i - 1) / 2;
            if (heap[parent].frequency > heap[i].frequency) {
                swap(parent, i);
                i = parent;
            } else break;
        }
    }

    public HuffmanNode extractMin() {
        if (size == 0) return null;
        HuffmanNode root = heap[0];
        heap[0] = heap[size - 1];
        heap[size - 1] = null;
        size--;
        heapify(0);
        return root;
    }

    public static MinHeap buildHeap(HuffmanNode[] nodes, int n) {
        MinHeap mh = new MinHeap(Math.max(1, n));
        mh.size = n;
        mh.capacity = Math.max(1, n);
        mh.heap = new HuffmanNode[mh.capacity];
        System.arraycopy(nodes, 0, mh.heap, 0, n);

        for (int i = (mh.size / 2) - 1; i >= 0; i--)
            mh.heapify(i);
        return mh;
    }

    public int getSize() { return size; }
}
