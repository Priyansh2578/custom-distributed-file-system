void main() {
    char *video_memory = (char*) 0xb8000;
    char *msg = "CDFS-OS Kernel";
    int i = 0;

    while (msg[i] != 0) {
        video_memory[i * 2] = msg[i];
        video_memory[i * 2 + 1] = 0x0f;
        i++;
    }
    while(1);
}