#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define FILENAME "data.txt"

typedef struct {
    int id;
    char nama[50];
    int umur;
} Mahasiswa;

typedef struct {
    int id;
    char nama[50];
    int harga;
    int stok;
} Barang;

Mahasiswa mainStorage[MAX];
int mainCount = 0;

Mahasiswa queue[MAX];
int front = -1, rear = -1;

Mahasiswa stack[MAX];
int top = -1;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
    getchar();
}

int isQueueEmpty() {
    return (front == -1);
}

int isQueueFull() {
    return (rear == MAX - 1);
}

void enqueue(Mahasiswa data) {
    if (isQueueFull()) {
        printf("Queue penuh! Tidak bisa menambah data.\n");
        return;
    }
    if (front == -1) front = 0;
    rear++;
    queue[rear] = data;
    printf("Data berhasil ditambahkan ke Queue!\n");
}

void dequeue() {
    if (isQueueEmpty()) {
        printf("Queue kosong! Tidak ada data untuk diproses.\n");
        return;
    }
    
    mainStorage[mainCount++] = queue[front];
    printf("Data '%s' dipindahkan ke Main Storage!\n", queue[front].nama);
    
    for (int i = front; i < rear; i++) {
        queue[i] = queue[i + 1];
    }
    rear--;
    
    if (rear == -1) {
        front = -1;
    }
}

void displayQueue() {
    if (isQueueEmpty()) {
        printf("Queue kosong!\n");
        return;
    }
    
    printf("\n=== ISI QUEUE ===\n");
    printf("%-5s %-30s %-10s\n", "ID", "Nama", "Umur");
    printf("--------------------------------------------------\n");
    for (int i = front; i <= rear; i++) {
        printf("%-5d %-30s %-10d\n", queue[i].id, queue[i].nama, queue[i].umur);
    }
}


int isStackEmpty() {
    return (top == -1);
}

int isStackFull() {
    return (top == MAX - 1);
}

void push(Mahasiswa data) {
    if (isStackFull()) {
        printf("Stack penuh!\n");
        return;
    }
    stack[++top] = data;
    printf("Data berhasil dihapus dan dipindahkan ke Stack (Temporary)!\n");
}

Mahasiswa pop() {
    if (isStackEmpty()) {
        Mahasiswa empty = {0, "", 0};
        return empty;
    }
    return stack[top--];
}

void restore() {
    if (isStackEmpty()) {
        printf("Stack kosong! Tidak ada data untuk direstore.\n");
        return;
    }
    
    Mahasiswa data = pop();
    mainStorage[mainCount++] = data;
    printf("Data '%s' berhasil dikembalikan ke Main Storage!\n", data.nama);
}

void displayStack() {
    if (isStackEmpty()) {
        printf("Stack kosong!\n");
        return;
    }
    
    printf("\n=== ISI STACK (Temporary) ===\n");
    printf("%-5s %-30s %-10s\n", "ID", "Nama", "Umur");
    printf("--------------------------------------------------\n");
    for (int i = top; i >= 0; i--) {
        printf("%-5d %-30s %-10d\n", stack[i].id, stack[i].nama, stack[i].umur);
    }
}


void displayMainStorage() {
    if (mainCount == 0) {
        printf("Main Storage kosong!\n");
        return;
    }
    
    printf("\n=== MAIN STORAGE ===\n");
    printf("%-5s %-30s %-10s\n", "ID", "Nama", "Umur");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < mainCount; i++) {
        printf("%-5d %-30s %-10d\n", mainStorage[i].id, mainStorage[i].nama, mainStorage[i].umur);
    }
}

