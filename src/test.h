






struct Test {
    uint32 widht;
    uint32 height;    
};

struct Memory {
    bool isInitiated;
    uint64 size;
    void* storage;
};

void InitTest(Memory* memory, uint32 width, uint32 height) {

    ASSERT(sizeof(Test) <= memory->size);
    
    Test* test = (Test*)memory->storage;
    if (!memory->isInitiated) {
        test->width = width;
        test->height = height;

        // @improve: Maybe more appropriate in the plateform layer.
        memory->isInitiated = true;
    }
}


int main() {

    // plateform alloc code
    Memory testMemory = {};
    memory->size = MB(64);
    memory->storage = VirtualAlloc(0, memory->size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    if (!memory->storage) {
        // panic/exit
    }

    InitTest(&memory, 100, 100);
}
