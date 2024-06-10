#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>

// Функция для открытия файла и проверки успешности
FILE* open_file(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Основная функция обработки файлов
void process_files(const char* f_filename, const char* g_filename) {
    // Открытие файлов f и g для чтения в бинарном режиме
    FILE *f_file = open_file(f_filename, "rb");
    FILE *g_file = open_file(g_filename, "rb");

    // Создание временного файла
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Ошибка создания временного файла");
        fclose(f_file);
        fclose(g_file);
        exit(EXIT_FAILURE);
    }

    int fi, gi;
    // Чтение из файлов и запись в временный файл
    while (fread(&fi, sizeof(int), 1, f_file) == 1 && fread(&gi, sizeof(int), 1, g_file) == 1) {
        if (fi > gi) {
            fwrite(&fi, sizeof(int), 1, temp_file);
        }
    }

    // Закрытие исходных файлов
    fclose(f_file);
    fclose(g_file);

    // Открытие файла f для записи в бинарном режиме
    f_file = open_file(f_filename, "wb");

    // Перемещение указателя временного файла в начало
    rewind(temp_file);

    // Копирование данных из временного файла в f
    while (fread(&fi, sizeof(int), 1, temp_file) == 1) {
        fwrite(&fi, sizeof(int), 1, f_file);
    }

    // Закрытие файлов
    fclose(f_file);
    fclose(temp_file);
}

// Функция для автоматического тестирования
void run_tests() {
    const char *f_filename = "C:\\untitled3\\file\\f.bin";
    const char *g_filename = "C:\\untitled3\\file\\g.bin";

    // Создание тестовых данных
    int f_data[] = {10, 20, 30, 40, 50};
    int g_data[] = {15, 25, 5, 35, 60};
    int expected_data[] = {30, 40}; // ожидаемый результат после обработки

    // Запись тестовых данных в файлы
    FILE *f_file = open_file(f_filename, "wb");
    FILE *g_file = open_file(g_filename, "wb");
    fwrite(f_data, sizeof(int), 5, f_file);
    fwrite(g_data, sizeof(int), 5, g_file);
    fclose(f_file);
    fclose(g_file);

    // Вызов функции process_files
    process_files(f_filename, g_filename);

    // Проверка результата
    f_file = open_file(f_filename, "rb");
    int result_data[5];
    size_t result_size = fread(result_data, sizeof(int), 5, f_file);
    fclose(f_file);

    assert(result_size == 2); // проверка, что размер совпадает с ожидаемым
    for (size_t i = 0; i < result_size; i++) {
        assert(result_data[i] == expected_data[i]); // проверка содержимого
    }

    printf("Все тесты пройдены успешно.\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    run_tests();
    printf("Процесс завершен успешно.\n");
    return 0;
}