void deleteFromMainStorage(int id) {
    int found = -1;
    for (int i = 0; i < mainCount; i++) {
        if (mainStorage[i].id == id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("Data dengan ID %d tidak ditemukan!\n", id);
        return;
    }
    
    push(mainStorage[found]);
    
    for (int i = found; i < mainCount - 1; i++) {
        mainStorage[i] = mainStorage[i + 1];
    }
    mainCount--;
}


void bubbleSort(int ascending) {
    for (int i = 0; i < mainCount - 1; i++) {
        for (int j = 0; j < mainCount - i - 1; j++) {
            int condition = ascending ? 
                (mainStorage[j].id > mainStorage[j + 1].id) : 
                (mainStorage[j].id < mainStorage[j + 1].id);
            
            if (condition) {
                Mahasiswa temp = mainStorage[j];
                mainStorage[j] = mainStorage[j + 1];
                mainStorage[j + 1] = temp;
            }
        }
    }
}

void selectionSort(int ascending) {
    for (int i = 0; i < mainCount - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < mainCount; j++) {
            int condition = ascending ? 
                (mainStorage[j].id < mainStorage[idx].id) : 
                (mainStorage[j].id > mainStorage[idx].id);
            
            if (condition) {
                idx = j;
            }
        }
        Mahasiswa temp = mainStorage[i];
        mainStorage[i] = mainStorage[idx];
        mainStorage[idx] = temp;
    }
}

void insertionSort(int ascending) {
    for (int i = 1; i < mainCount; i++) {
        Mahasiswa key = mainStorage[i];
        int j = i - 1;
        
        while (j >= 0 && ((ascending && mainStorage[j].id > key.id) || 
                          (!ascending && mainStorage[j].id < key.id))) {
            mainStorage[j + 1] = mainStorage[j];
            j--;
        }
        mainStorage[j + 1] = key;
    }
}

int partition(int low, int high, int ascending) {
    int pivot = mainStorage[high].id;
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        int condition = ascending ? 
            (mainStorage[j].id < pivot) : 
            (mainStorage[j].id > pivot);
        
        if (condition) {
            i++;
            Mahasiswa temp = mainStorage[i];
            mainStorage[i] = mainStorage[j];
            mainStorage[j] = temp;
        }
    }
    
    Mahasiswa temp = mainStorage[i + 1];
    mainStorage[i + 1] = mainStorage[high];
    mainStorage[high] = temp;
    
    return i + 1;
}

void quickSortHelper(int low, int high, int ascending) {
    if (low < high) {
        int pi = partition(low, high, ascending);
        quickSortHelper(low, pi - 1, ascending);
        quickSortHelper(pi + 1, high, ascending);
    }
}

void quickSort(int ascending) {
    quickSortHelper(0, mainCount - 1, ascending);
}

void merge(int left, int mid, int right, int ascending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    Mahasiswa L[n1], R[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = mainStorage[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = mainStorage[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        int condition = ascending ? 
            (L[i].id <= R[j].id) : 
            (L[i].id >= R[j].id);
        
        if (condition) {
            mainStorage[k++] = L[i++];
        } else {
            mainStorage[k++] = R[j++];
        }
    }
    
    while (i < n1)
        mainStorage[k++] = L[i++];
    while (j < n2)
        mainStorage[k++] = R[j++];
}

void mergeSortHelper(int left, int right, int ascending) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(left, mid, ascending);
        mergeSortHelper(mid + 1, right, ascending);
        merge(left, mid, right, ascending);
    }
}

void mergeSort(int ascending) {
    mergeSortHelper(0, mainCount - 1, ascending);
}


