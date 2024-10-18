int main() {
    volatile int i = 0;
    while (true) {
        while (i < 1000) {
            i++;
        }
        i = 0;
    }
    return 0;
}