int binarySearch(int id) {
    bubbleSort(1);
    
    int left = 0, right = mainCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (mainStorage[mid].id == id)
            return mid;
        
        if (mainStorage[mid].id < id)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int linearSearch(char nama[]) {
    for (int i = 0; i < mainCount; i++) {
        if (strcasecmp(mainStorage[i].nama, nama) == 0) {
            return i;
        }
    }
    return -1;
}


int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int sumArray(int arr[], int n) {
    if (n <= 0) return 0;
    return arr[n - 1] + sumArray(arr, n - 1);
}


void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error membuka file!\n");
        return;
    }
    
    fprintf(file, "%d\n", mainCount);
    for (int i = 0; i < mainCount; i++) {
        fprintf(file, "%d|%s|%d\n", mainStorage[i].id, mainStorage[i].nama, mainStorage[i].umur);
    }
    
    fclose(file);
    printf("Data berhasil disimpan ke file '%s'!\n", FILENAME);
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("File tidak ditemukan. Memulai dengan data kosong.\n");
        return;
    }
    
    fscanf(file, "%d\n", &mainCount);
    for (int i = 0; i < mainCount; i++) {
        fscanf(file, "%d|%[^|]|%d\n", &mainStorage[i].id, mainStorage[i].nama, &mainStorage[i].umur);
    }
    
    fclose(file);
    printf("Data berhasil dimuat dari file '%s'!\n", FILENAME);
}


void menuInputData() {
    clearScreen();
    printf("\n=== TAMBAH DATA BARU ===\n");
    
    Mahasiswa data;
    printf("Masukkan ID: ");
    scanf("%d", &data.id);
    getchar();
    
    printf("Masukkan Nama: ");
    fgets(data.nama, sizeof(data.nama), stdin);
    data.nama[strcspn(data.nama, "\n")] = 0;
    
    printf("Masukkan Umur: ");
    scanf("%d", &data.umur);
    
    enqueue(data);
    pause();
}

void menuProcessData() {
    clearScreen();
    printf("\n=== PROSES DATA (Confirm Order) ===\n");
    displayQueue();
    
    if (!isQueueEmpty()) {
        printf("\nProses data pertama dari Queue ke Main Storage? (y/n): ");
        char confirm;
        scanf(" %c", &confirm);
        
        if (confirm == 'y' || confirm == 'Y') {
            dequeue();
        }
    }
    pause();
}

void menuDeleteData() {
    clearScreen();
    printf("\n=== HAPUS DATA ===\n");
    displayMainStorage();
    
    if (mainCount > 0) {
        int id;
        printf("\nMasukkan ID yang akan dihapus: ");
        scanf("%d", &id);
        deleteFromMainStorage(id);
    }
    pause();
}

void menuRestoreData() {
    clearScreen();
    printf("\n=== RESTORE DATA ===\n");
    displayStack();
    
    if (!isStackEmpty()) {
        printf("\nRestore data terakhir dari Stack? (y/n): ");
        char confirm;
        scanf(" %c", &confirm);
        
        if (confirm == 'y' || confirm == 'Y') {
            restore();
        }
    }
    pause();
}

void menuSortData() {
    clearScreen();
    printf("\n=== SORTING DATA ===\n");
    displayMainStorage();
    
    if (mainCount == 0) {
        pause();
        return;
    }
    
    printf("\nPilih Metode Sorting:\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("Pilihan: ");
    int method;
    scanf("%d", &method);
    
    printf("\nUrutan:\n");
    printf("1. Ascending (Kecil ke Besar)\n");
    printf("2. Descending (Besar ke Kecil)\n");
    printf("Pilihan: ");
    int order;
    scanf("%d", &order);
    
    int ascending = (order == 1);
    
    switch(method) {
        case 1: bubbleSort(ascending); break;
        case 2: selectionSort(ascending); break;
        case 3: insertionSort(ascending); break;
        case 4: mergeSort(ascending); break;
        case 5: quickSort(ascending); break;
        default: printf("Pilihan tidak valid!\n");
    }
    
    printf("\nData berhasil diurutkan!\n");
    displayMainStorage();
    pause();
}

void menuSearchData() {
    clearScreen();
    printf("\n=== PENCARIAN DATA ===\n");
    
    printf("1. Binary Search (berdasarkan ID)\n");
    printf("2. Linear Search (berdasarkan Nama)\n");
    printf("Pilihan: ");
    int method;
    scanf("%d", &method);
    getchar();
    
    if (method == 1) {
        int id;
        printf("Masukkan ID yang dicari: ");
        scanf("%d", &id);
        
        int result = binarySearch(id);
        if (result != -1) {
            printf("\nData ditemukan!\n");
            printf("ID: %d\n", mainStorage[result].id);
            printf("Nama: %s\n", mainStorage[result].nama);
            printf("Umur: %d\n", mainStorage[result].umur);
        } else {
            printf("Data tidak ditemukan!\n");
        }
    } else if (method == 2) {
        char nama[50];
        printf("Masukkan Nama yang dicari: ");
        fgets(nama, sizeof(nama), stdin);
        nama[strcspn(nama, "\n")] = 0;
        
        int result = linearSearch(nama);
        if (result != -1) {
            printf("\nData ditemukan!\n");
            printf("ID: %d\n", mainStorage[result].id);
            printf("Nama: %s\n", mainStorage[result].nama);
            printf("Umur: %d\n", mainStorage[result].umur);
        } else {
            printf("Data tidak ditemukan!\n");
        }
    }
    pause();
}

void menuRecursion() {
    clearScreen();
    printf("\n=== FUNGSI REKURSIF ===\n");
    
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Sum Total (contoh statistik)\n");
    printf("Pilihan: ");
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        int n;
        printf("Masukkan angka: ");
        scanf("%d", &n);
        printf("Factorial dari %d = %d\n", n, factorial(n));
    } else if (choice == 2) {
        int n;
        printf("Masukkan posisi Fibonacci: ");
        scanf("%d", &n);
        printf("Fibonacci ke-%d = %d\n", n, fibonacci(n));
    } else if (choice == 3) {
        if (mainCount == 0) {
            printf("Main Storage kosong!\n");
        } else {
            int ages[MAX];
            for (int i = 0; i < mainCount; i++) {
                ages[i] = mainStorage[i].umur;
            }
            printf("Total umur semua mahasiswa: %d\n", sumArray(ages, mainCount));
        }
    }
    pause();
}

void menuViewAll() {
    clearScreen();
    printf("\n=== LIHAT SEMUA DATA ===\n");
    
    printf("\n--- QUEUE (Pending) ---\n");
    displayQueue();
    
    printf("\n--- MAIN STORAGE ---\n");
    displayMainStorage();
    
    printf("\n--- STACK (Temporary/Deleted) ---\n");
    displayStack();
    
    pause();
}


int main() {
    loadFromFile();
    
    int choice;
    
    do {
        clearScreen();
        printf("\n");
        printf("╔════════════════════════════════════════╗\n");
        printf("║   SISTEM INFORMASI SEDERHANA BEBAS    ║\n");
        printf("║      BNCC LnT C Programming 2025      ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\n");
        printf("MENU UTAMA:\n");
        printf("1. Add New Data (Input ke Queue)\n");
        printf("2. Process Data (Queue -> Main Storage)\n");
        printf("3. Delete Data (Main Storage -> Stack)\n");
        printf("4. Restore Data (Stack -> Main Storage)\n");
        printf("5. View All Data\n");
        printf("6. Sort Data\n");
        printf("7. Search Data\n");
        printf("8. Summary Report (Rekursif)\n");
        printf("9. Save & Exit\n");
        printf("0. Exit (tanpa save)\n");
        printf("\nPilihan: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: menuInputData(); break;
            case 2: menuProcessData(); break;
            case 3: menuDeleteData(); break;
            case 4: menuRestoreData(); break;
            case 5: menuViewAll(); break;
            case 6: menuSortData(); break;
            case 7: menuSearchData(); break;
            case 8: menuRecursion(); break;
            case 9: 
                saveToFile();
                printf("Terima kasih!\n");
                break;
            case 0:
                printf("Exit tanpa menyimpan. Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
                pause();
        }
    } while(choice != 9 && choice != 0);
    
    return 0;
}
